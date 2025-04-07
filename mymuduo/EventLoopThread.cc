#include "EventLoop.h"
#include "EventLoopThread.h"

namespace mymuduo {

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
                                 const std::string& name)
    : loop_(nullptr),
      exiting_(false),
      // std::bind生成一个对象，传给ThreadFunc，调用时相当于 this->threadFunc()
      // 类中的成员函数，若想像普通函数一样给其它类使用，需要传入this指针
      // 这个this指针是EventLoopThread和Thread的桥梁
      thread_(std::bind(&EventLoopThread::threadFunc, this), name),
      mutex_(),
      cond_(),
      // 执行Thread初始化成功的回调
      callback_(cb) {}

EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    if (loop_ != nullptr)
    {
        loop_->quit();
        thread_.join();
    }
}      

EventLoop* EventLoopThread::startLoop() {
    // 启动底层的新线程
    thread_.start();

    EventLoop* loop = nullptr;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (loop_ == nullptr) {
            cond_.wait(lock);
        }
        loop = loop_;
    }
    return loop;
}

// threadFunc -> Thread(ThreadFunc) -> Thread::start()运行的func_
// 线程运行的函数
void EventLoopThread::threadFunc() {
    // 此时也传入 EventLoopThread对象的指针（this）
    EventLoop loop;

    if (callback_) {
        callback_(&loop);
    }

    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = &loop;
        cond_.notify_one();
        // startLoop() 结束阻塞，表明 EventLoop 中 threadFunc 运行成功
    }

    loop.loop();
    std::unique_lock<std::mutex> lock(mutex_);
    loop_ = nullptr;
}

};  // namespace mymuduo