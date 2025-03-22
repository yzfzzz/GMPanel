#pragma once
#include <arpa/inet.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <memory>
#include "loadbalancer.h"

class MprpcChannel : public google::protobuf::RpcChannel {
   public:
    // 所有通过stub代理对象调用的rpc方法, 都走到了这里,
    // 统一做rpc方法调用数据的数据序列化和网络发送
    MprpcChannel(int load_pattern = 0) {
        p_load_balancer = std::make_unique<LoadBalancer>(load_pattern);
    }
    // ~MprpcChannel() { delete load_balancer_; }
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller,
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response,
                    google::protobuf::Closure* done);

   private:
    std::unique_ptr<LoadBalancer> p_load_balancer;
};