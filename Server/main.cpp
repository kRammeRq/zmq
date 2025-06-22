#include "student_parser.h"
#include "student_merger.h"
#include "student_publisher.h"
#include "zmq_transport.h"
#include "http_transport.h"
#include "processor.h"
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>
#include <cctype>

static std::string trim(std::string s)
{
    auto not_space = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
    return s;
}

static std::string trim_null_and_space(std::string s)
{
    while (!s.empty() && (s.back() == '\0' || std::isspace((unsigned char)s.back()))) {
        s.pop_back();
    }
    while (!s.empty() && (s.front() == '\0' || std::isspace((unsigned char)s.front()))) {
        s.erase(s.begin());
    }
    return s;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " file1 file2\n";
        return 1;
    }
    StudentParser parser; std::vector<Student> raw;
    parser.parse_file(argv[1], raw);
    parser.parse_file(argv[2], raw);
    StudentMerger m; auto uniq = m.merge(raw);
    auto http = std::make_shared<HttpTransport>();
    http->start();              
    http->publish(uniq);        
    auto zmq = std::make_shared<ZmqTransport>(); 
    std::thread hs([&zmq, &uniq] {
        zmq::context_t ctx(1);
        zmq::socket_t  rep(ctx, zmq::socket_type::rep);
        rep.bind("tcp://*:5556");
        std::cout << "[HS] waiting READY on 5556...\n";
        zmq::message_t req;
        rep.recv(req);                                   
        std::string s(static_cast<char*>(req.data()), req.size());
        if (trim_null_and_space(s) == "READY") {
            rep.send(zmq::buffer("OK"), zmq::send_flags::none);
            std::cout << "[HS] OK sent.  Starting ZMQ broadcast.\n";
            zmq->publish(uniq);    
            zmq->start();                 
        }
        });
    std::cout << "HTTP ready  ? http://localhost:8080/students\n"
        "Awaiting ZMQ handshake (PUB tcp://*:5555)…\n";

    std::this_thread::sleep_for(std::chrono::hours(24));
    zmq->stop();  http->stop();  hs.join();

    
}