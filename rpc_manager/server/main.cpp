#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include <iostream>
#include "rpc_manager.h"

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

int main() {
    InitServer();
    return 0;
}