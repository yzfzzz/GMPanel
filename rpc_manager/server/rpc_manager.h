#pragma once
#include <yaml-cpp/yaml.h>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include "config.h"
#include "connection_pool.h"
#include "json.hpp"
#include "log.h"
#include "midinfo.h"

#if RPC_TYPE_DEFINE == MPRPC
#include "mprpcapplication.h"
#include "rpcprovider.h"
#elif RPC_TYPE_DEFINE == GRPC
#include <grpcpp/support/status.h>
#include "monitor_info.grpc.pb.h"
#endif

#include "monitor_info.pb.h"
#include "mysql_conn.h"
namespace monitor {
#if RPC_TYPE_DEFINE == MPRPC
class ServerManagerImpl : public monitor::proto::MonitorManager
#elif RPC_TYPE_DEFINE == GRPC
class ServerManagerImpl : public monitor::proto::MonitorManager::Service
#endif
{
   public:
    ServerManagerImpl();
    virtual ~ServerManagerImpl();

#if RPC_TYPE_DEFINE == MPRPC
    void SetMonitorInfo(::google::protobuf::RpcController* controller,
                        const ::monitor::proto::MonitorInfo* request,
                        ::google::protobuf::Empty* response,
                        ::google::protobuf::Closure* done);
    void GetMonitorInfo(::google::protobuf::RpcController* controller,
                        const ::monitor::proto::QueryMessage* request,
                        ::monitor::proto::QueryResults* response,
                        ::google::protobuf::Closure* done);
#elif RPC_TYPE_DEFINE == GRPC
    ::grpc::Status SetMonitorInfo(::grpc::ServerContext* context,
                                  const ::monitor::proto::MonitorInfo* request,
                                  ::google::protobuf::Empty* response);

    ::grpc::Status GetMonitorInfo(::grpc::ServerContext* context,
                                  const ::monitor::proto::QueryMessage* request,
                                  ::monitor::proto::QueryResults* response);
#endif

    // 插入一条数据
    bool insertOneInfo(monitor::proto::MonitorInfo& monitor_infos_);
    // 根据账号名查询用户id
    std::string selectUserId(std::string accountNum);
    bool isTableExist(std::string tableName,
                      std::shared_ptr<MysqlConn> conn_ptr);
    bool isMachineExist(std::string user_id, std::string machine_name);

    bool updateMachineStatus(std::string user_id, std::string machine_name);

    std::string formatDate(const std::string& datetime) {
        if (datetime.empty()) {
            return "";
        }
        return datetime.substr(0, 4) + datetime.substr(5, 2) +
               datetime.substr(8, 2);
    }

    MidInfo parseInfos(monitor::proto::MonitorInfo& monitor_infos_);

    template <typename T>
    std::string toJsonStr(std::vector<T>& v) {
        nlohmann::json json_data = v;
        return json_data.dump();
    }

    bool queryDataInfo(const ::monitor::proto::QueryMessage* request,
                       ::monitor::proto::QueryResults* response);

   private:
    YAML::Node sql_book;
    std::mutex create_mutex_;
    std::mutex set_mutex_;
    std::mutex get_mutex_;
    monitor::proto::MonitorInfo monitor_infos_;
    ConnectionPool* pool = ConnectionPool::getConnectPool();
};

#if RPC_TYPE_DEFINE == MPRPC
class UserManagerImpl : public monitor::proto::UserManager
#elif RPC_TYPE_DEFINE == GRPC
class UserManagerImpl : public monitor::proto::UserManager::Service
#endif
{
   public:
    UserManagerImpl();
    virtual ~UserManagerImpl();

#if RPC_TYPE_DEFINE == MPRPC
    void LoginRegister(::google::protobuf::RpcController* controller,
                       const ::monitor::proto::UserMessage* request,
                       ::monitor::proto::UserResponseMessage* response,
                       ::google::protobuf::Closure* done);
#elif RPC_TYPE_DEFINE == GRPC
    ::grpc::Status LoginRegister(
        ::grpc::ServerContext* context,
        const ::monitor::proto::UserMessage* request,
        ::monitor::proto::UserResponseMessage* response);

#endif

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