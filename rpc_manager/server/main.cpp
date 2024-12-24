#include <iostream>
#include <string>
#include <vector>
#include "rpc_manager.h"
#include "log.h"

int main(int argv, char** argc) {
    std::string log_path = "./logs";
    monitor::SetupLogging(log_path);
    // 调用框架的初始化操作
    MprpcApplication::Init(argv, argc);

    RpcProvider provider;
    provider.NotifyService(new monitor::ServerManagerImpl());

    // 启动一个rpc服务发布节点, Run以后进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}