#include "rpc_client.h"

namespace monitor {

RpcClient::RpcClient(const std::string& server_address) {
    stub_ptr_ = monitor::proto::MonitorManager::NewStub(grpc::CreateChannel(
        server_address, grpc::InsecureChannelCredentials()));
    user_stub_ptr_ = monitor::proto::UserManager::NewStub(grpc::CreateChannel(
        server_address, grpc::InsecureChannelCredentials()));
}
RpcClient::~RpcClient() {}

void RpcClient::SetMonitorInfo(
    const monitor::proto::MonitorInfo& monitor_info) {
    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    ::grpc::Status status =
        stub_ptr_->SetMonitorInfo(&context, monitor_info, &response);

    if (status.ok()) {
    } else {
        std::cout << status.error_details() << std::endl;
        std::cout << "status.error_message: " << status.error_message()
                  << std::endl;
        std::cout << "falied to connect !!!" << std::endl;
    }
}

bool RpcClient::GetMonitorInfo(monitor::proto::QueryMessage& request,
                               monitor::proto::QueryResults& response) {
    ::grpc::ClientContext context;
    ::grpc::Status status =
        stub_ptr_->GetMonitorInfo(&context, request, &response);
    if (status.ok()) {
        return true;
    } else {
        std::cout << status.error_details() << std::endl;
        std::cout << "status.error_message: " << status.error_message()
                  << std::endl;
        std::cout << "falied to connect !!!" << std::endl;
        return false;
    }
}

void RpcClient::LoginRegister(monitor::proto::UserMessage& request,
                              monitor::proto::UserResponseMessage& response) {
    ::grpc::ClientContext context;
    ::grpc::Status status =
        user_stub_ptr_->LoginRegister(&context, request, &response);
    if (status.ok()) {
    } else {
        std::cout << status.error_details() << std::endl;
        std::cout << "status.error_message: " << status.error_message()
                  << std::endl;
        std::cout << "falied to connect !!!" << std::endl;
    }
}
}  // namespace monitor
