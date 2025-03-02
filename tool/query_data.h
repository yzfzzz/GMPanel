#pragma once
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
    bool queryDataInfo(std::string account_num, std::string machine_name,
                       int count, monitor::RpcClient& rpc_client);

    std::vector<MidInfo> query_result_array_;

    std::string toJsonStr() {
        nlohmann::json data;
        std::vector<float> gpu_util_array;
        std::vector<float> net_send_array;
        std::vector<float> net_recv_array;
        std::vector<std::string> time_array;
        for (auto q : query_result_array_) {
            gpu_util_array.push_back(q.gpu_avg_util);
            net_send_array.push_back(q.net_send_rate);
            net_recv_array.push_back(q.net_rcv_rate);
            time_array.push_back(q.timehms);
            data["userInfo"] = q.accountnum;
            data["gpuModel"] = q.gpu_name;
            // 8是cpu核心数，现在还未能获取
            data["cpuUsage"] = (int)(q.cpu_load_avg_3 * 100 / 8);
            data["memoryUsage"] = (int)q.mem_used;
            data["vramUsage"] = (int)(q.gpu_used_mem * 100 / q.gpu_total_mem);
            data["gpuUsage"] = (int)q.gpu_avg_util;
        }

        data["gpuChart"] = gpu_util_array;
        data["netUpload"] = net_send_array;
        data["netDownload"] = net_recv_array;
        data["time"] = time_array;
        data["userInfo"] = account_num;
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