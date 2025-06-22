#pragma once
#include <string>
#include <ctime>

namespace util {
    bool parse_date(const std::string& text, std::tm& out);
    std::string date_to_string(const std::tm& tm);
} 