#include "util.h"
#include <regex>
#include <sstream>
#include <iomanip>

namespace util {
    bool parse_date(const std::string& text, std::tm& out)
    {
        static const std::regex re(R"((\d{1,2})\.(\d{1,2})\.(\d{4}))");
        std::smatch m;
        if (!std::regex_match(text, m, re)) return false;
        int d = std::stoi(m[1]);
        int mo = std::stoi(m[2]);
        int y = std::stoi(m[3]);
        if (y < 1900 || y > 2100 || mo < 1 || mo > 12 || d < 1 || d > 31) return false;
        static const int dim[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
        int days = dim[mo - 1];
        if (mo == 2) {
            bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
            days = leap ? 29 : 28;
        }
        if (d > days) return false;
        out = {};
        out.tm_mday = d; out.tm_mon = mo - 1; out.tm_year = y - 1900;
        return true;
    }

    std::string date_to_string(const std::tm& tm)
    {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << tm.tm_mday << '.'
            << std::setw(2) << tm.tm_mon + 1 << '.'
            << std::setw(4) << tm.tm_year + 1900;
        return oss.str();
    }
} 