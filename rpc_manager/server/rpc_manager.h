#pragma once
#include <grpcpp/support/status.h>
#include <yaml-cpp/yaml.h>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include "connection_pool.h"
#include "json.hpp"
#include "log.h"
#include "midinfo.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
#include "mysql_conn.h"
namespace monitor {

class ServerManagerImpl : public monitor::proto::MonitorManager::Service {
   public:
    ServerManagerImpl();
    virtual ~ServerManagerImpl();

    ::grpc::Status SetMonitorInfo(::grpc::ServerContext* context,
                                  const ::monitor::proto::MonitorInfo* request,
                                  ::google::protobuf::Empty* response);

    ::grpc::Status GetMonitorInfo(::grpc::ServerContext* context,
                                  const ::monitor::proto::QueryMessage* request,
                                  ::monitor::proto::QueryResults* response);

    // 插入一条数据
    bool insertOneInfo(monitor::proto::MonitorInfo& monitor_infos_);
    // 根据账号名查询用户id
    std::string selectUserId(std::string accountNum);
    bool isTableExist(std::string tableName,
                      std::shared_ptr<MysqlConn> conn_ptr);
    bool isMachineExist(std::string user_id, std::string machine_name);

    MidInfo parseInfos(monitor::proto::MonitorInfo& monitor_infos_);

    template <typename T>
    std::string toJsonStr(std::vector<T>& v) {
        nlohmann::json json_data = v;
        return json_data.dump();
    }

    bool queryDataInfo(
        const ::monitor::proto::QueryMessage* request,
        ::monitor::proto::QueryResults* response);

   private:
    YAML::Node sql_book;
    std::mutex create_mutex_;
    std::mutex set_mutex_;
    std::mutex get_mutex_;
    monitor::proto::MonitorInfo monitor_infos_;
    ConnectionPool* pool = ConnectionPool::getConnectPool();
};

class UserManagerImpl : public monitor::proto::UserManager::Service {
   public:
    UserManagerImpl();
    virtual ~UserManagerImpl();

    ::grpc::Status LoginRegister(
        ::grpc::ServerContext* context,
        const ::monitor::proto::UserMessage* request,
        ::monitor::proto::UserResponseMessage* response);

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