#include "student.h"
#include <vector>
#include <string>

class StudentParser {
public:
    void parse_file(const std::string& path, std::vector<Student>& out);
private:
    bool parse_line(const std::string& line, Student& out);
};