#pragma once
#include <string>
#include <vector>
#include "ConnectionPool.h"
#include "MysqlConn.h"
#include "client/rpc_client.h"
#include "midinfo.h"

namespace monitor {
class queryData {
   public:
    ConnectionPool* pool = ConnectionPool::getConnectPool();
    bool queryDataInfo(std::string account_num, int count,
                       monitor::RpcClient& rpc_client);


    std::vector<MidInfo> queryData_array;
};
}  // namespace monitor