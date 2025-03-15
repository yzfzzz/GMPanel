#pragma once
#include <fmt/format.h>
#include <memory>
#include <vector>
#include "config.h"
#include "json.hpp"
#include "log.h"
#include "login_register.pb.h"
#include "monitor_info.pb.h"

#if RPC_TYPE_DEFINE == MPRPC
#include "mprpcapplication.h"
#elif RPC_TYPE_DEFINE == GRPC
#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include "login_register.grpc.pb.h"
#include "monitor_info.grpc.pb.h"
#endif

namespace monitor {

class RpcClient {
   public:
    RpcClient(const std::string& server_address = "localhost:50051");
    ~RpcClient();

    void SetMonitorInfo(const monitor::proto::MonitorInfo& monitor_info);
    bool GetMonitorInfo(monitor::proto::QueryMessage& request,
                        monitor::proto::QueryResults& response);

    void LoginRegister(monitor::proto::UserMessage& request,
                       monitor::proto::UserResponseMessage& response);

   public:
// 每个service对应着一个指针
#if RPC_TYPE_DEFINE == MPRPC
    std::unique_ptr<monitor::proto::MonitorManager_Stub> stub_ptr_;
    std::unique_ptr<monitor::proto::UserManager_Stub> user_stub_ptr_;
#elif RPC_TYPE_DEFINE == GRPC
    std::unique_ptr<monitor::proto::MonitorManager::Stub> stub_ptr_;
    std::unique_ptr<monitor::proto::UserManager::Stub> user_stub_ptr_;
#endif
};

}  // namespace monitor