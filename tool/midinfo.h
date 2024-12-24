#pragma once
#include <string>
namespace monitor {

class MidInfo {
   public:
    int gpu_num = 0;
    std::string gpu_name = "";
    int gpu_used_mem = 0;
    int gpu_total_mem = 0;
    int gpu_avg_util = 0;
    float cpu_load_avg_1 = 0;
    float cpu_load_avg_3 = 0;
    float cpu_load_avg_15 = 0.0;
    float mem_used = 0.0;
    float mem_total = 0.0;
    float net_send_rate = 0.0;
    float net_rcv_rate = 0.0;
    std::string accountnum = "";
    std::string timeymd = "1970-1-1";
    std::string timehms = "00:00:00";
};
}  // namespace monitor