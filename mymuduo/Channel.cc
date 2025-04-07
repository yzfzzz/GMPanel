#include "Channel.h"

namespace mymuduo {

// 表示没有任何事件需要监听。在epoll中，值为0表示不关注任何事件
const int Channel::kNoneEvent = 0;
// 表示关注可读事件和紧急数据事件
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
// 表示关注可写事件，即文件描述符可写入数据
const int Channel::kWriteEvent = EPOLLOUT;

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
    if (revents_ & EPOLLERR) {
        if (errorCallback_) {
            errorCallback_();
        }
    }
    if (revents_ & (EPOLLIN | EPOLLPRI)) {
        // 返回事件表示: 缓冲区可读, 或有紧急事件
        if (readCallback_) {
            readCallback_(receiveTime);
        }
    }

    if (revents_ & EPOLLOUT) {
        // 缓冲区可写
        if (writeCallback_) {
            writeCallback_();
        }
    }
}
/**
 * 当改变channel所表示fd的events事件后，update负责在poller里面更改fd相应的事件epoll_ctl
 * EventLoop => ChannelList   Poller
 */
void Channel::update() {
    // 通过channel所属的EventLoop，调用poller的相应方法，注册fd的events事件
    loop_->updateChannel(this);
}
};  // namespace mymuduo