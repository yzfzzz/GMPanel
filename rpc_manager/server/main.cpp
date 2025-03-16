#include <iostream>
#include "rpc_manager.h"

#if RPC_TYPE_DEFINE == GRPC
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
constexpr char kServerPortInfo[] = "0.0.0.0:50051";
void InitServer() {
    grpc::ServerBuilder builder;
    builder.AddListeningPort(kServerPortInfo,
                             grpc::InsecureServerCredentials());
    monitor::ServerManagerImpl server_manager;
    monitor::UserManagerImpl user_manager;
    builder.RegisterService(&server_manager);
    builder.RegisterService(&user_manager);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
    return;
}
#endif

#if RPC_TYPE_DEFINE == MPRPC
int main() {
    std::string log_path = "./logs";
    monitor::SetupLogging(log_path);
    // 调用框架的初始化操作
    MprpcApplication::Init(rpc_config_path);
    RpcProvider provider;
    // 在zookeeper中挂载函数
    provider.NotifyService(new monitor::ServerManagerImpl());
    provider.NotifyService(new monitor::UserManagerImpl());
    // 启动一个rpc服务发布节点, Run以后进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();
    return 0;
}
#elif RPC_TYPE_DEFINE == GRPC
int main() {
    InitServer();
    return 0;
}
#endif
