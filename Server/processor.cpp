#include "processor.h"
#include "student_parser.h"
#include "student_merger.h"
#include "transport.h"
#include <memory>

void run_processing(const std::string& f1,
    const std::string& f2,
    const std::vector<std::shared_ptr<ITransport>>& transports)
{
    StudentParser parser;  std::vector<Student> raw;
    parser.parse_file(f1, raw);
    parser.parse_file(f2, raw);

    StudentMerger m;
    auto uniq = m.merge(raw);

    for (auto& t : transports) t->publish(uniq);
}