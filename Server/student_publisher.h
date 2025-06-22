#pragma once
#include "student.h"
#include <zmq.hpp>
#include <vector>
#include <string>

class StudentPublisher {
public:
    explicit StudentPublisher(const std::string& endpoint);
    void publish(const std::vector<Student>& list);
private:
    zmq::context_t ctx_{ 1 };
    zmq::socket_t  sock_{ ctx_, zmq::socket_type::pub };
};