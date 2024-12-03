#include <iostream>
#include "get_time.h"
#include "json.hpp"
#include "log.h"
#include "rpc_manager.h"
namespace monitor {

ServerManagerImpl::ServerManagerImpl() {}
ServerManagerImpl::~ServerManagerImpl() {}
UserManagerImpl ::UserManagerImpl() {}
UserManagerImpl ::~UserManagerImpl() {}

void ServerManagerImpl::SetMonitorInfo(
    ::google::protobuf::RpcController* controller,
    const ::monitor::proto::MonitorInfo* request,
    ::google::protobuf::Empty* response, ::google::protobuf::Closure* done) {
    LOG(INFO) << "RPC Call: ServerManagerImpl::SetMonitorInfo";
    monitor_infos_.Clear();
    monitor_infos_ = *request;
    insertOneInfo(monitor_infos_);
    done->Run();
}

void ServerManagerImpl::GetMonitorInfo(
    ::google::protobuf::RpcController* controller,
    const ::monitor::proto::QueryMessage* request,
    ::monitor::proto::QueryResults* response,
    ::google::protobuf::Closure* done) {
    // LOG(INFO) << "RPC Call: ServerManagerImpl::GetMonitorInfo";
    *response = queryDataInfo(request);
    done->Run();
}

::monitor::proto::QueryResults ServerManagerImpl::queryDataInfo(
    const ::monitor::proto::QueryMessage* request) {
    std::string sql = request->sql();
    ::monitor::proto::QueryResults result_array;
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            auto query_data_msg = result_array.add_query_data();
            query_data_msg->set_gpu_num(std::stoi(conn_ptr->value(0)));
            query_data_msg->set_gpu_name(conn_ptr->value(1));
            query_data_msg->set_gpu_used_mem(std::stoi(conn_ptr->value(2)));
            query_data_msg->set_gpu_total_mem(std::stoi(conn_ptr->value(3)));
            query_data_msg->set_gpu_avg_util(std::stoi(conn_ptr->value(4)));
            query_data_msg->set_cpu_load_avg_1(std::stof(conn_ptr->value(5)));
            query_data_msg->set_cpu_load_avg_3(std::stof(conn_ptr->value(6)));
            query_data_msg->set_cpu_load_avg_15(std::stof(conn_ptr->value(7)));
            query_data_msg->set_mem_used(std::stof(conn_ptr->value(8)));
            query_data_msg->set_mem_total(std::stof(conn_ptr->value(9)));
            query_data_msg->set_net_send_rate(std::stof(conn_ptr->value(10)));
            query_data_msg->set_net_rcv_rate(std::stof(conn_ptr->value(11)));
            query_data_msg->set_timehms(conn_ptr->value(13));
        }
    }
    return result_array;
}

MidInfo ServerManagerImpl::parseInfos(
    monitor::proto::MonitorInfo& monitor_infos_) {
    MidInfo mid_info;
    mid_info.gpu_num = monitor_infos_.gpu_info_size();
    if (monitor_infos_.gpu_info_size() > 0) {
        mid_info.gpu_name = monitor_infos_.gpu_info(0).gpu_name();
        mid_info.gpu_used_mem = monitor_infos_.gpu_info(0).gpu_mem_used();
        mid_info.gpu_total_mem = monitor_infos_.gpu_info(0).gpu_mem_total();
        mid_info.gpu_avg_util = monitor_infos_.gpu_info(0).gpu_mem_utilize();
    }
    mid_info.cpu_load_avg_1 = monitor_infos_.cpu_load().load_avg_1();
    mid_info.cpu_load_avg_3 = monitor_infos_.cpu_load().load_avg_3();
    mid_info.cpu_load_avg_15 = monitor_infos_.cpu_load().load_avg_15();
    mid_info.mem_used = monitor_infos_.mem_info().used_percent();
    mid_info.mem_total = monitor_infos_.mem_info().total();
    if (monitor_infos_.net_info_size() > 0) {
        float send_sum = 0;
        float rcv_sum = 0;
        for (int i = 0; i < monitor_infos_.net_info_size(); i++) {
            send_sum += monitor_infos_.net_info(i).send_rate();
            rcv_sum += monitor_infos_.net_info(i).rcv_rate();
        }
        mid_info.net_send_rate = send_sum / monitor_infos_.net_info_size();
        mid_info.net_rcv_rate = rcv_sum / monitor_infos_.net_info_size();
    }
    mid_info.accountnum = monitor_infos_.accountnum();
    mid_info.machine_name = monitor_infos_.machine_name();
    mid_info.timeymd = monitor_infos_.time().timeymd();
    mid_info.timehms = monitor_infos_.time().timehms();
    // LOG(INFO) << "Parse monitor_infos_...";
    return mid_info;
}

bool ServerManagerImpl::insertOneInfo(
    monitor::proto::MonitorInfo& monitor_infos_) {
    LOG(INFO) << "Run: ServerManagerImpl::insertOneInfo";
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();

    MidInfo mid_info = parseInfos(monitor_infos_);

    std::string table_name = "table_" + mid_info.timeymd;
    if (!isTableExist(table_name, conn_ptr)) {
        // LOG(WARNING) << "Failed to select table!";
        return false;
    }

    std::string sql =
        "INSERT INTO " + table_name +
        "(gpu_name, gpu_num, gpu_used_mem, gpu_total_mem, gpu_avg_util, " +
        "cpu_load_avg_1, cpu_load_avg_3, cpu_load_avg_15, mem_used,mem_total," +
        "net_send_rate, net_rcv_rate, user_id, time, machine_name) " +
        "VALUES(";
    LOG(INFO) << "InsertOneInfo SQL: " << sql;

    std::string user_id = selectUserId(mid_info.accountnum);
    isMachineExist(user_id, mid_info.machine_name);
    if (user_id.empty() == true) {
        return false;
    }
    sql = sql + "'" + mid_info.gpu_name + "'" + "," +
          std::to_string(mid_info.gpu_num) + "," +
          std::to_string(mid_info.gpu_used_mem) + "," +
          std::to_string(mid_info.gpu_total_mem) + "," +
          std::to_string(mid_info.gpu_avg_util) + "," +
          std::to_string(mid_info.cpu_load_avg_1) + "," +
          std::to_string(mid_info.cpu_load_avg_3) + "," +
          std::to_string(mid_info.cpu_load_avg_15) + "," +
          std::to_string(mid_info.mem_used) + "," +
          std::to_string(mid_info.mem_total) + "," +
          std::to_string(mid_info.net_send_rate) + "," +
          std::to_string(mid_info.net_rcv_rate) + "," + user_id + "," + "'" +
          mid_info.timehms + "'" + ",'" + mid_info.machine_name + "')";
    // LOG(INFO) << sql;
    if (conn_ptr->update(sql)) {
        // LOG(INFO) << "Succeed to insert one sql";
        return true;
    }
    // LOG(WARNING) << "Failed to insert one sql";
    return false;
}

std::string ServerManagerImpl::selectUserId(std::string accountNum) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql = "SELECT id FROM `user` u WHERE accountnum =" + accountNum;
    std::string user_id;
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            user_id = conn_ptr->value(0);
        }
        // LOG(INFO) << "Succeed to select user_id";
        return user_id;
    }
    // LOG(WARNING) << "Failed to request user_id!";
    return "";
}

bool ServerManagerImpl::isMachineExist(std::string user_id,
                                       std::string machine_name) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql =
        "SELECT COUNT(*) FROM `machine` m WHERE user_id =" + user_id +
        " and machine_name='" + machine_name + "'";
    LOG(INFO) << "isMachineExist select SQL: " << sql;
    int machine_count = 0;
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            machine_count = std::stoi(conn_ptr->value(0));
        }
    }
    if (machine_count == 0) {
        sql = "INSERT INTO machine (user_id,machine_name) values(" + user_id +
              ",'" + machine_name + "')";
        LOG(INFO) << "isMachineExist insert SQL: " << sql;
        conn_ptr->update(sql);
    }
}

bool ServerManagerImpl::isTableExist(std::string table_name,
                                     std::shared_ptr<MysqlConn> conn_ptr) {
    std::string sql = "SELECT * FROM tableRegister tr WHERE table_name = '" +
                      table_name + "'";
    // LOG(INFO) << "isTableExist select tableRegister SQL: " << sql;
    if (conn_ptr->query(sql) == true) {
        if (conn_ptr->next()) {
            return true;
        } else {
            // 新建一个表
            // 这里可能会有两个客户端同时建一张表的情况 => 互斥
            std::unique_lock<std::mutex> locker(create_mutex_);
            std::string create_table_sql =
                "CREATE TABLE " + table_name + " " + create_subsql_;
            std::string register_sql =
                "INSERT INTO tableRegister (table_name) values('" + table_name +
                "')";
            LOG(INFO) << "isTableExist create table SQL: " << create_table_sql;
            if (conn_ptr->update(create_table_sql) &&
                conn_ptr->update(register_sql)) {
                return true;
            }
            return false;
        }
    }
    return false;
}

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
    std::string sql = "";
    sql = sql + "SELECT machine_name FROM `machine` m WHERE user_id = " +
          "(SELECT id "+
          "FROM `user` u WHERE accountnum = '" +
          account_num_ + "')";
    LOG(INFO) << "queryUserMachineName: "<< sql;
    std::vector<std::string> machine_name_array;
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            machine_name_array.push_back(conn_ptr->value(0));
        }
    }
    for (int i = 0; i < machine_name_array.size(); i++) {
        auto machine_name_msg = response->add_machine_name_array();
        *machine_name_msg = machine_name_array[i];
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
                    response = "register successful, account:" + gen_number +
                               ", automatically jump after 10s";
                }
            }
        }
    }
    return response;
}

std::string UserManagerImpl::generateRandomSixNumber() {
    std::string result;
    // 用当前时间作为随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < 6; ++i) {
        // 生成0-6的随机数
        int random_number = std::rand() % 10;
        result += std::to_string(random_number);
    }

    return result;
}

}  // namespace monitor