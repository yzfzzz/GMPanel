#include <memory>
#include <thread>
#include <vector>
#include "mprpcapplication.h"
#include "client/rpc_client.h"
#include "cpu_load_monitor.h"
#include "cpu_softirq_monitor.h"
#include "cpu_stat_monitor.h"
#include "mem_monitor.h"
#include "monitor_inter.h"
#include "net_monitor.h"
#include "gpu_monitor.h"

#include "monitor_info.pb.h"
#include "monitor_info.pb.h"
#include "log.h"

int main(int argc, char** argv) {
    char* fifo_path = "py_cpp_pipe.fifo";
    MprpcApplication::Init(argc, argv);
    google::InitGoogleLogging(argv[0]);
    std::string log_path = "./logs";
    monitor::SetupLogging(log_path);
    std::vector<std::shared_ptr<monitor::MonitorInter>> runners_;
    runners_.emplace_back(new monitor::CpuSoftIrqMonitor());
    runners_.emplace_back(new monitor::CpuLoadMonitor());
    runners_.emplace_back(new monitor::CpuStatMonitor());
    runners_.emplace_back(new monitor::MemMonitor());
    runners_.emplace_back(new monitor::NetMonitor());
    runners_.emplace_back(new monitor::GpuMonitor(fifo_path));


    std::string server_address = "localhost:50051";
    LOG(INFO) << "server_address: " << server_address;
    LOG(WARNING) << "This is a warning message.";
    LOG(ERROR) << "This is an error message.";

    monitor::RpcClient rpc_client_(server_address);
    char* name = getenv("USER");
    LOG(INFO) << "name " << name;
    std::unique_ptr<std::thread> thread_ = nullptr;
    thread_ = std::make_unique<std::thread>([&]() {
        while (true) {
            monitor::proto::MonitorInfo monitor_info;
            monitor_info.set_name(std::string(name));
            for (auto& runner : runners_) {
                runner->UpdateOnce(&monitor_info);
            }

            rpc_client_.SetMonitorInfo(monitor_info);
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });

    thread_->join();
    return 0;
}
