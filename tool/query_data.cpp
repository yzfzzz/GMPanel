#include "query_data.h"
#include <algorithm>
#include <iostream>
#include "get_time.h"
#include "log.h"
namespace monitor {
bool QueryData::queryDataInfo(std::string account_num,
                              std::string machine_name) {
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << __func__
              << std::endl;
    monitor::RpcClient rpc_client;
    this->account_num = account_num;
    this->machine_name = machine_name;
    GetCurTime cur_time;

    ::monitor::proto::QueryMessage query_message;
    query_message.set_timeymd(cur_time.get_year_mon_day());
    query_message.set_machine_name(machine_name);
    query_message.set_accountnum(account_num);

    ::monitor::proto::QueryResults query_results;
    rpc_client.GetMonitorInfo(query_message, query_results);
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << __func__
              << std::endl;

    // TODO: 远端函数调用可能会失败，需要处理
    ::monitor::proto::QueryResults* result = &query_results;

    midinfo.machine_name = result->machine_name();
    midinfo.hard_disk_total = result->hard_disk_total();
    midinfo.hard_disk_used = result->hard_disk_used();
    midinfo.hard_disk_used_percent = result->hard_disk_used_percent();
    midinfo.cpu_percent = result->cpu_percent();
    midinfo.cpu_logic_num = result->cpu_logic_num();
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << __func__
              << "result->cpu_each_core_uesd: " << result->cpu_each_core_uesd()
              << std::endl;
    midinfo.cpu_each_core_uesd =
        json2Vector<float>(result->cpu_each_core_uesd());
    midinfo.mem_total = result->mem_total();
    midinfo.mem_avail = result->mem_avail();
    midinfo.os_name = result->os_name();
    midinfo.os_startup_time = result->os_startup_time();
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " << __func__
              << std::endl;
    midinfo.gpu_id = json2Vector<std::string>(result->gpu_id());
    midinfo.gpu_name = json2Vector<std::string>(result->gpu_name());
    midinfo.gpu_total_mem = json2Vector<int>(result->gpu_total_mem());
    midinfo.gpu_used_mem = json2Vector<int>(result->gpu_used_mem());
    midinfo.gpu_temperture = json2Vector<int>(result->gpu_temperture());
    midinfo.gpu_mem_utilize = json2Vector<int>(result->gpu_mem_utilize());

    for (int i = 0; i < result->net_rcv_rate_size(); i++) {
        midinfo.net_rcv_rate_array.push_back(result->net_rcv_rate(i));
        midinfo.net_send_rate_array.push_back(result->net_send_rate(i));
        midinfo.timehms_array.push_back(result->timehms(i));
    }
    return true;
}

}  // namespace monitor