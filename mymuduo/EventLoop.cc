#include <errno.h>
#include <fcntl.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <memory>
#include "Channel.h"
#include "EventLoop.h"
#include "Poller.h"

namespace mymuduo {
// 定义默认的Poller IO复用接口的超时时间
const int kPollTimeMs = 10000;

void EventLoop::EventLoop()
    : looping_(false), quit_(false), poller_(Poller::newDefaultPoller(this)), {}

void EventLoop::loop() {
    looping_ = true;
    quit_ = false;

    while (!quit_) {
        activeChannels_.clear();
        // 启动事件分发器
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        // 有事件发生
        for (Channel* channel : activeChannels_) {
            // 各个 channel 执行相应的回调
            channel->handleEvent(pollReturnTime_);
        }

        // 执行回调操作
    }

    looping_ = false;
}

void EventLoop::updateChannel(Channel* channel) {
    poller_->updateChannel(channel);
}

void EventLoop::queueInLoop(Functor cb) {
    {
        std::unqiue_lock<std::mutex> lock(mutex_);
        pendingFunctors_.emplace_back(cb);
    }

    // 唤醒相应的，需要执行上面回调操作的loop的线程了
    // ||
    // callingPendingFunctors_的意思是：当前loop正在执行回调，但是loop又有了新的回调
    if (!isInLoopThread() || callingPendingFunctors_) {
        wakeup();  // 唤醒loop所在线程
    }
}

};  // namespace mymuduo