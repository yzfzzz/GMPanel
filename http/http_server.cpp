#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/http/HttpRequest.h>
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/http/HttpServer.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>
#include "json.hpp"

using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

// 模拟用户数据库
std::unordered_map<std::string, std::string> users = {{"admin", "password123"},
                                                      {"user", "123456"}};

void onRequest(const HttpRequest& req, HttpResponse* resp) {
    // 打印请求信息
    LOG_INFO << "Method: " << req.methodString();
    LOG_INFO << "Path: " << req.path();
    LOG_INFO << "Version: " << req.getVersion();

    std::string path = req.path();
    if (path == "/" || path == "/index.html" || path == "/home.html") {
        if (path == "/") {
            path = "/index.html";
        }
        std::ifstream file("." + path, std::ios::in | std::ios::binary);
        if (!file) {
            resp->setStatusCode(HttpResponse::k404NotFound);
            resp->setStatusMessage("Not Found");
            resp->setBody("File not found");
            return;
        }
        std::ostringstream content;
        content << file.rdbuf();
        file.close();
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->setBody(content.str());
    } else if (req.path() == "/data") {
        json j;
        j["userInfo"] = "Alice";
        j["gpuModel"] = "AMD 4090";
        j["cpuUsage"] = 99;
        j["memoryUsage"] = 80;
        j["gpuChart"] = {10, 20, 50, 40, 80, 10};
        j["netUpload"] = {39, 20, 50, 80, 90};
        j["netDownload"] = {19, 90, 70, 10, 20};
        std::string json_str = j.dump();

        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/plain");
        resp->setBody(json_str);
    } else if (req.path() == "/login") {
        const std::string& request_query = req.query();
        // 定义正则表达式
        std::regex pattern(R"(\?username=([^&]+)&password=([^&]+))");
        // 用于存储匹配结果
        std::smatch matches;
        // 解析表单数据（格式为 username=admin&password=password123）
        std::string username, password;
        // 进行匹配
        if (std::regex_match(request_query, matches, pattern)) {
            std::cout << "Matched URL: " << matches[0] << std::endl;
            // 提取捕获组
            username = matches[1];
            password = matches[2];
            std::cout << "Username: " << username << std::endl;
            std::cout << "Password: " << password << std::endl;
        } else {
            std::cout << "Not matched: " << request_query << std::endl;
        }

        // 验证账号和密码
        if (users.find(username) != users.end() &&
            users[username] == password) {
            resp->setStatusCode(HttpResponse::k200Ok);
            resp->setStatusMessage("OK");
            resp->setContentType("text/plain");
            resp->setBody("Login successful");
        } else {
            resp->setStatusCode(HttpResponse::k400BadRequest);
            resp->setStatusMessage("Unauthorized");
            resp->setContentType("text/plain");
            resp->setBody("Invalid username or password");
        }
    } else {
        // 其他请求返回 404
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setContentType("text/html");
        resp->setBody("<h1>404 Not Found</h1>");
    }
}

int main() {
    EventLoop loop;
    HttpServer server(&loop, InetAddress("10.0.4.3", 80), "http_server");
    server.setHttpCallback(onRequest);
    server.start();
    loop.loop();
}