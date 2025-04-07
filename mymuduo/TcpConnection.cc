#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "TcpConnection.h"

#include <errno.h>
#include <netinet/tcp.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <functional>
#include <string>

namespace mymuduo {
static EventLoop* CheckLoopNotNull(EventLoop* loop) {
    if (loop == nullptr) {
    }
    return loop;
}

TcpConnection::TcpConnection(EventLoop* loop, const std::string& nameArg,
                             int sockfd, const InetAddress& localAddr,
                             const InetAddress& peerAddr)
    : loop_(CheckLoopNotNull(loop)),
      name_(nameArg),
      state_(kConnecting),
      reading_(true),
      socket_(new Socket(sockfd)),
      channel_(new Channel(loop, sockfd)),
      localAddr_(localAddr),
      peerAddr_(peerAddr),
      highWaterMark_(64 * 1024 * 1024)  // ?这个变量是干嘛的
{
    // 下面给channel设置相应的回调函数，poller给channel通知感兴趣的事件发生了，channel会回调相应的操作函数
    channel_->setReadCallback(
        std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection() {}

void TcpConnection::handleRead(Timestamp receiveTime) {
    int saveErrno = 0;
    // 从缓冲区读取数据
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &saveErrno);
    if (n > 0) {
        // 对于已经连接的用户, 当发送数据到服务器, 执行回调操作 onMessage
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    } else if (n == 0) {
        // 没有读取到数据
        handleClose();
    } else {
        errno = saveErrno;
        handleError();
    }
}

void TcpConnection::handleWrite() {
    // 如果fd可写
    if (channel_->isWriting()) {
        int savedErrno = 0;
        // 正式发送数据
        ssize_t n = outputBuffer_.writeFd(channel_->fd(), &savedErrno);
        if (n > 0) {
            // 表示已经发送n个字节
            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableBytes() == 0) {
                // channel_->disableWriting
                // -> Channel::update
                // -> loop.update
                // -> EPollPoller.updateChannel
                // -> EPollPoller.update
                // -> epoll_ctl
                channel_->disableWriting();
                if(writeCompleteCallback_)
                {
                    // 唤醒loop对应的thread线程, 执行回调
                    loop_->queue
                }
            }
        }
    }
}

};  // namespace mymuduo