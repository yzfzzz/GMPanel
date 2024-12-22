#include <QThread>
#include "work_thread.h"

WorkThread::WorkThread(QObject* parent) : QObject(parent) { max_size_ = 30; }

void WorkThread::run(int argc, char** argv, std::string account_num,
                     std::string machine_name) {
    std::string server_address = "localhost:50051";
    monitor::RpcClient rpc_client(server_address);

    monitor::QueryData query_data;
    if (query_data.queryDataInfo(account_num, machine_name, 30, rpc_client)) {
        updateData(query_data.query_result_array_);
        std::cout << "queryData init succeed!" << std::endl;
    }
    QThread::msleep(100);

    while (true) {
        int n = 1;
        query_data.query_result_array_.clear();
        if (query_data.queryDataInfo(account_num, machine_name, n,
                                     rpc_client)) {
            updateData(query_data.query_result_array_);
        }
        sleep(3);
    }
}

void WorkThread::updateData(std::vector<monitor::MidInfo>& midinfo_array) {
    if (midinfo_array.empty()) {
        return;
    }
    monitor::MidInfo midinfo = midinfo_array[midinfo_array.size() - 1];

    std::string gpu_name = "显卡型号\n" + midinfo.gpu_name;
    gpu_name_qt_str_ = QString::fromStdString(gpu_name);

    std::string gpu_num = "显卡数量\n" + std::to_string(midinfo.gpu_num);
    gpu_num_qt_str_ = QString::fromStdString(gpu_num);

    // GPU使用率
    water_value_1_ = midinfo.gpu_avg_util;
    if (water_value_1_ > 70) {
        label_down_1_qt_str_ = "繁忙";
        label_down_1_qss_ = "QLabel{color:red;}";
    } else if (water_value_1_ > 30 && water_value_1_ <= 70) {
        label_down_1_qt_str_ = "良好";
        label_down_1_qss_ = "QLabel{color:#E4B703;}";
    } else {
        label_down_1_qt_str_ = "流畅";
        label_down_1_qss_ = "QLabel{color:green;}";
    }

    // 显存使用情况
    water_value_2_ = (int)(midinfo.gpu_used_mem * 100 / midinfo.gpu_total_mem);
    std::string label_down_2_str = std::to_string(midinfo.gpu_used_mem) + "/" +
                                   std::to_string(midinfo.gpu_total_mem) +
                                   "(M)";
    label_down_2_qt_str_ = QString::fromStdString(label_down_2_str);

    // CPU
    water_value_3_ = midinfo.cpu_load_avg_1 * 10;
    if (water_value_3_ > 70) {
        label_down_3_qt_str_ = "繁忙";
        label_down_3_qss_ = "QLabel{color:red;}";
    } else if (water_value_3_ > 30 && water_value_3_ <= 70) {
        label_down_3_qt_str_ = "良好";
        label_down_3_qss_ = "QLabel{color:#E4B703;}";
    } else {
        label_down_3_qt_str_ = "流畅";
        label_down_3_qss_ = "QLabel{color:green;}";
    }

    // 运行内存
    int total_mem = midinfo.mem_total;
    int used_mem = midinfo.mem_used * midinfo.mem_total * 0.01;
    water_value_4_ = midinfo.mem_used;

    std::string label_down_4_str =
        std::to_string(used_mem) + "/" + std::to_string(total_mem) + "(G)";

    label_down_4_qt_str_ = QString::fromStdString(label_down_4_str);

    for (int i = 0; i < midinfo_array.size(); i++) {
        // GPU使用情况, 网络收发
        std::cout << "[" << i << "]  " << midinfo_array[i].timehms << ":  "
                  << midinfo_array[i].net_rcv_rate << std::endl;
        dataReceived(midinfo_array[i].gpu_avg_util,
                     midinfo_array[i].net_send_rate,
                     midinfo_array[i].net_rcv_rate, midinfo_array[i].timehms);
    }
    sendData();
}

void WorkThread::sendData() {
    std::cout << "running func: WorkThread::send_data()" << std::endl;
    QVector<QString> qstr_array;
    qstr_array.push_back(label_down_1_qt_str_);
    qstr_array.push_back(label_down_2_qt_str_);
    qstr_array.push_back(label_down_3_qt_str_);
    qstr_array.push_back(label_down_4_qt_str_);
    emit sendLabelDownStr(qstr_array);

    QVector<QString> qss_array;
    qss_array.push_back(label_down_1_qss_);
    qss_array.push_back(label_down_3_qss_);
    emit sendLabelDownQss(qss_array);

    QVector<int> water_array;
    water_array.push_back(water_value_1_);
    water_array.push_back(water_value_2_);
    water_array.push_back(water_value_3_);
    water_array.push_back(water_value_4_);
    emit sendWaterValue(water_array);

    QVector<QString> gpu_infos_array;
    gpu_infos_array.push_back(gpu_name_qt_str_);
    gpu_infos_array.push_back(gpu_num_qt_str_);
    emit sendGpuInfos(gpu_infos_array);

    emit sendNetList(net_list_send_, net_list_recv_);
    emit sendGpuList(gpu_list_);
}

void WorkThread::dataReceived(int value_gpu, int value_send, int value_recv,
                              std::string cur_time) {
    std::cout << "running func: WorkThread::dataReceived " << cur_time
              << std::endl;
    QString time_str = QString::fromStdString(cur_time);

    QDateTime time = QDateTime::fromString(time_str, "hh:mm:ss");

    QPointF send_point(time.toMSecsSinceEpoch(), value_send);
    QPointF recv_point(time.toMSecsSinceEpoch(), value_recv);
    QPointF gpu_point(time.toMSecsSinceEpoch(), value_gpu);

    if(!net_list_send_.isEmpty() && net_list_send_.last().x()==time.toMSecsSinceEpoch())
    {
        return;
    }
    net_list_send_ << send_point;
    net_list_recv_ << recv_point;
    gpu_list_ << gpu_point;

    // 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动
    while (net_list_send_.size() > max_size_) {
        net_list_send_.removeFirst();
    }
    while (net_list_recv_.size() > max_size_) {
        net_list_recv_.removeFirst();
    }
    while (gpu_list_.size() > max_size_) {
        gpu_list_.removeFirst();
    }
}
