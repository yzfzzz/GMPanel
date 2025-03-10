#include <fmt/format.h>
#include <fmt/ostream.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/http/HttpRequest.h>
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/http/HttpServer.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "client/rpc_client.h"
#include "json.hpp"
#include "log.h"
#include "query_data.h"
using namespace muduo;
using namespace muduo::net;
std::string server_address = "124.223.141.236:50051";
std::unordered_set<std::string> paths;

// 新增函数，用于检测是否是长连接
bool isKeepAlive(const HttpRequest& req) {
    const std::string& connectionHeader = req.getHeader("Connection");
    if (req.getVersion() == HttpRequest::kHttp10) {
        // HTTP/1.0 默认是短连接，除非明确指定 Keep-Alive
        return connectionHeader == "Keep-Alive";
    } else {
        // HTTP/1.1 及以上默认是长连接，除非明确指定 close
        return connectionHeader != "close";
    }
}

void onRequest(const HttpRequest& req, HttpResponse* resp) {
    bool keepAlive = isKeepAlive(req);

    // 打印请求信息
    LOG(INFO) << "Method: " << req.methodString() << " Path: " << req.path()
              << " Version: " << req.getVersion();

    std::string path = req.path();
    if (paths.find(path) != paths.end()) {
        if (path == "/") {
            path = "/index.html";
        }
        std::ifstream file("./resource" + path,
                           std::ios::in | std::ios::binary);
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
        if (!keepAlive) {
            resp->setCloseConnection(true);
        }
    } else if (path == "/data") {
        monitor::QueryData query_data;
        const std::string& request_query = req.query();
        std::regex pattern(R"(\?account_num=([^&]+)&machine_name=([^&]+))");
        std::smatch matches;
        std::string account_num, machine_name;
        if (std::regex_match(request_query, matches, pattern)) {
            account_num = matches[1];
            machine_name = matches[2];
        }
        LOG(INFO) << fmt::format("request: account_num:{} machine_name:{}",
                                 account_num, machine_name);

        if (!account_num.empty() &&
            query_data.queryDataInfo(account_num, machine_name)) {
            LOG_INFO << "2";
            resp->setStatusCode(HttpResponse::k200Ok);
            resp->setStatusMessage("OK");
            resp->setContentType("text/plain");
            resp->setBody(query_data.toJsonStr());
        } else {
            resp->setStatusCode(HttpResponse::k400BadRequest);
            resp->setStatusMessage("Unauthorized");
            resp->setContentType("text/plain");
            resp->setBody("Invalid account num");
        }
    } else if (path == "/signup") {
        const std::string& request_query = req.query();
        std::regex pattern(R"(\?email=([^&]+)&password=([^&]+))");
        std::smatch matches;
        std::string email, password;
        if (std::regex_match(request_query, matches, pattern)) {
            email = matches[1];
            password = matches[2];
        }
        monitor::UserManage user_manage;

        if (!password.empty() && user_manage.signup(password)) {
            // 注册
            resp->setStatusCode(HttpResponse::k200Ok);
            resp->setStatusMessage("OK");
            resp->setContentType("text/plain");
            resp->setBody(user_manage.data.dump());
        } else {
            resp->setStatusCode(HttpResponse::k400BadRequest);
            resp->setStatusMessage("Unauthorized");
            resp->setContentType("text/plain");
            resp->setBody("Invalid username or password");
        }
    } else if (path == "/login") {
        const std::string& request_query = req.query();
        // 定义正则表达式
        std::regex pattern(R"(\?username=([^&]+)&password=([^&]+))");
        // 用于存储匹配结果
        std::smatch matches;
        // 解析表单数据（格式为 username=admin&password=password123）
        std::string username, password;
        // 进行匹配
        if (std::regex_match(request_query, matches, pattern)) {
            // 提取捕获组
            username = matches[1];
            password = matches[2];
        }
        monitor::UserManage user_manage;

        if (!username.empty() && !password.empty() &&
            user_manage.login(username, password)) {
            // 登录
            resp->setStatusCode(HttpResponse::k200Ok);
            resp->setStatusMessage("OK");
            resp->setContentType("text/plain");
            resp->setBody(user_manage.data.dump());
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
    // 根据是否是长连接设置响应头
    if (!keepAlive) {
        resp->setCloseConnection(true);
    }
}

int main() {
    YAML::Node config = YAML::LoadFile("http_server_path.yaml");
    for (auto c : config["file"]) {
        paths.insert(c.as<std::string>());
    }
    
    EventLoop loop;
    HttpServer server(&loop, InetAddress("10.0.4.3", 80), "http_server");
    server.setHttpCallback(onRequest);
    server.start();
    loop.loop();
}