#include "rpc_client.h"
#if RPC_TYPE_DEFINE == MPRPC
#include "mprpcapplication.h"
#endif
namespace monitor {
RpcClient::RpcClient(const std::string& server_address) {
#if RPC_TYPE_DEFINE == MPRPC
    stub_ptr_ = std::make_unique<monitor::proto::MonitorManager_Stub>(
        new MprpcChannel());
    user_stub_ptr_ =
        std::make_unique<monitor::proto::UserManager_Stub>(new MprpcChannel());

#elif RPC_TYPE_DEFINE == GRPC
    stub_ptr_ = monitor::proto::MonitorManager::NewStub(grpc::CreateChannel(
        server_address, grpc::InsecureChannelCredentials()));
    user_stub_ptr_ = monitor::proto::UserManager::NewStub(grpc::CreateChannel(
        server_address, grpc::InsecureChannelCredentials()));
#endif
}
RpcClient::~RpcClient() {}

void RpcClient::SetMonitorInfo(
    const monitor::proto::MonitorInfo& monitor_info) {
#if RPC_TYPE_DEFINE == MPRPC
    ::google::protobuf::Empty response;
    MprpcController controller;
    stub_ptr_->SetMonitorInfo(&controller, &monitor_info, &response, nullptr);

#elif RPC_TYPE_DEFINE == GRPC
    ::grpc::ClientContext context;
    ::google::protobuf::Empty response;
    ::grpc::Status status =
        stub_ptr_->SetMonitorInfo(&context, monitor_info, &response);

    if (status.ok()) {
    } else {
        LOG(ERROR) << fmt::format(
            "failed to connect !!!status.error_message: {}, "
            "status.error_details: {}",
            status.error_message(), status.error_details());
    }
#endif
}

bool RpcClient::GetMonitorInfo(monitor::proto::QueryMessage& request,
                               monitor::proto::QueryResults& response) {
#if RPC_TYPE_DEFINE == MPRPC
    MprpcController controller;
    // monitor::proto::MonitorInfo response;
    stub_ptr_->GetMonitorInfo(&controller, &request, &response, nullptr);
#elif RPC_TYPE_DEFINE == GRPC
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
#endif
}

void RpcClient::LoginRegister(monitor::proto::UserMessage& request,
                              monitor::proto::UserResponseMessage& response) {
#if RPC_TYPE_DEFINE == MPRPC
    MprpcController controller;
    // monitor::proto::MonitorInfo response;
    user_stub_ptr_->LoginRegister(&controller, &request, &response, nullptr);
#elif RPC_TYPE_DEFINE == GRPC
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
#endif
}
}  // namespace monitor
