#include <functional>
#include <string>
#include <vector>

#include <memory>

namespace mymuduo {

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable {
   public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;
    EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg);
    // ?ThreadInitCallback()是什么
    void start(const ThreadInitCallback& cb = ThreadInitCallback());

    void setThreadNumber(int numThreads) { numThreads = numThreads; }

    EventLoop* getNextLoop();
    std::vector<EventLoop*> getAllLoops();

    bool started() const { return started_; }
    const std::string name() const { return name_; }

   private:
    EventLoop* baseLoop_;
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
}

};  // namespace mymuduo