#pragma once
#include <memory>
#include "monitor_info.pb.h"
#include "mprpcapplication.h"

namespace monitor {

class RpcClient {
   public:
    RpcClient(const std::string& server_address = "localhost:50051");
    ~RpcClient();

    void SetMonitorInfo(const monitor::proto::MonitorInfo& monitor_info);
    void GetMonitorInfo(monitor::proto::QueryMessage& request, monitor::proto::QueryResults& response);

   public:
    std::unique_ptr<monitor::proto::MonitorManager_Stub> stub_ptr_;
};

}  // namespace monitor