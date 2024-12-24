#include "mprpcapplication.h"
#include "rpc_client.h"

namespace monitor {
RpcClient::RpcClient(const std::string& server_address)
    : stub_ptr_(std::make_unique<monitor::proto::MonitorManager_Stub>(new MprpcChannel())) {}
RpcClient::~RpcClient() {}

void RpcClient::SetMonitorInfo(const monitor::proto::MonitorInfo& monitor_info) {
    // monitor::proto::MonitorInfo request;
    ::google::protobuf::Empty response;
    MprpcController controller;
    stub_ptr_->SetMonitorInfo(&controller, &monitor_info, &response, nullptr);
}

void RpcClient::GetMonitorInfo(monitor::proto::QueryMessage& request, monitor::proto::QueryResults& response) {
    MprpcController controller;
    // monitor::proto::MonitorInfo response;
    stub_ptr_->GetMonitorInfo(&controller, &request, &response, nullptr);
}
}  // namespace monitor
