#include <algorithm>
#include <iostream>
#include "get_time.h"
#include "log.h"
#include "query_data.h"
namespace monitor {
bool QueryData::queryDataInfo(std::string account_num, std::string machine_name,
                              int count, monitor::RpcClient& rpc_client) {
    this->account_num = account_num;
    this->machine_name = machine_name;
    GetCurTime cur_time;
    // sql:
    // select * from table_20241126 t WHERE user_id = (SELECT id FROM `user` u
    // WHERE accountnum = 12345678)  order by time desc limit 10;
    std::string table_name = "table_" + cur_time.get_year_mon_day();
    std::string select_count = std::to_string(count);

    std::string sql =
        "select * from " + table_name + " WHERE user_id = " +
        "(SELECT id FROM `user` u WHERE accountnum = " + account_num +
        " and machine_name = '" + machine_name + "')" +
        " order by time desc limit " + select_count;

    std::cout << "queryDataInfo sql: " << sql << std::endl;

    ::monitor::proto::QueryMessage query_message;
    query_message.set_sql(sql);
    ::monitor::proto::QueryResults query_results;
    rpc_client.GetMonitorInfo(query_message, query_results);
    if (query_results.query_data_size() == 0) {
        return false;
    }
    MidInfo midinfo;
    for (int i = 0; i < query_results.query_data_size(); i++) {
        midinfo.gpu_num = query_results.query_data(i).gpu_num();
        midinfo.gpu_name = query_results.query_data(i).gpu_name();
        midinfo.gpu_used_mem = query_results.query_data(i).gpu_used_mem();
        midinfo.gpu_total_mem = query_results.query_data(i).gpu_total_mem();
        midinfo.gpu_avg_util = query_results.query_data(i).gpu_avg_util();
        midinfo.cpu_load_avg_1 = query_results.query_data(i).cpu_load_avg_1();
        midinfo.cpu_load_avg_3 = query_results.query_data(i).cpu_load_avg_3();
        midinfo.cpu_load_avg_15 = query_results.query_data(i).cpu_load_avg_15();
        midinfo.mem_used = query_results.query_data(i).mem_used();
        midinfo.mem_total = query_results.query_data(i).mem_total();
        midinfo.net_send_rate = query_results.query_data(i).net_send_rate();
        midinfo.net_rcv_rate = query_results.query_data(i).net_rcv_rate();
        midinfo.timehms = query_results.query_data(i).timehms();
        query_result_array_.push_back(midinfo);
    }
    std::reverse(query_result_array_.begin(), query_result_array_.end());
    return true;
}

}  // namespace monitor