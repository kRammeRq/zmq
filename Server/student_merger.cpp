#include "student_merger.h"
#include <unordered_map>

std::vector<Student> StudentMerger::merge(const std::vector<Student>& in)
{
    std::unordered_map<StudentKey, Student, StudentKeyHash> m;
    for (const auto& s : in) {
        StudentKey k{ s.full_name,s.birthday };
        auto it = m.find(k);
        if (it == m.end() || s.id < it->second.id) m[k] = s;
    }
    std::vector<Student> out; out.reserve(m.size());
    for (auto& p : m) out.push_back(std::move(p.second));
    return out;
}