#pragma once
#include "student.h"
#include <vector>
#include <thread>

class ITransport {
public:
    virtual ~ITransport() = default;
    virtual void start() = 0;     
    virtual void stop() = 0;      
    virtual void publish(const std::vector<Student>& list) = 0; 
};