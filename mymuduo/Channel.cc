#include "Channel.h"

namespace mymuduo {

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop), fd(fd_), events_(0), revents_(0), index_(-1), tied_(false) {}

// handleEvent
//  -> handleEventWithGuard
void Channel::handleEvent(Timestamp& receiveTime) {
    handleEventWithGuard(receiveTime);
}

void Channel::handleEventWithGuard(Timestamp& receiveTime) {
    // epoll_waut(..., revents, ...)
    // event 是一个二进制掩码
    // EPOLLIN：0x001（二进制 0000 0001）
    // EPOLLOUT：0x004（二进制 0000 0100）
    // EPOLLERR：0x008（二进制 0000 1000）
    if ((revents_ & EPOLLHUP) && (revents_ & EPOLLIN)) {
        // 如果客户端关闭 + 缓冲不可读 + 有close回调
        if (closeCallback_) {
            // 执行close回调
            closeCallback_();
        }
    }
    if(revents_ & EPOLLERR){
        if(errorCallback_)
        {
            errorCallback_();
        }
    }
    if(revents_ & (EPOLLIN | EPOLLPRI))
    {
        // 返回事件表示: 缓冲区可读, 或有紧急事件
        if(readCallback_)
        {
            readCallback_(receiveTime);
        }
    }

    if(revents_ & EPOLLOUT)
    {
        // 缓冲区可写
        if(writeCallback_)
        {
            writeCallback_();
        }
    }
}

};  // namespace mymuduo