#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

#include "ConnectionPool.h"
#include "MysqlConn.h"
#include "midinfo.h"
#include "monitor_info.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
namespace monitor {

class ServerManagerImpl : public monitor::proto::MonitorManager {
   public:
    ServerManagerImpl();
    virtual ~ServerManagerImpl();

    void SetMonitorInfo(::google::protobuf::RpcController* controller,
                        const ::monitor::proto::MonitorInfo* request,
                        ::google::protobuf::Empty* response,
                        ::google::protobuf::Closure* done);

    void GetMonitorInfo(::google::protobuf::RpcController* controller,
                        const ::monitor::proto::QueryMessage* request,
                        ::monitor::proto::QueryResults* response,
                        ::google::protobuf::Closure* done);

    bool InsertOneInfo(monitor::proto::MonitorInfo& monitor_infos_);
    std::string SelectUserId(std::string accountNum);
    bool isTableExist(std::string tableName,
                      std::shared_ptr<MysqlConn> conn_ptr);

    MidInfo parseInfos(monitor::proto::MonitorInfo& monitor_infos_);

    ::monitor::proto::QueryResults queryDataInfo(
        const ::monitor::proto::QueryMessage* request);

   private:
    std::mutex m_create_mutex;
    monitor::proto::MonitorInfo monitor_infos_;
    ConnectionPool* pool = ConnectionPool::getConnectPool();

    std::string create_subsql =
        std::string("(gpu_num int DEFAULT NULL,") +
        "gpu_name varchar(100) DEFAULT NULL," +
        "gpu_used_mem int DEFAULT NULL," + "gpu_total_mem int DEFAULT NULL," +
        "gpu_avg_util int DEFAULT NULL," +
        "cpu_load_avg_1 float DEFAULT NULL," +
        "cpu_load_avg_3 float DEFAULT NULL," +
        "cpu_load_avg_15 float DEFAULT NULL," + "mem_used float DEFAULT NULL," +
        "mem_total float DEFAULT NULL," + "net_send_rate float DEFAULT NULL," +
        "net_rcv_rate float DEFAULT NULL," + "user_id int NOT NULL," +
        "time time NOT NULL" +
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=" +
        "utf8mb4_0900_ai_ci COMMENT='create table according to date'";
};
}  // namespace monitor