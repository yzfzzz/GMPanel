#pragma once
#include <cstdlib>
#include <ctime>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include "connection_pool.h"
#include "midinfo.h"
#include "monitor_info.pb.h"
#include "mprpcapplication.h"
#include "mysql_conn.h"
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
    // 插入一条数据
    bool insertOneInfo(monitor::proto::MonitorInfo& monitor_infos_);
    // 根据账号名查询用户id
    std::string selectUserId(std::string accountNum);
    bool isTableExist(std::string tableName,
                      std::shared_ptr<MysqlConn> conn_ptr);
    bool isMachineExist(std::string user_id, std::string machine_name);

    MidInfo parseInfos(monitor::proto::MonitorInfo& monitor_infos_);

    ::monitor::proto::QueryResults queryDataInfo(
        const ::monitor::proto::QueryMessage* request);

   private:
    std::mutex create_mutex_;
    std::mutex set_mutex_;
    std::mutex get_mutex_;
    monitor::proto::MonitorInfo monitor_infos_;
    ConnectionPool* pool = ConnectionPool::getConnectPool();

    std::string create_subsql_ =
        std::string("(gpu_num int DEFAULT NULL,") +
        "gpu_name varchar(100) DEFAULT NULL," +
        "gpu_used_mem int DEFAULT NULL," + "gpu_total_mem int DEFAULT NULL," +
        "gpu_avg_util int DEFAULT NULL," +
        "cpu_load_avg_1 float DEFAULT NULL," +
        "cpu_load_avg_3 float DEFAULT NULL," +
        "cpu_load_avg_15 float DEFAULT NULL," + "mem_used float DEFAULT NULL," +
        "mem_total float DEFAULT NULL," + "net_send_rate float DEFAULT NULL," +
        "net_rcv_rate float DEFAULT NULL," + "user_id int NOT NULL," +
        "time time NOT NULL," + "machine_name varchar(100) DEFAULT NULL" +
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=" +
        "utf8_general_ci COMMENT='create table according to date'";
};

class UserManagerImpl : public monitor::proto::UserManager {
   public:
    UserManagerImpl();
    virtual ~UserManagerImpl();

    void LoginRegister(::google::protobuf::RpcController* controller,
                       const ::monitor::proto::UserMessage* request,
                       ::monitor::proto::UserResponseMessage* response,
                       ::google::protobuf::Closure* done);

    std::string verifyLoginInformation();
    std::string registerNewUser();
    std::string generateRandomSixNumber();
    void queryUserMachineName(::monitor::proto::UserResponseMessage* response);

   private:
    std::string account_num_;
    std::string pwd_;
    ConnectionPool* pool = ConnectionPool::getConnectPool();
};

}  // namespace monitor