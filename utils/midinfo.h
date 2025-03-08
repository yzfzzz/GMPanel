#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "json.hpp"
namespace monitor {

class MidInfo {
   public:
    void printInfo() {
        // 打印GPU信息
        std::cout << "GPU Number: " << gpu_num << std::endl;
        for (int i = 0; i < gpu_num; ++i) {
            std::cout << "GPU ID: " << gpu_id[i] << std::endl;
            std::cout << "GPU Name: " << gpu_name[i] << std::endl;
            std::cout << "GPU Used Memory: " << gpu_used_mem[i] << " MB"
                      << std::endl;
            std::cout << "GPU Total Memory: " << gpu_total_mem[i] << " MB"
                      << std::endl;
            std::cout << "GPU Average Utilization: " << gpu_mem_utilize[i]
                      << " %" << std::endl;
            std::cout << "GPU Temperature: " << gpu_temperture[i] << " °C"
                      << std::endl;
        }

        // 打印CPU状态
        std::cout << "CPU Percent: " << cpu_percent << " %" << std::endl;
        std::cout << "CPU Logic Number: " << cpu_logic_num << std::endl;
        for (size_t i = 0; i < cpu_each_core_uesd.size(); ++i) {
            std::cout << "Core " << i
                      << " Utilization Rate: " << cpu_each_core_uesd[i] << " %"
                      << std::endl;
        }

        // 打印内存信息
        std::cout << "Memory Used Percent: " << mem_avail << " GB" << std::endl;
        std::cout << "Memory Total: " << mem_total << " GB" << std::endl;

        // 打印网络信息
        std::cout << "Network Send Rate: " << net_send_rate << " MB/s"
                  << std::endl;
        std::cout << "Network Receive Rate: " << net_rcv_rate << " MB/s"
                  << std::endl;

        // 打印硬盘信息
        std::cout << "Hard Disk Total: " << hard_disk_total << " GB"
                  << std::endl;
        std::cout << "Hard Disk Used: " << hard_disk_used << " GB" << std::endl;
        std::cout << "Hard Disk Used Percent: " << hard_disk_used_percent
                  << " %" << std::endl;

        // 打印操作系统信息
        std::cout << "OS Name: " << os_name << std::endl;
        std::cout << "OS Startup Time: " << os_startup_time << std::endl;

        // 打印用户信息
        std::cout << "Account Number: " << accountnum << std::endl;
        std::cout << "Machine Name: " << machine_name << std::endl;

        // 打印时间信息
        std::cout << "Date: " << timeymd << std::endl;
        std::cout << "Time: " << timehms << std::endl;
    }

    // GPU 信息
    int gpu_num;
    std::vector<std::string> gpu_id;
    std::vector<std::string> gpu_name;
    std::vector<int> gpu_used_mem;
    std::vector<int> gpu_total_mem;
    std::vector<int> gpu_mem_utilize;
    std::vector<int> gpu_temperture;
    // CPULoad 信息
    float cpu_load_avg_1;
    float cpu_load_avg_3;
    float cpu_load_avg_15;

    // CPU状态
    float cpu_percent;
    int cpu_logic_num;
    int cpu_physical_core_num;
    float cpu_used_percent;
    std::vector<float> cpu_each_core_uesd;

    // 内存信息
    float mem_avail;
    float mem_total;

    // 网络信息
    float net_send_rate;
    float net_rcv_rate;

    // 硬盘信息
    float hard_disk_total;
    float hard_disk_used;
    float hard_disk_used_percent;

    // 操作系统信息
    std::string os_name;
    std::string os_startup_time;

    // 用户信息
    std::string accountnum;
    std::string machine_name = "unkown";

    // 时间信息
    std::string timeymd = "1970-1-1";
    std::string timehms = "00:00:00";

    // 网络时序信息
    std::vector<float> net_send_rate_array;
    std::vector<float> net_rcv_rate_array;
    // 时间序列
    std::vector<std::string> timehms_array;
};
}  // namespace monitor