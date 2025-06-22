#include "student_publisher.h"
#include "util.h"
#include <sstream>
#include <iostream>

StudentPublisher::StudentPublisher(const std::string& ep)
{
    sock_.bind(ep);
    std::cout << "[INFO] PUB " << ep << " bound\n";
}

void StudentPublisher::publish(const std::vector<Student>& list)
{
    for (const auto& s : list) {
        std::ostringstream oss; oss << s.id << ' ' << s.full_name << ' ' << util::date_to_string(s.birthday);
        auto str = oss.str();
        sock_.send(zmq::buffer(str), zmq::send_flags::none);
        std::cout << "[PUB] " << str << '\n';
    }
}