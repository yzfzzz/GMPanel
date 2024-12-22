#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "client/rpc_client.h"
#include "cpu_load_monitor.h"
#include "cpu_softirq_monitor.h"
#include "cpu_stat_monitor.h"
#include "get_time.h"
#include "gpu_monitor.h"
#include "json.hpp"
#include "log.h"
#include "mem_monitor.h"
#include "monitor_info.pb.h"
#include "monitor_inter.h"
#include "mprpcapplication.h"
#include "net_monitor.h"

int main(int argc, char** argv) {
    std::ifstream config_file("./utils/monitor_config.json");
    nlohmann::json config_json_data;
    config_file >> config_json_data;
    config_file.close();

    std::string server_address = config_json_data["server_address"];
    std::string account_num = config_json_data["account_num"];
    std::string machine_name = config_json_data["machine_name"];
    std::string fifo_path = "./utils/"+account_num + "_" + machine_name + ".fifo";
    std::cout << "FIFO PATH:" << fifo_path << std::endl;

    MprpcApplication::Init(argc, argv);
    google::InitGoogleLogging(argv[0]);

    std::vector<std::shared_ptr<monitor::MonitorInter>> runners_;
    runners_.emplace_back(new monitor::CpuSoftIrqMonitor());
    runners_.emplace_back(new monitor::CpuLoadMonitor());
    runners_.emplace_back(new monitor::CpuStatMonitor());
    runners_.emplace_back(new monitor::MemMonitor());
    runners_.emplace_back(new monitor::NetMonitor());
    runners_.emplace_back(new monitor::GpuMonitor(fifo_path.c_str()));

    monitor::RpcClient rpc_client_(server_address);
    char* name = getenv("USER");
    LOG(INFO) << "name " << name;
    std::unique_ptr<std::thread> thread_ = nullptr;
    thread_ = std::make_unique<std::thread>([&]() {
        while (true) {
            monitor::proto::MonitorInfo monitor_info;
            monitor::GetCurTime cur_time;

            monitor_info.set_name(std::string(name));
            monitor_info.set_accountnum(account_num);
            monitor_info.set_machine_name(machine_name);

            auto curtime_msg = monitor_info.mutable_time();
            curtime_msg->set_timeymd(cur_time.get_year_mon_day());
            curtime_msg->set_timehms(cur_time.get_hour_min_sec());

            for (auto& runner : runners_) {
                runner->updateOnce(&monitor_info);
            }

            rpc_client_.SetMonitorInfo(monitor_info);
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });

    thread_->join();
    return 0;
}
