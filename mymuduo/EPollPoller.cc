#include "EPollPoller.h"

namespace mymuduo {
// channel未添加到poller中
const int kNew = -1;  // channel的成员index_ = -1
// channel已添加到poller中
const int kAdded = 1;
// channel从poller中删除
const int kDeleted = 2;
EpollPoller::EPollPoller(EventLoop* loop)
    : Poller(loop),
      epollfd_(epoll_create(EPOLL_CLOEXEC)),
      events_(kInitEventListSize) {
    if (epollfd_ < 0) {
    }
}

EpollPoller::~EpollPoller() { close(epollfd_); }

Timestamp EpollPoller::poll(int timeoutMs, ChannelList* activeChannels) {
    int numEvents = epoll_wait(epollfd_, events_.data(),
                               static_cast<int>(events_.size()), timeoutMs);

    Timestamp now(Timestamp::now());
    if (numEvents > 0) {
        // 将活跃事件分配到各个Channel中
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size()) {
            events_.resize(events_.size() * 2);
        }
    } else if (numEvents == 0) {
    } else {
        if (errno != EINTR) {
        }
    }
    return now;
}

void EPollPoller::fillActiveChannels(int& numEvents,
                                     ChannelList* activeChannels) const {
    for (int i = 0; i < numEvents; i++) {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels.push_back(channel);
    }
}

void EPollPoller::updateChannel(Channel* channel) {
    const int index = channel->index();

    if (index == kNew || index == kDeleted) {
        if (index == kNew) {
            int fd = channel->fd();
            channels_[fd] = channel;
        }

        channel->set_index(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    // channel 在 poller上注册过了
    else {
        int fd = channel->fd();
        if (channel->isNoneEvent()) {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(kDeleted);
        } else {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void EPollPoller::update(int operation, Channel* channel) {
    epoll_event event;
    bzero(&event, sizeof(event));

    int fd = channel->fd();

    event.events = channel->events();
    event.data.fd = fd;
    event.data.ptr = channel;
    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
        if (operation == EPOLL_CTL_DEL) {
        } else {
        }
    }
}

};  // namespace mymuduo