#pragma once
#include <memory>
#include "login_register.pb.h"
#include "monitor_info.pb.h"
#include "mprpcapplication.h"

namespace monitor {

class RpcClient {
   public:
    RpcClient(const std::string& server_address = "localhost:50051");
    ~RpcClient();

    void SetMonitorInfo(const monitor::proto::MonitorInfo& monitor_info);
    void GetMonitorInfo(monitor::proto::QueryMessage& request,
                        monitor::proto::QueryResults& response);

    void LoginRegister(monitor::proto::UserMessage& request,
                       monitor::proto::UserResponseMessage& response);

   public:
    std::unique_ptr<monitor::proto::MonitorManager_Stub> stub_ptr_;
    std::unique_ptr<monitor::proto::UserManager_Stub> user_stub_ptr_;
};

}  // namespace monitor