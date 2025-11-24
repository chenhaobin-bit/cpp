#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <mutex>
#include <iostream>

using json = nlohmann::json;

std::unordered_map<std::string, std::pair<std::string, std::string>> users;
std::mutex user_mutex;

int main() {
    httplib::Server svr;

    svr.Get("/hello", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("hello", "text/plain");
    });

    svr.Post("/register", [](const httplib::Request& req, httplib::Response& res) {
        auto data = json::parse(req.body, nullptr, false);
        if (data.is_discarded() ||
            !data.contains("name") ||
            !data.contains("password") ||
            !data.contains("info")) {
            res.set_content("failed", "text/plain");
            return;
        }

        std::string name = data["name"];
        std::string pwd  = data["password"];
        std::string info = data["info"];

        std::lock_guard<std::mutex> lock(user_mutex);
        users[name] = {pwd, info};
        res.set_content("ok", "text/plain");
    });

    svr.Post("/login", [](const httplib::Request& req, httplib::Response& res) {
        auto data = json::parse(req.body, nullptr, false);
        if (data.is_discarded() ||
            !data.contains("name") ||
            !data.contains("password")) {
            res.set_content("failed", "text/plain");
            return;
        }

        std::string name = data["name"];
        std::string pwd  = data["password"];

        std::lock_guard<std::mutex> lock(user_mutex);
        auto it = users.find(name);
        if (it == users.end() || it->second.first != pwd) {
            res.set_content("failed", "text/plain");
            return;
        }

        json resp = {
            {"status", "ok"},
            {"info", it->second.second}
        };
        res.set_content(resp.dump(), "application/json");
    });

    std::cout << "Server started.\n";
    svr.listen("127.0.0.1", 11451);
}
