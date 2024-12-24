#include <iostream>
#include "get_time.h"
#include "json.hpp"
#include "log.h"
#include "rpc_manager.h"
using namespace std;
namespace monitor {

ServerManagerImpl::ServerManagerImpl() {}
ServerManagerImpl::~ServerManagerImpl() {}

// ::grpc::Status ServerManagerImpl::SetMonitorInfo(
//     ::grpc::ServerContext* context,
//     const ::monitor::proto::MonitorInfo* request,
//     ::google::protobuf::Empty* response) {
//     monitor_infos_.Clear();
//     monitor_infos_ = *request;

//     cout << "jinru" << request->soft_irq_size() << endl;
//     return grpc::Status::OK;
// }

void ServerManagerImpl::SetMonitorInfo(
    ::google::protobuf::RpcController* controller,
    const ::monitor::proto::MonitorInfo* request,
    ::google::protobuf::Empty* response, ::google::protobuf::Closure* done) {
    LOG(INFO) << "RPC Call: ServerManagerImpl::SetMonitorInfo";
    monitor_infos_.Clear();
    monitor_infos_ = *request;
    InsertOneInfo(monitor_infos_);
    done->Run();
}

// ::grpc::Status ServerManagerImpl::GetMonitorInfo(
//     ::grpc::ServerContext* context, const ::google::protobuf::Empty* request,
//     ::monitor::proto::MonitorInfo* response) {
//     *response = monitor_infos_;
//     return grpc::Status::OK;
// }

void ServerManagerImpl::GetMonitorInfo(
    ::google::protobuf::RpcController* controller,
    const ::monitor::proto::QueryMessage* request,
    ::monitor::proto::QueryResults* response,
    ::google::protobuf::Closure* done) {
    LOG(INFO) << "RPC Call: ServerManagerImpl::GetMonitorInfo";
    *response = queryDataInfo(request);
    done->Run();
}

::monitor::proto::QueryResults ServerManagerImpl::queryDataInfo(
    const ::monitor::proto::QueryMessage* request
    ) {
    std::string sql = request->sql();
    ::monitor::proto::QueryResults res_array;
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            auto query_data_msg = res_array.add_query_data();
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
    return res_array;
}

MidInfo ServerManagerImpl::parseInfos(
    monitor::proto::MonitorInfo& monitor_infos_) {
    MidInfo mifo;
    mifo.gpu_num = monitor_infos_.gpu_info_size();
    if (monitor_infos_.gpu_info_size() > 0) {
        mifo.gpu_name = monitor_infos_.gpu_info(0).gpu_name();
        mifo.gpu_used_mem = monitor_infos_.gpu_info(0).gpu_mem_used();
        mifo.gpu_total_mem = monitor_infos_.gpu_info(0).gpu_mem_total();
        mifo.gpu_avg_util = monitor_infos_.gpu_info(0).gpu_mem_utilize();
    }
    mifo.cpu_load_avg_1 = monitor_infos_.cpu_load().load_avg_1();
    mifo.cpu_load_avg_3 = monitor_infos_.cpu_load().load_avg_3();
    mifo.cpu_load_avg_15 = monitor_infos_.cpu_load().load_avg_15();
    mifo.mem_used = monitor_infos_.mem_info().used_percent();
    mifo.mem_total = monitor_infos_.mem_info().total();
    if (monitor_infos_.net_info_size() > 0) {
        float send_sum = 0;
        float rcv_sum = 0;
        for (int i = 0; i < monitor_infos_.net_info_size(); i++) {
            send_sum += monitor_infos_.net_info(i).send_rate();
            rcv_sum += monitor_infos_.net_info(i).rcv_rate();
        }
        mifo.net_send_rate = send_sum / monitor_infos_.net_info_size();
        mifo.net_rcv_rate = rcv_sum / monitor_infos_.net_info_size();
    }
    mifo.accountnum = monitor_infos_.accountnum();
    mifo.timeymd = monitor_infos_.time().timeymd();
    mifo.timehms = monitor_infos_.time().timehms();
    LOG(INFO) << "Parse monitor_infos_...";
    return mifo;
}

bool ServerManagerImpl::InsertOneInfo(
    monitor::proto::MonitorInfo& monitor_infos_) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();

    MidInfo mifo = parseInfos(monitor_infos_);

    std::string tableName = "table_" + mifo.timeymd;
    bool table_exist = isTableExist(tableName, conn_ptr);
    if (!table_exist) {
        LOG(WARNING) << "Failed to select table!";
        return false;
    }

    std::string sql =
        "INSERT INTO " + tableName +
        "(gpu_name, gpu_num, gpu_used_mem, gpu_total_mem, gpu_avg_util, " +
        "cpu_load_avg_1, cpu_load_avg_3, cpu_load_avg_15, mem_used,mem_total "
        ", " +
        "net_send_rate, net_rcv_rate, user_id, time) " + "VALUES(";
    LOG(INFO) << "InsertOneInfo SQL: " << sql;

    std::string user_id = SelectUserId(mifo.accountnum);
    if (user_id.empty() == true) {
        return false;
    }
    sql = sql + "'" + mifo.gpu_name + "'" + "," + std::to_string(mifo.gpu_num) +
          "," + std::to_string(mifo.gpu_used_mem) + "," +
          std::to_string(mifo.gpu_total_mem) + "," +
          std::to_string(mifo.gpu_avg_util) + "," +
          std::to_string(mifo.cpu_load_avg_1) + "," +
          std::to_string(mifo.cpu_load_avg_3) + "," +
          std::to_string(mifo.cpu_load_avg_15) + "," +
          std::to_string(mifo.mem_used) + "," + std::to_string(mifo.mem_total) +
          "," + std::to_string(mifo.net_send_rate) + "," +
          std::to_string(mifo.net_rcv_rate) + "," + user_id + "," + "'" +
          mifo.timehms + "')";
    LOG(INFO) << sql;
    if (conn_ptr->update(sql)) {
        LOG(INFO) << "Succeed to insert one sql";
        return true;
    }
    LOG(WARNING) << "Failed to insert one sql";
    return false;
}

std::string ServerManagerImpl::SelectUserId(std::string accountNum) {
    std::shared_ptr<MysqlConn> conn_ptr = this->pool->getConnection();
    std::string sql = "SELECT id FROM `user` u WHERE accountnum =" + accountNum;
    std::string user_id;
    if (conn_ptr->query(sql) == true) {
        while (conn_ptr->next()) {
            user_id = conn_ptr->value(0);
        }
        LOG(INFO) << "Succeed to select user_id";
        return user_id;
    }
    LOG(WARNING) << "Failed to request user_id!";
    return "";
}

bool ServerManagerImpl::isTableExist(std::string tableName,
                                     std::shared_ptr<MysqlConn> conn_ptr) {
    std::string sql =
        "SELECT * FROM tableRegister tr WHERE table_name = '" + tableName + "'";
    LOG(INFO) << "isTableExist select tableRegister SQL: " << sql;
    if (conn_ptr->query(sql) == true) {
        if (conn_ptr->next()) {
            return true;
        } else {
            // 新建一个表
            // 这里可能会有两个客户端同时建一张表的情况 => 互斥
            std::unique_lock<std::mutex> locker(m_create_mutex);
            std::string create_sql =
                "CREATE TABLE " + tableName + " " + create_subsql;
            std::string register_sql =
                "INSERT INTO tableRegister (table_name) values('" + tableName +
                "')";
            LOG(INFO) << "isTableExist create table SQL: " << create_sql;
            if (conn_ptr->update(create_sql) &&
                conn_ptr->update(register_sql)) {
                return true;
            }
            return false;
        }
    }
    return false;
}

}  // namespace monitor