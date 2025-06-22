#include "student_parser.h"
#include "util.h"
#include <fstream>
#include <sstream>
#include <iostream>

void StudentParser::parse_file(const std::string& path, std::vector<Student>& out)
{
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Cannot open file " + path);
    std::string line; size_t lineno = 0;
    while (std::getline(in, line)) {
        ++lineno;
        if (line.empty()) continue;
        Student s;
        if (!parse_line(line, s)) {
            std::cerr << "[WARN] " << path << ":" << lineno << " invalid\n";
            continue;
        }
        out.push_back(std::move(s));
    }
}

bool StudentParser::parse_line(const std::string& line, Student& out)
{
    std::istringstream iss(line);
    if (!(iss >> out.id)) return false;
    std::vector<std::string> tokens; std::string tok;
    while (iss >> tok) {
        std::tm dt{};
        if (util::parse_date(tok, dt)) { out.birthday = dt; break; }
        tokens.push_back(tok);
    }
    if (tokens.empty() || out.birthday.tm_year == 0) return false;
    out.full_name.clear();
    for (size_t i = 0; i < tokens.size(); ++i) { if (i) out.full_name += ' '; out.full_name += tokens[i]; }
    return true;
}