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
#include <vector>
#include "client/rpc_client.h"
#include "json.hpp"
#include "mprpcapplication.h"
#include "query_data.h"
using namespace muduo;
using namespace muduo::net;
// using json = nlohmann::json;

int h_argv;
char** h_argc;

// 模拟用户数据库
std::unordered_map<std::string, std::string> users = {{"admin", "password123"},
                                                      {"user", "123456"}};

void onRequest(const HttpRequest& req, HttpResponse* resp) {
    // 打印请求信息
    LOG_INFO << "Method: " << req.methodString() << " Path: " << req.path()
             << " Version: " << req.getVersion();

    std::string path = req.path();
    if (path == "/" || path == "/index.html" || path == "/home.html") {
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
    } else if (path == "/data") {
        monitor::RpcClient rpc_client;
        monitor::QueryData query_data;

        const std::string& request_query = req.query();
        // 定义正则表达式
        std::regex pattern(R"(\?account_num=([^&]+)&machine_name=([^&]+))");
        // 用于存储匹配结果
        std::smatch matches;
        // 解析表单数据（格式为 username=admin&password=password123）
        std::string account_num, machine_name;
        // 进行匹配
        if (std::regex_match(request_query, matches, pattern)) {
            // 提取捕获组
            account_num = matches[1];
            machine_name = matches[2];
        }
        std::cout << "account_num:" << account_num << std::endl;
        std::cout << "machine_name" << machine_name << std::endl;
        if (query_data.queryDataInfo(account_num, machine_name, 5,
                                     rpc_client)) {
            resp->setStatusCode(HttpResponse::k200Ok);
            resp->setStatusMessage("OK");
            resp->setContentType("text/plain");
            resp->setBody(query_data.toJsonStr());
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
        monitor::Login l;

        // 验证账号和密码
        if (l.login(username, password)) {
            resp->setStatusCode(HttpResponse::k200Ok);
            resp->setStatusMessage("OK");
            resp->setContentType("text/plain");
            resp->setBody(l.data.dump());
        } else {
            resp->setStatusCode(HttpResponse::k400BadRequest);
            resp->setStatusMessage("Unauthorized");
            resp->setContentType("text/plain");
            resp->setBody(l.data.dump());
        }
    } else {
        // 其他请求返回 404
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setContentType("text/html");
        resp->setBody("<h1>404 Not Found</h1>");
    }
}

int main(int argv, char** argc) {
    h_argc = argc;
    h_argv = argv;
    MprpcApplication::Init(h_argv, h_argc);
    EventLoop loop;
    HttpServer server(&loop, InetAddress("10.0.4.3", 80), "http_server");
    server.setHttpCallback(onRequest);
    server.start();
    loop.loop();
}