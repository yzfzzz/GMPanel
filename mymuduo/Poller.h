#pragma once

#include <sys/epoll.h>
#include <unordered_map>
#include <vector>
#include "Timestamp.h"
#include "noncopyable.h"

namespace mymuduo {
class Channel;
class EventLoop;
// epoll 的抽象类
class Poller : noncopyable {
   public:
    using ChannelList = std::vector<Channel*>;
    Poller(EventLoop* loop) : ownerLoop_(loop){};
    // 使用默认析构函数
    virtual ~Poller() = default;
    // 纯虚函数，给 epoll 作接口
    virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;
    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;

    //?为什么要用static，不用不行吗
    // -> 方便不依赖任何Poller对象
    static Poller* newDefaultPoller(EventLoop* loop);

   protected:
    // TODO: 优化
    using ChannelMap = std::unordered_map<int, Channel*>;
    ChannelMap channels;

   private:
    // 定义Poller所属的事件循环
    EventLoop* ownerLoop_;
};

};  // namespace mymuduo