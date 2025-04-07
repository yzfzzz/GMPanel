#pragma once

#include <sys/epoll.h>
#include <functional>
#include <memory>
#include "Timestamp.h"
#include "noncopyable.h"

namespace mymuduo {
class EventLoop;
class Channel : noncopyable {
   public:
    // 定义一个 ”函数指针“
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop* loop, int fd);

    void handleEvent(Timestamp& receiveTime);

    void handleEventWithGuard(Timestamp& receiveTime);

    void set_revents(int revt) { revents_ = revt; }

    // 设置fd相应的事件状态
    void disableWriting() {
        events_ &= ~kWriteEvent;
        update(); 
    }

    // fd当前正在写, 用在 TcpConnection::handleWrite() 中
    bool isWriting const() { return events_ & kWriteEvent; }
    bool isNoneEvent() const { return events_ == kNoneEvent; }
    void set_index(int idx) { index_ = idx; }

   private:
    void update();

    // 事件循环
    EventLoop* loop_;
    // 管理的 fd 文件描述符
    const int fd_;
    // fd 感兴趣的事件
    int events_;
    // epoll（内核）返回的事件类型
    int revents_;
    int index_;

    bool tied_;

    // fd 的读写函数
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    // fd状态标志位
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
};
};  // namespace mymuduo