#pragma once
#include <iostream>
#include <string>
#include "json.hpp"
namespace monitor {

class MidInfo {
   public:
    void print() {
        std::cout << "GPU Number: " << gpu_num << "\n"
                  << "GPU Name: " << gpu_name << "\n"
                  << "GPU Used Memory: " << gpu_used_mem << " MB\n"
                  << "GPU Total Memory: " << gpu_total_mem << " MB\n"
                  << "GPU Average Utilization: " << gpu_avg_util << "%\n"
                  << "CPU Load Average (1 min): " << cpu_load_avg_1 << "\n"
                  << "CPU Load Average (3 min): " << cpu_load_avg_3 << "\n"
                  << "CPU Load Average (15 min): " << cpu_load_avg_15 << "\n"
                  << "Memory Used: " << mem_used << " MB\n"
                  << "Memory Total: " << mem_total << " MB\n"
                  << "Network Send Rate: " << net_send_rate << " MB/s\n"
                  << "Network Receive Rate: " << net_rcv_rate << " MB/s\n"
                  << "Account Number: " << accountnum << "\n"
                  << "Date (Y-M-D): " << timeymd << "\n"
                  << "Time (H:M:S): " << timehms << "\n"
                  << "Machine Name: " << machine_name << "\n";
    }

    
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
    std::string machine_name = "unkown";
};
}  // namespace monitor