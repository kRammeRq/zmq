#pragma once
#include <string>
#include <vector>
#include <memory>
#include "transport.h"

void run_processing(const std::string& f1,
    const std::string& f2,
    const std::vector<std::shared_ptr<ITransport>>& transports);