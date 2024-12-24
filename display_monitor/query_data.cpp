#include <algorithm>
#include <iostream>
#include "get_time.h"
#include "log.h"
#include "query_data.h"
namespace monitor {
bool queryData::queryDataInfo(std::string account_num, int count,
                              monitor::RpcClient& rpc_client) {
    get_curTime cur_time;
    // select * from table_20241126 t WHERE user_id = (SELECT id FROM `user` u
    // WHERE accountnum = 12345678)  order by time desc limit 10;
    std::string table_name = "table_" + cur_time.get_year_mon_day();
    std::string select_count = std::to_string(count);

    std::string sql =
        "select * from " + table_name + " WHERE user_id = " +
        "(SELECT id FROM `user` u WHERE accountnum = " + account_num + ")" +
        " order by time desc limit " + select_count;

    std::cout << "sql: " << sql << std::endl;

    ::monitor::proto::QueryMessage q_message;
    q_message.set_sql(sql);
    ::monitor::proto::QueryResults response;
    rpc_client.GetMonitorInfo(q_message, response);
    if (response.query_data_size() == 0) {
        return false;
    }
    MidInfo midinfo;
    std::cout << "response.query_data_size: " << response.query_data_size() << std::endl;
    for (int i = 0; i < response.query_data_size(); i++) {
        midinfo.gpu_num = response.query_data(i).gpu_num();
        midinfo.gpu_name = response.query_data(i).gpu_name();
        midinfo.gpu_used_mem = response.query_data(i).gpu_used_mem();
        midinfo.gpu_total_mem = response.query_data(i).gpu_total_mem();
        midinfo.gpu_avg_util = response.query_data(i).gpu_avg_util();
        midinfo.cpu_load_avg_1 = response.query_data(i).cpu_load_avg_1();
        midinfo.cpu_load_avg_3 = response.query_data(i).cpu_load_avg_3();
        midinfo.cpu_load_avg_15 = response.query_data(i).cpu_load_avg_15();
        midinfo.mem_used = response.query_data(i).mem_used();
        midinfo.mem_total = response.query_data(i).mem_total();
        midinfo.net_send_rate = response.query_data(i).net_send_rate();
        midinfo.net_rcv_rate = response.query_data(i).net_rcv_rate();
        midinfo.timehms = response.query_data(i).timehms();
        queryData_array.push_back(midinfo);
    }
    std::reverse(queryData_array.begin(), queryData_array.end());
    return true;
}

}  // namespace monitor