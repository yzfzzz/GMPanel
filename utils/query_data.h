#pragma once
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <string>
#include <vector>
#include "client/rpc_client.h"
#include "connection_pool.h"
#include "json.hpp"
#include "log.h"
#include "midinfo.h"
#include "mysql_conn.h"

namespace monitor {
class QueryData {
   public:
    ConnectionPool* pool = ConnectionPool::getConnectPool();
    bool queryDataInfo(std::string account_num, std::string machine_name);
    std::string json_string;
    std::string account_num;
    std::string machine_name;
    monitor::RpcClient rpc_client;
};

class UserManage {
   public:
    bool login(std::string account_num, std::string pwd);
    bool signup(std::string pwd);

    std::string json_string;
    std::string response_str;
    std::vector<std::string> machine_name_array;
    monitor::RpcClient rpc_client;
};

}  // namespace monitor