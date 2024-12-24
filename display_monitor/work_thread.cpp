#include "work_thread.h"
#include <QThread>
#include <QDateTime>
#include <iostream>

WorkThread::WorkThread(QObject* parent) : QObject(parent) {
    maxSize = 30;
}

void WorkThread::run(int argc, char** argv) {
    MprpcApplication::Init(argc, argv);
    std::string server_address = "localhost:50051";
    monitor::RpcClient rpc_client(server_address);

    monitor::queryData queryData;
    if (queryData.queryDataInfo("12345678", 30, rpc_client)) {
       update_data(queryData.queryData_array);
        std::cout << "queryData init succeed!" << std::endl;
    }
    while (true) {
        int n = 1;
        queryData.queryData_array.clear();
        if (queryData.queryDataInfo("12345678", n, rpc_client)) {
            update_data(queryData.queryData_array);
        }
        sleep(3);	
    }
}

void WorkThread::update_data(std::vector<monitor::MidInfo>& midinfo_array) {
    if (midinfo_array.empty()) {
        return;
    }
    monitor::MidInfo midinfo = midinfo_array[midinfo_array.size() - 1];

    std::string gpu_name = "显卡型号\n" + midinfo.gpu_name;
    gpu_name_qt_str = QString::fromStdString(gpu_name);

    std::string gpu_num = "显卡数量\n" + std::to_string(midinfo.gpu_num);
    gpu_num_qt_str = QString::fromStdString(gpu_num);

    // GPU使用率
    water_value_1 = midinfo.gpu_avg_util;
    if (midinfo.gpu_avg_util > 70) {
        label_down_1_qt_str = "繁忙";
        label_down_1_qss = "QLabel{color:red;}";
    } else {
        label_down_1_qt_str = "流畅";
        label_down_1_qss = "QLabel{color:green;}";
    }

    // 显存使用情况
    water_value_2 = (int)(midinfo.gpu_used_mem * 100 / midinfo.gpu_total_mem);
    std::string label_down_2_str = std::to_string(midinfo.gpu_used_mem) + "/" +
                                   std::to_string(midinfo.gpu_total_mem) +
                                   "(M)";
    label_down_2_qt_str = QString::fromStdString(label_down_2_str);

    // CPU
    water_value_3 = midinfo.cpu_load_avg_1 * 10;
    if (midinfo.cpu_load_avg_3 * 10 > 70) {
        label_down_3_qt_str = "繁忙";
        label_down_3_qss = "QLabel{color:red;}";
    } else {
        label_down_3_qt_str = "流畅";
        label_down_3_qss = "QLabel{color:green;}";
    }

    // 运行内存
    int total_mem = midinfo.mem_total;
    int used_mem = midinfo.mem_used * midinfo.mem_total * 0.01;
    water_value_4 = midinfo.mem_used;

    std::string label_down_4_str =
        std::to_string(used_mem) + "/" + std::to_string(total_mem) + "(G)";

    label_down_4_qt_str = QString::fromStdString(label_down_4_str);

    for (int i = 0; i < midinfo_array.size(); i++) {
        // GPU使用情况, 网络收发
        // std::cout << "[" << i << "]  " << midinfo_array[i].timehms << ":  "
        //           << midinfo_array[i].net_rcv_rate << std::endl;
        dataReceived(midinfo_array[i].gpu_avg_util,midinfo_array[i].net_send_rate,
                                       midinfo_array[i].net_rcv_rate,
                                       midinfo_array[i].timehms);
    }
    send_data();
}

void WorkThread::send_data() {
std::cout << "running func: WorkThread::send_data()" << std::endl;
    QVector<QString> qstr_array;
    qstr_array.push_back(label_down_1_qt_str);
    qstr_array.push_back(label_down_2_qt_str);
    qstr_array.push_back(label_down_3_qt_str);
    qstr_array.push_back(label_down_4_qt_str);
    emit send_label_down_str(qstr_array);

    QVector<QString> qss_array;
    qss_array.push_back(label_down_1_qss);
    qss_array.push_back(label_down_3_qss);
    emit send_label_down_qss(qss_array);

    QVector<int> water_array;
    water_array.push_back(water_value_1);
    water_array.push_back(water_value_2);
    water_array.push_back(water_value_3);
    water_array.push_back(water_value_4);
    emit send_water_value(water_array);

    QVector<QString> gpu_infos_array;
    gpu_infos_array.push_back(gpu_name_qt_str);
    gpu_infos_array.push_back(gpu_num_qt_str);
    emit send_gpu_infos(gpu_infos_array);

    emit send_net_data(net_data_send, net_data_recv);
    emit send_gpu_list(gpu_data);
}

void WorkThread::dataReceived(int value_gpu, int value_send, int value_recv,
                              std::string cur_time) {
                                std::cout << "running func: WorkThread::dataReceived" << std::endl;
    std::cout << cur_time << std::endl;
    QString timeStr = QString::fromStdString(cur_time);

    QDateTime time = QDateTime::fromString(timeStr, "hh:mm:ss");

    QPointF send_point(time.toMSecsSinceEpoch(), value_send);
    QPointF recv_point(time.toMSecsSinceEpoch(), value_recv);
    QPointF gpu_point(time.toMSecsSinceEpoch(), value_gpu);

    net_data_send << send_point;
    net_data_recv << recv_point;
    gpu_data << gpu_point;

    // 数据个数超过了最大数量，则删除最先接收到的数据，实现曲线向前移动
    while (net_data_send.size() > maxSize) {
        net_data_send.removeFirst();
    }
    while (net_data_recv.size() > maxSize) {
        net_data_recv.removeFirst();
    }
    while (gpu_data.size() > maxSize) {
        gpu_data.removeFirst();
    }
}
