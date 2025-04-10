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
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
    void doPendingFunctors();

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
    // 标识当前loop是否有需要执行的回调操作
    std::atomic_bool callingPendingFunctors_;
    // 主要作用，当mainLoop获取一个新用户的channel，通过轮询算法选择一个subloop，通过该成员唤醒subloop处理channel
    // 应该是 subReactor的epollfd
    int wakeupFd_;
};

};  // namespace mymuduo