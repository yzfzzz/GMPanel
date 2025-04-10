#pragma once

#include "noncopyable.h"
#include "Socket.h"
#include "Channel.h"

#include <functional>

namespace mymuduo{
class EventLoop;
class InetAddress;

class Acceptor: noncopyable
{
    public:
    using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;
    Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
    ~Acceptor();
    void setNewConnectionCallback(const NewConnectionCallback &cb) 
    {
        newConnectionCallback_ = cb;
    }

    bool listening() const{
        return listening;
    }

    void listen();

    private:
    void handleRead();

    EventLoop* loop_; // Acceptor用的是用户定义的baseLoop, 也称作mainLoop
    // 负责监听的 socket 套接字
    Socket acceptSocket_;
    // 监听的 socket 套接字 和 它对应的回调函数
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
}

};