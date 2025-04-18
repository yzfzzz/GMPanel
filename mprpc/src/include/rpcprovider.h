#pragma once
#include <google/protobuf/descriptor.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/TcpServer.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "google/protobuf/service.h"
#include "tokenbucket.h"

// 框架提供的专门服务发布rpc服务的网络对象类
class RpcProvider {
   public:
    // 这里是框架给外部使用的, 可以发布rpc方法的函数接口
    // google::protobuf::Service ==> UserServiceRpc ==> UserService
    void NotifyService(google::protobuf::Service* service);

    // 启动rpc服务节点，开始远程调用
    void Run();

   private:
    // 组合EventLoop
    muduo::net::EventLoop m_eventLoop;
    TokenBucket m_tokenBucket;

    // service服务类型信息
    struct ServiceInfo {
        google::protobuf::Service* m_service;  // 保存服务对象
        std::unordered_map<std::string,
                           const google::protobuf::MethodDescriptor*>
            m_methodMap;  // 保存服务方法
    };
    // 存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;

    // 新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr& conn);

    // 发现缓冲区可读写, 则执行该回调函数
    void OnMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buffer, muduo::Timestamp);

    // Closure的回调操作, 用于序列化rpc的响应和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr& conn,
                         google::protobuf::Message*);
};