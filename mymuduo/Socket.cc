#include <netinet/tcp.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "InetAddress.h"
#include "Socket.h"

namespace mymuduo {

Socket::~Socket() { close(sockfd_); }

void Socket::bindAddress(const InetAddress& localaddr) {
    if (bind(sockfd_, (sockaddr*)localaddr.getSockAddr(),
             sizeof(sockaddr_in))) {
    }
}

void Socket::listen() {
    // ?会不会是1024这个限制了10000个客户端并发
    // 1024 这个参数表示 已完成连接队列（accept queue）的最大长度
    if (0 != ::listen(sockfd_, 1024)) {
    }
}

int Socket::accept(InetAddress* peeraddr) {
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    bzero(&addr, sizeof(addr));

    int connfd =
        accept4(sockfd_, (sockaddr*)&addr, &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd >= 0) {
        // 将客户端的地址信息（sockaddr_in）存储到 peeraddr 对象中
        peeraddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::shutdownWrite() {
    if (::shutdown(sockfd_, SHUT_WR) < 0) {
    }
}

// 作用：控制是否启用 Nagle 算法（默认启用）。
void Socket::setTcpNoDelay(bool on) {
    // on=true（禁用 Nagle）：立即发送小数据包，降低延迟（适合交互式应用，如
    // SSH、游戏）。
    // on=false（启用 Nagle）：合并小数据包，提高吞吐量（适合文件传输）
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof optval);
}

// 作用：允许绑定处于 TIME_WAIT 状态的地址（端口）
void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
}

// ?允许多个套接字绑定到同一端口
// 需谨慎使用，避免端口冲突
void Socket::setReusePort(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof optval);
}

// 启用 TCP 保活探测，检测死连接
void Socket::setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof optval);
}

};  // namespace mymuduo