#pragma once
#include "transport.h"
#include <httplib.h>        
#include <nlohmann/json.hpp>
#include <atomic>

class HttpTransport : public ITransport {
public:
    explicit HttpTransport(int port = 8080);
    void start() override;
    void publish(const std::vector<Student>& list) override;
    void stop()  override;
private:
    void worker();
    int                       port_;
    std::vector<Student>      cache_;
    std::thread               th_;
    std::atomic<bool>         running_{ false };
};