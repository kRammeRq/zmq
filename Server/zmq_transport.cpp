#include "zmq_transport.h"
#include "util.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <iostream>

ZmqTransport::ZmqTransport(std::string ep) :ep_(std::move(ep)) {}
void ZmqTransport::start() {
    running_ = true;
    th_ = std::thread(&ZmqTransport::worker, this);
}
void ZmqTransport::publish(const std::vector<Student>& list) { cache_ = list; }
void ZmqTransport::stop() {
    running_ = false;
    if (th_.joinable()) th_.join();
}
void ZmqTransport::worker() {
    zmq::context_t ctx;
    zmq::socket_t  pub(ctx, zmq::socket_type::pub);
    pub.bind(ep_);
    while (running_) {
        for (const auto& s : cache_) {
            std::ostringstream oss;
            oss << s.id << ' ' << s.full_name << ' ' << util::date_to_string(s.birthday);
            pub.send(zmq::buffer(oss.str()), zmq::send_flags::none);
        }
        pub.send(zmq::buffer("__END__"), zmq::send_flags::none);
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
}

