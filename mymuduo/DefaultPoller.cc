#include "Poller.h"
#include "EPollPoller.h"

#include <stdlib.h>

namespace mymuduo{

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    if(::getenv("MUDUO_USE_POLL"))
    {
        return nullptr;
    }
    return new EpollPoller(loop);
}
};
