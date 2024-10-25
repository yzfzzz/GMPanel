#include "mprpcapplication.h"
#include "rpc_client.h"

namespace monitor {
RpcClient::RpcClient(const std::string& server_address)
    : stub_ptr_(std::make_unique<monitor::proto::MonitorManager_Stub>(new MprpcChannel())) {}
RpcClient::~RpcClient() {}

void RpcClient::SetMonitorInfo(const monitor::proto::MonitorInfo& monito_info) {
    // monitor::proto::MonitorInfo request;
    ::google::protobuf::Empty response;
    MprpcController controller;
    stub_ptr_->SetMonitorInfo(&controller, &monito_info, &response, nullptr);
}

void RpcClient::GetMonitorInfo(monitor::proto::MonitorInfo* monito_info) {
    MprpcController controller;
    // monitor::proto::MonitorInfo response;
    ::google::protobuf::Empty request;
    stub_ptr_->GetMonitorInfo(&controller, &request, monito_info, nullptr);

    // һ��rpc�������, �����ý��
    if (!controller.Failed()) {
        if (monito_info->result().errcode() == 0) {
            // ������ȷ
            std::cout << "rpc GetFriendsList response success!" << std::endl;
        } else {
            // ����ʧ��
            std::cout << "rpc GetFriendsList response error:" << monito_info->result().errmsg()
                      << std::endl;
        }
    } else {
        std::cout << controller.ErrorText() << std::endl;
    }
}
}  // namespace monitor
