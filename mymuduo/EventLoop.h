#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include "EPollPoller.h"
#include "noncopyable.h"
namespace mymuduo {

class Channel;
class EventLoop : noncopyable {
   public:
    using Functor = std::function<void()>;

    EventLoop();

    void loop();
    // 把cb放入队列中，唤醒loop所在的线程，执行cb
    void queueInLoop(Functor cb) void updateChannel(Channel *channel);
    // 判断EventLoop对象是否在自己的线程里面
    bool isInLoopThread() const { return threadId_ ==  CurrentThread::tid(); }
   private:
    std::unique_ptr<Poller> poller_;
    std::atomic_bool looping_;
    std::atomic_bool quit_;
    Timestamp pollReturnTime_;

    using ChannelList = std::vector<Channel *>;
    ChannelList activeChannels_;
    std::mutex mutex_;
    // 存储当前loop中是否有需要执行的回调操作
    std::vector<Functor> pendingFunctors_;
    int wakeupFd_;
};

};  // namespace mymuduo