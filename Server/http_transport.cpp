#include "http_transport.h"
#include "util.h"

HttpTransport::HttpTransport(int p) :port_(p) {}
void HttpTransport::start() { running_ = true; th_ = std::thread(&HttpTransport::worker, this); }
void HttpTransport::publish(const std::vector<Student>& list) { cache_ = list; }
void HttpTransport::stop() { running_ = false; if (th_.joinable()) th_.join(); }

void HttpTransport::worker() {
    httplib::Server svr;
    svr.Get("/students", [this](const httplib::Request&, httplib::Response& res) {
        nlohmann::json j = nlohmann::json::array();
        for (const auto& s : cache_) {
            j.push_back({ {"id",s.id},
                         {"name",s.full_name},
                         {"birthday",util::date_to_string(s.birthday)} });
        }
        res.set_content(j.dump(2), "application/json");
        });
    svr.listen("0.0.0.0", port_); 
}