#pragma once
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <string>
#include <vector>
#include "client/rpc_client.h"
#include "connection_pool.h"
#include "json.hpp"
#include "midinfo.h"
#include "mysql_conn.h"

namespace monitor {
class Service {
 public:
  Service() { LOG(INFO) << "Service Init";}
  ~Service() { LOG(INFO) << "Service Destory!!!"; }
  ConnectionPool* pool = ConnectionPool::getConnectPool();
  bool queryDataInfo(std::string account_num, std::string machine_name);
  std::string json_string;
  std::string account_num;
  std::string machine_name;
  monitor::RpcClient rpc_client;
  bool login(std::string account_num, std::string pwd);
  bool signup(std::string pwd);
  std::string response_str;
  std::vector<std::string> machine_name_array;
};

}  // namespace monitor