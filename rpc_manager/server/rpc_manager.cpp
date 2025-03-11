#include "rpc_manager.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include "get_time.h"
namespace monitor {

enum SystemMetrics {
    USER_ID = 0,
    MACHINE_NAME = 1,
    TIMEHMS = 2,
    HARD_DISK_TOTAL = 3,
    HARD_DISK_USED = 4,
    HARD_DISK_USED_PERCENT = 5,
    CPU_PERCENT = 6,
    CPU_LOGIC_NUM = 7,
    CPU_EACH_CORE_USED = 8,
    MEM_TOTAL = 9,
    MEM_AVAIL = 10,
    NET_SEND_RATE = 11,
    NET_RCV_RATE = 12,
    OS_NAME = 13,
    OS_STARTUP_TIME = 14,
    GPU_ID = 15,
    GPU_NAME = 16,
    GPU_TOTAL_MEM = 17,
    GPU_USED_MEM = 18,
    GPU_TEMPERTURE = 19,
    GPU_MEM_UTILIZE = 20
};

ServerManagerImpl::ServerManagerImpl() {
    this->sql_book = YAML::LoadFile("sql.yaml");
}
ServerManagerImpl::~ServerManagerImpl() {}
UserManagerImpl ::UserManagerImpl() {}
UserManagerImpl ::~UserManagerImpl() {}

#if RPC_TYPE_DEFINE == MPRPC
void ServerManagerImpl::SetMonitorInfo(
    ::google::protobuf::RpcController* controller,
    const ::monitor::proto::MonitorInfo* request,
    ::google::protobuf::Empty* response, ::google::protobuf::Closure* done) {
    std::lock_guard<std::mutex> lock(set_mutex_);
    LOG(INFO) << "RPC Call: ServerManagerImpl::SetMonitorInfo";
    monitor_infos_.Clear();
    monitor_infos_ = *request;
    insertOneInfo(monitor_infos_);
    done->Run();
}
#elif RPC_TYPE_DEFINE == GRPC
::grpc::Status ServerManagerImpl::SetMonitorInfo(
    ::grpc::ServerContext* context,
    const ::monitor::proto::MonitorInfo* request,
    ::google::protobuf::Empty* response) {
    std::lock_guard<std::mutex> lock(set_mutex_);
    monitor_infos_.Clear();
    monitor_infos_ = *request;
    insertOneInfo(monitor_infos_);
    return grpc::Status::OK;
}

#endif
#if RPC_TYPE_DEFINE == MPRPC
void ServerManagerImpl::GetMonitorInfo(
    ::google::protobuf::RpcController* controller,
    const ::monitor::proto::QueryMessage* request,
    ::monitor::proto::QueryResults* response,
    ::google::protobuf::Closure* done) {
    std::lock_guard<std::mutex> lock(get_mutex_);
    // LOG(INFO) << "RPC Call: ServerManagerImpl::GetMonitorInfo";
    queryDataInfo(request, response);
    done->Run();
}
#elif RPC_TYPE_DEFINE == GRPC
::grpc::Status ServerManagerImpl::GetMonitorInfo(
    ::grpc::ServerContext* context,
    const ::monitor::proto::QueryMessage* request,
    ::monitor::proto::QueryResults* response) {
    std::lock_guard<std::mutex> lock(get_mutex_);

    if (queryDataInfo(request, response)) {
        return ::grpc::Status::OK;
    } else {
        return ::grpc::Status(::grpc::StatusCode::UNKNOWN, "Error message");
    }
}

#endif

bool ServerManagerImpl::queryDataInfo(
    const ::monitor::proto::QueryMessage* request,
    ::monitor::proto::QueryResults* response) {
    // std::string sql = request->sql();
    std::string table_name = "table_" + request->timeymd();
    std::string machine_name = request->machine_name();
    std::string accountnum = request->accountnum();

    std::string sql =
        fmt::format(sql_book["select_all_metrics_sql"].as<std::string>(),
                    table_name, machine_name, accountnum);
    // LOG(INFO) << "sql: " << sql;
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            response->set_machine_name(conn_ptr->value(MACHINE_NAME));

            response->set_hard_disk_total(
                std::stof(conn_ptr->value(HARD_DISK_TOTAL)));

            response->set_hard_disk_used(
                std::stof(conn_ptr->value(HARD_DISK_USED)));

            response->set_hard_disk_used_percent(
                std::stof(conn_ptr->value(HARD_DISK_USED_PERCENT)));

            response->set_cpu_percent(std::stof(conn_ptr->value(CPU_PERCENT)));

            response->set_cpu_logic_num(
                std::stoi(conn_ptr->value(CPU_LOGIC_NUM)));

            response->set_cpu_each_core_uesd(
                conn_ptr->value(CPU_EACH_CORE_USED));

            response->set_mem_total(std::stof(conn_ptr->value(MEM_TOTAL)));
            response->set_mem_avail(std::stof(conn_ptr->value(MEM_AVAIL)));
            response->set_os_name(conn_ptr->value(OS_NAME));
            response->set_os_startup_time(conn_ptr->value(OS_STARTUP_TIME));
            response->set_gpu_id(conn_ptr->value(GPU_ID));
            response->set_gpu_name(conn_ptr->value(GPU_NAME));
            response->set_gpu_total_mem(conn_ptr->value(GPU_TOTAL_MEM));
            response->set_gpu_used_mem(conn_ptr->value(GPU_USED_MEM));
            response->set_gpu_temperture(conn_ptr->value(GPU_TEMPERTURE));
            response->set_gpu_mem_utilize(conn_ptr->value(GPU_MEM_UTILIZE));
        }
    } else {
        return false;
    }

    // 处理repeated数据
    sql = fmt::format(sql_book["search_net_last_10_data_sql"].as<std::string>(),
                      table_name, machine_name, accountnum);

    LOG(INFO) << "[" << __FILE__ << ":" << __LINE__ << "] " << __func__;
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            response->add_net_send_rate(std::stof(conn_ptr->value(0)));
            response->add_net_rcv_rate(std::stof(conn_ptr->value(1)));
            response->add_timehms(conn_ptr->value(2));
        }
    } else {
        return false;
    }

    return true;
}

MidInfo ServerManagerImpl::parseInfos(
    monitor::proto::MonitorInfo& monitor_infos_) {
    LOG(INFO) << "Parse monitor_infos_...";
    MidInfo mid_info;
    // 用户信息
    mid_info.accountnum = monitor_infos_.accountnum();
    mid_info.machine_name = monitor_infos_.machine_name();

    // 时间
    mid_info.timeymd = monitor_infos_.time().timeymd();
    mid_info.timehms = monitor_infos_.time().timehms();

    // 磁盘
    mid_info.hard_disk_total = monitor_infos_.hard_disk().total();
    mid_info.hard_disk_used = monitor_infos_.hard_disk().used();
    mid_info.hard_disk_used_percent = monitor_infos_.hard_disk().used_percent();

    // 处理器
    mid_info.cpu_percent = monitor_infos_.cpu_stat().cpu_percent();
    mid_info.cpu_logic_num = monitor_infos_.cpu_stat().cpu_logic_num();
    for (int i = 0; i < monitor_infos_.cpu_stat().cpu_each_core_uesd_size();
         i++) {
        mid_info.cpu_each_core_uesd.push_back(
            monitor_infos_.cpu_stat().cpu_each_core_uesd(i));
    }

    // 内存
    mid_info.mem_avail = monitor_infos_.mem_info().avail();
    mid_info.mem_total = monitor_infos_.mem_info().total();

    // 网络
    mid_info.net_send_rate = monitor_infos_.net_info().send_rate();
    mid_info.net_rcv_rate = monitor_infos_.net_info().rcv_rate();

    // 操作系统
    mid_info.os_name = monitor_infos_.os().os_name();
    mid_info.os_startup_time = monitor_infos_.os().os_startup_time();

    // 显卡
    mid_info.gpu_num = monitor_infos_.gpu_info_size();
    for (int i = 0; i < monitor_infos_.gpu_info_size(); i++) {
        mid_info.gpu_id.push_back(monitor_infos_.gpu_info(i).id());
        mid_info.gpu_name.push_back(monitor_infos_.gpu_info(i).gpu_name());
        mid_info.gpu_mem_utilize.push_back(
            monitor_infos_.gpu_info(i).gpu_mem_utilize());
        mid_info.gpu_temperture.push_back(
            monitor_infos_.gpu_info(i).temperture());
        mid_info.gpu_total_mem.push_back(
            monitor_infos_.gpu_info(i).gpu_mem_total());
        mid_info.gpu_used_mem.push_back(
            monitor_infos_.gpu_info(i).gpu_mem_used());
    }
    mid_info.printInfo();
    return mid_info;
}

bool ServerManagerImpl::insertOneInfo(
    monitor::proto::MonitorInfo& monitor_infos_) {
    LOG(INFO) << "Run: ServerManagerImpl::insertOneInfo";
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    MidInfo mid_info = parseInfos(monitor_infos_);
    std::string table_name = "table_" + mid_info.timeymd;
    if (!isTableExist(table_name, conn_ptr)) {
        LOG(WARNING) << "Failed to select table!";
        return false;
    }

    std::string user_id = selectUserId(mid_info.accountnum);
    isMachineExist(user_id, mid_info.machine_name);
    if (user_id.empty() == true) {
        return false;
    }

    std::string sql = fmt::format(
        sql_book["insert_sql"].as<std::string>(), table_name, user_id,
        mid_info.machine_name, mid_info.timehms, mid_info.hard_disk_total,
        mid_info.hard_disk_used, mid_info.hard_disk_used_percent,
        mid_info.cpu_percent, mid_info.cpu_logic_num,
        toJsonStr(mid_info.cpu_each_core_uesd), mid_info.mem_total,
        mid_info.mem_avail, mid_info.net_send_rate, mid_info.net_rcv_rate,
        mid_info.os_name, mid_info.os_startup_time, toJsonStr(mid_info.gpu_id),
        toJsonStr(mid_info.gpu_name), toJsonStr(mid_info.gpu_total_mem),
        toJsonStr(mid_info.gpu_used_mem), toJsonStr(mid_info.gpu_temperture),
        toJsonStr(mid_info.gpu_mem_utilize));

    LOG(INFO) << "InsertOneInfo SQL: " << sql;
    if (conn_ptr->update(sql)) {
        LOG(INFO) << "Succeed to insert one sql";
        return true;
    }
    LOG(WARNING) << "Failed to insert one sql";
    return false;
}

std::string ServerManagerImpl::selectUserId(std::string accountNum) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql = fmt::format(
        "SELECT id FROM `user` u WHERE accountnum ='{}'", accountNum);
    LOG(INFO) << "selectUserId select SQL: " << sql;
    std::string user_id;
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            user_id = conn_ptr->value(0);
        }
    }
    return user_id;
}

bool ServerManagerImpl::isMachineExist(std::string user_id,
                                       std::string machine_name) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql = fmt::format(
        "SELECT COUNT(*) FROM `machine` m WHERE user_id={} and "
        "machine_name='{}'",
        user_id, machine_name);
    LOG(INFO) << "isMachineExist select SQL: " << sql;
    int machine_count = 0;
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            machine_count = std::stoi(conn_ptr->value(0));
        }
    }
    if (machine_count == 0) {
        sql = fmt::format(
            "INSERT INTO machine (user_id,machine_name) values('{}','{}')",
            user_id, machine_name);
        LOG(INFO) << "isMachineExist insert SQL: " << sql;
        conn_ptr->update(sql);
    }
}

bool ServerManagerImpl::isTableExist(std::string table_name,
                                     std::shared_ptr<MysqlConn> conn_ptr) {
    std::string select_sql = fmt::format(
        "SELECT * FROM tableRegister tr WHERE table_name = '{}'", table_name);
    if (conn_ptr->query(select_sql) == true) {
        if (conn_ptr->next()) {
            return true;
        } else {
            // 新建一个表
            // 这里可能会有两个客户端同时建一张表的情况 => 互斥
            std::unique_lock<std::mutex> locker(create_mutex_);
            std::string create_table_sql =
                fmt::format("CREATE TABLE {} {}", table_name,
                            sql_book["create_table_sql"].as<std::string>());
            std::string register_sql = fmt::format(
                "INSERT INTO tableRegister (table_name) values('{}')",
                table_name);
            if (conn_ptr->update(create_table_sql) &&
                conn_ptr->update(register_sql)) {
                return true;
            }
            return false;
        }
    }
    return false;
}
#if RPC_TYPE_DEFINE == MPRPC
void UserManagerImpl::LoginRegister(
    ::google::protobuf::RpcController* controller,
    const ::monitor::proto::UserMessage* request,
    ::monitor::proto::UserResponseMessage* response,
    ::google::protobuf::Closure* done) {
    LOG(INFO) << "RPC Call: UserManagerImpl::LoginRegister";
    account_num_ = request->account_num();
    pwd_ = request->pwd();
    if (account_num_.empty()) {
        response->set_response_str(registerNewUser());
    } else {
        response->set_response_str(verifyLoginInformation());
    }
    queryUserMachineName(response);
    account_num_.clear();
    pwd_.clear();
    done->Run();
}
#elif RPC_TYPE_DEFINE == GRPC
::grpc::Status UserManagerImpl::LoginRegister(
    ::grpc::ServerContext* context,
    const ::monitor::proto::UserMessage* request,
    ::monitor::proto::UserResponseMessage* response) {
    LOG(INFO) << "RPC Call: UserManagerImpl::LoginRegister";
    account_num_ = request->account_num();
    pwd_ = request->pwd();
    if (account_num_.empty()) {
        response->set_response_str(registerNewUser());
    } else {
        response->set_response_str(verifyLoginInformation());
    }
    queryUserMachineName(response);
    account_num_.clear();
    pwd_.clear();
    return grpc::Status::OK;
}
#endif

std::string UserManagerImpl::verifyLoginInformation() {
    std::string response;
    std::string select_account_sql =
        "SELECT password FROM `user` u WHERE accountnum = ";
    select_account_sql += account_num_;
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string pwd;
    if (conn_ptr->query(select_account_sql) == true) {
        while (conn_ptr->next()) {
            pwd = conn_ptr->value(0);
        }
        if (pwd.empty()) {
            response = "account num error!";
        } else if (pwd == pwd_) {
            response = "login successful";
        } else {
            response = "pwd error!";
        }
    }
    return response;
}

void UserManagerImpl::queryUserMachineName(
    ::monitor::proto::UserResponseMessage* response) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql = fmt::format(
        "SELECT machine_name FROM `machine` m WHERE user_id =(SELECT id FROM "
        "`user` u WHERE accountnum = '{}')",
        account_num_);
    LOG(INFO) << "queryUserMachineName: " << sql;
    std::vector<std::string> machine_name_array;
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            machine_name_array.push_back(conn_ptr->value(0));
        }
        for (int i = 0; i < machine_name_array.size(); i++) {
            auto machine_name_msg = response->add_machine_name_array();
            *machine_name_msg = machine_name_array[i];
        }
    }
}

std::string UserManagerImpl::registerNewUser() {
    std::string user_num_same_account;
    std::string response = "";
    std::string sql = "SELECT count(*) FROM  `user` u WHERE accountnum = ";
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    while (response.empty()) {
        std::string gen_number = generateRandomSixNumber();
        sql += gen_number;
        if (conn_ptr->query(sql) == true) {
            while (conn_ptr->next()) {
                user_num_same_account = conn_ptr->value(0);
            }
            if (std::stoi(user_num_same_account) == 0) {
                // INSERT INTO `user` (password,accountnum)
                // values("88888888m","1933720");
                sql = "INSERT INTO `user` (password,accountnum) values('" +
                      pwd_ + "','" + gen_number + "')";
                if (conn_ptr->query(sql)) {
                    response = gen_number;
                }
            }
        }
    }
    return response;
}

std::string UserManagerImpl::generateRandomSixNumber() {
    std::string response;
    // 用当前时间作为随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < 6; ++i) {
        // 生成0-6的随机数
        int random_number = std::rand() % 10;
        response += std::to_string(random_number);
    }

    return response;
}

}  // namespace monitor