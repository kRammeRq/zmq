#include "student.h"
#include "util.h"
#include <zmq.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

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

static bool parse_student_message(const std::string& line, Student& out)
{
    std::istringstream iss(line);
    if (!(iss >> out.id)) return false;
    std::string tok; std::vector<std::string> names;
    while (iss >> tok) {
        std::tm dt{};
        if (util::parse_date(tok, dt)) {
            out.birthday = dt;
            break;
        }
        names.push_back(tok);
    }
    if (names.empty() || out.birthday.tm_year == 0) return false;

    out.full_name.clear();
    for (size_t i = 0; i < names.size(); ++i) {
        if (i) out.full_name += ' ';
        out.full_name += names[i];
    }
    return true;
}

int main(int argc, char* argv[])
{
    zmq::context_t ctx;
    zmq::socket_t sub(ctx, zmq::socket_type::sub);
    std::string endpoint = "tcp://localhost:5555";
    if (argc >= 2) endpoint = argv[1];
    
    sub.connect(endpoint);
    sub.set(zmq::sockopt::subscribe, "");
    std::cout << "[INFO] SUB connected to " << endpoint << '\n';
    zmq::socket_t handshake_socket(ctx, zmq::socket_type::req);
    handshake_socket.connect("tcp://localhost:5556");
    std::cout << "[INFO] Sending handshake...\n";
    handshake_socket.send(zmq::buffer("READY"), zmq::send_flags::none);
    zmq::message_t reply_msg;
    handshake_socket.recv(reply_msg, zmq::recv_flags::none);
    std::string reply(static_cast<char*>(reply_msg.data()), reply_msg.size());
    reply = trim_null_and_space(reply);
    if (reply != "OK") {
        std::cerr << "[ERROR] Unexpected handshake reply: " << reply << '\n';
        return 1;
    }
    handshake_socket.close();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::vector<Student> students;
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    bool got_first = false;
    while (true) {
        zmq::message_t msg;
        sub.recv(msg);                    
        std::string line(static_cast<char*>(msg.data()), msg.size());
        line = trim_null_and_space(line);
        if (line == "__END__") {          
            std::cout << "[INFO] End marker received, stop receiving.\n";
            break;
        }

        std::cout << "[DEBUG] " << line << '\n';

        Student s;
        if (parse_student_message(line, s))
            students.push_back(std::move(s));
    }
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.full_name < b.full_name;
        });
    std::cout << "------ Sorted list ------\n";
    for (const auto& s : students) {
        std::cout << s.full_name << " (" << s.id << ") "
            << util::date_to_string(s.birthday) << '\n';
    }
    std::cout << "-------------------------\n";
    return 0;
}