#pragma once

#include <vector>
#include "Poller.h"
#include "Timestamp.h"

namespace mymuduo {

class Channel;

class EpollPoller : public Poller {
   public:
    EPollPoller(EventLoop* loop);
    ~EPollPoller() override;

    Timestamp poll(int timeoutMs, ChannelList* activeChannels);
    void fillActiveChannels(int& numEvents, ChannelList* activeChannels) const;
    void updateChannel(Channel* channel);
    void update(int operation, Channel* channel);
    

   private:
    static const int kInitEventListSize = 16;
    int epollfd_;
    using EventList = std::vector<epoll_event>;
    EventList events_;
};

};  // namespace mymuduo