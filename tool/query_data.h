#pragma once
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <string>
#include <vector>
#include "client/rpc_client.h"
#include "connection_pool.h"
#include "json.hpp"
#include "midinfo.h"
#include "mysql_conn.h"

namespace monitor {
class QueryData {
   public:
    ConnectionPool* pool = ConnectionPool::getConnectPool();
    bool queryDataInfo(std::string account_num, std::string machine_name);

    MidInfo midinfo;

    template <typename T>
    std::vector<T> json2Vector(const std::string& json_str) {
        std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << __func__
                  << std::endl;
        std::vector<T> array;
        if (!json_str.empty()) {
            nlohmann::json json_data = nlohmann::json::parse(json_str);
            array = json_data.get<std::vector<T>>();
        }
        return array;
    }

    std::string toJsonStr() {
        nlohmann::json data;

        // 处理器
        data["cpu_percent"] = midinfo.cpu_percent;
        data["cpu_logic_num"] = midinfo.cpu_logic_num;
        // 一个浮点数组，柱型图
        data["cpu_each_core_uesd"] = midinfo.cpu_each_core_uesd;

        // 内存
        data["mem_total"] = midinfo.mem_total;
        data["mem_avail"] = midinfo.mem_avail;
        data["mem_used_percent"] =
            100 * (midinfo.mem_total - midinfo.mem_avail) / midinfo.mem_total;

        // 磁盘
        data["hard_disk_used"] = midinfo.hard_disk_used;
        data["hard_disk_total"] = midinfo.hard_disk_total;
        data["hard_disk_used_percent"] = midinfo.hard_disk_used_percent;

        // 操作系统
        data["os_name"] = midinfo.os_name;
        data["os_startup_time"] = midinfo.os_startup_time;

        // 网络数组
        data["net_rcv_rate_array"] = midinfo.net_rcv_rate_array;
        data["net_send_rate_array"] = midinfo.net_send_rate_array;

        // 时间数组
        data["timehms_array"] = midinfo.timehms_array;

        // 显卡数组
        data["gpu_name"] = midinfo.gpu_name;
        data["gpu_id"] = midinfo.gpu_id;
        data["gpu_mem_utilize"] = midinfo.gpu_mem_utilize;
        data["gpu_temperture"] = midinfo.gpu_temperture; 
        data["gpu_total_mem"] = midinfo.gpu_total_mem;
        data["gpu_used_mem"] = midinfo.gpu_used_mem;
        data["gpu_num"] = midinfo.gpu_num;

        return data.dump();
    }
    std::string account_num;
    std::string machine_name;
};

class UserManage {
   public:
    bool login(std::string account_num, std::string pwd) {
        monitor::RpcClient rpc_client;
        monitor::proto::UserMessage request;
        monitor::proto::UserResponseMessage response;
        request.set_account_num(account_num);
        request.set_pwd(pwd);
        rpc_client.LoginRegister(request, response);
        response_str = response.response_str();

        for (int i = 0; i < response.machine_name_array_size(); i++) {
            std::string machine_name = *response.mutable_machine_name_array(i);
            machine_name_array.push_back(machine_name);
            std::cout << "login machine name: " << machine_name << std::endl;
        }
        std::cout << response_str << std::endl;
        if (response_str == "login successful") {
            // 登录成功
            data["state"] = response_str;
            data["machine_name"] = machine_name_array;
            data["account_num"] = account_num;
            return true;
        } else {
            // 失败
            data["state"] = "error";
            return false;
        }
    }

    bool signup(std::string pwd) {
        std::string account_num;
        monitor::RpcClient rpc_client;
        monitor::proto::UserMessage request;
        monitor::proto::UserResponseMessage response;
        request.set_account_num(account_num);
        request.set_pwd(pwd);
        rpc_client.LoginRegister(request, response);
        response_str = response.response_str();
        std::cout << response_str << std::endl;
        if (!response_str.empty()) {
            // 注册成功
            data["state"] = response_str;
            return true;
        } else {
            // 失败
            data["state"] = "error";
            return false;
        }
    }

    nlohmann::json data;
    std::string response_str;
    std::vector<std::string> machine_name_array;
};

}  // namespace monitor