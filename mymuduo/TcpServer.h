#include "Acceptor.h"
#include "Buffer.h"
#include "Callbacks.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "InetAddress.h"
#include "TcpConnection.h"
#include "noncopyable.h"

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace mymuduo {
class TcpServer {
   public:
    using ThreadInitCallback = std::function<void(EventLoop*)>;

    enum Option {
        kNoReusePort,
        kReusePort,
    };

    TcpServer(EventLoop* loop, const InetAddress& listenAddr,
              const std::string& nameArg, Option option = kNoReusePort);
    ~TcpServer();

    // 设置底层的subloop数
    void setThreadNum(int numThreads);

    // 开启服务器
    void start();

   private:
    void newConnection(int sockfd, const InetAddress& peerAddr);

    using ConnectionMap = std::unordered_mapM<std::string, TcpConnectionPtr>;

    EventLoop* loop_;  // baseloop, 用户定义的loop

    const std::string ipPort_;
    const std::string name_;

    // 负责将已完成连接的socket套接字分配sub Reactor
    std::unique_ptr<Acceptor> acceptor_;

    // 线程池
    std::shared_ptr<EventLoopThreadPool> threadPool_;

    ConnectionCallback connectionCallback_;  // 有新连接时的回调
    MessageCallback messageCallback_;        // 有读写消息时的回调
    WriteCompleteCallback writeCompleteCallback_;  // 消息发送完成以后的回调

    ThreadInitCallback threadInitCallback_;  // loop线程初始化的回调

    std::atomic_int started_;

    int nextConnId_;
    ConnectionMap connections_;  // 保存所有的连接
};
};  // namespace mymuduo