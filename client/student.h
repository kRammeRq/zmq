#pragma once
#include <string>
#include <ctime>
#include <functional>

struct Student {
    int id = 0;
    std::string full_name;
    std::tm     birthday{};
};

struct StudentKey {
    std::string full_name;
    std::tm     birthday;
    bool operator==(const StudentKey& o) const noexcept {
        return full_name == o.full_name && birthday.tm_mday == o.birthday.tm_mday && birthday.tm_mon == o.birthday.tm_mon && birthday.tm_year == o.birthday.tm_year;
    }
};

struct StudentKeyHash {
    std::size_t operator()(const StudentKey& k) const noexcept {
        std::size_t h1 = std::hash<std::string>{}(k.full_name);
        std::size_t h2 = (k.birthday.tm_year << 16) ^ (k.birthday.tm_mon << 8) ^ k.birthday.tm_mday;
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ull + (h1 << 6) + (h1 >> 2));
    }
};