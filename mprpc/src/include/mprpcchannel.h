#pragma once
#include <arpa/inet.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <memory>
#include "loadbalancer.h"

class MprpcChannel : public google::protobuf::RpcChannel {
   public:
    // ����ͨ��stub���������õ�rpc����, ���ߵ�������,
    // ͳһ��rpc�����������ݵ��������л������緢��
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