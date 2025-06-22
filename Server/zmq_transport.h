#pragma once
#include "transport.h"
#include <zmq.hpp>
#include <atomic>

class ZmqTransport : public ITransport {
public:
    explicit ZmqTransport(std::string endpoint = "tcp://*:5555");
    void start() override;
    void publish(const std::vector<Student>& list) override;
    void stop()  override;
private:
    void worker();
    std::string               ep_;
    std::vector<Student>      cache_;
    std::thread               th_;
    std::atomic<bool>         running_{ false };
};
