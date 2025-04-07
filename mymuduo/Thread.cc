#include <semaphore.h>
#include "CurrentThread.h"
#include "Thread.h"

namespace mymuduo {
std::atomic_int Thread::numCreated_(0);

// 传入一个函数指针
Thread::Thread(ThreadFunc func, const std::string &name)
    :  // std::move后，原来的函数指针是否有处理?
      started_(false),
      joined_(false),
      tid_(0),
      func_(std::move(func)),
      name(name) {
        setDefaultName();
      }

Thread::~Thread(){
    // 线程启动了，且用户并未设置 thread_->join()
    if(started_ && !joined_)
    {
        thread_->detach();
    }
}

void Thread::start(){
    started_ = true;
    sem_t sem;
    sem_init(&sem,false, 0);

    // 新建一个线程，并在这个线程中运行设置的函数
    thread_ = std::shared_ptr<std::thread>(new std::thread([&](){
        tid_ = CurrentThread::tid();
        // 结束主线程的sem_wait()
        sem_post(&sem);
        func_();
    }));
    // 此时，这个Thread类管理的是这个新建的子线程，而不是父线程

    // 等待真正使用的线程初始化完成
    sem_wait(&sem);
}

void Thread::join(){
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName(){
    int num = numCreated_++;
    if(name_.empty()){
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}

};  // namespace mymuduo