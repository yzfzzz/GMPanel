#pragma once

#include <string>
#include <unordered_map>

#include <boost/chrono.hpp>

#include "monitor_info.pb.h"
#include "monitor_info.pb.h"
#include "monitor_inter.h"

namespace monitor {
class CpuSoftIrqMonitor : public MonitorInter {
    struct SoftIrq {
        std::string cpu_name;
        int64_t hi;
        int64_t timer;
        int64_t net_tx;
        int64_t net_rx;
        int64_t block;
        int64_t irq_poll;
        int64_t tasklet;
        int64_t sched;
        int64_t hrtimer;
        int64_t rcu;
        boost::chrono::steady_clock::time_point timepoint;
    };

   public:
    CpuSoftIrqMonitor() {}
    void updateOnce(monitor::proto::MonitorInfo *monitor_info);
    void stop() override {}

   private:
    std::unordered_map<std::string, struct SoftIrq> cpu_softirqs_;
};

}  // namespace monitor
