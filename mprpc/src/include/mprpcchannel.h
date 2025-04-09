#pragma once
#include <arpa/inet.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <memory>
#include <unordered_map>
#include "loadbalancer.h"
#include "zookeeperutil.h"

class MprpcChannel : public google::protobuf::RpcChannel {
   public:
    // 所有通过stub代理对象调用的rpc方法, 都走到了这里,
    // 统一做rpc方法调用数据的数据序列化和网络发送
    MprpcChannel(int load_pattern = 0) : zkCli(), load_balancer(load_pattern) {
        zkCli.Start();
    }
    // ~MprpcChannel() { delete load_balancer_; }
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller,
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response,
                    google::protobuf::Closure* done);

   private:
    LoadBalancer load_balancer;
    ZkClient zkCli;
    std::unordered_map<std::string, std::vector<std::string>> method_host_map;
};