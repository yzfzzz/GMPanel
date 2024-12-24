#pragma once

#include <QString>
#include <QObject>
#include <QVector>
#include <QPointF>
#include "client/rpc_client.h"
#include "mprpcapplication.h"
#include "query_data.h"

class WorkThread : public QObject {
    Q_OBJECT
   public:
    explicit WorkThread(QObject* parent = nullptr);

    void run(int argc, char** argv);

   signals:
    void send_label_down_str(QVector<QString> qstr_array);
    void send_label_down_qss(QVector<QString> qss_array);
    void send_water_value(QVector<int> water_array);
    void send_gpu_infos(QVector<QString> gpu_infos_array);
    void send_net_data(QList<QPointF> send_list, QList<QPointF> recv_list);
    void send_gpu_list(QList<QPointF> gpu_list);

   private:
    void dataReceived(int value_gpu, int value_send, int value_recv,
                      std::string cur_time);
    void send_data();
    void update_data(std::vector<monitor::MidInfo>& midinfo_array);

    int maxSize;

    QString gpu_name_qt_str;
    QString gpu_num_qt_str;

    QString label_down_1_qt_str;
    QString label_down_1_qss;
    int water_value_1;

    int water_value_2;
    QString label_down_2_qt_str;

    int water_value_3;
    QString label_down_3_qt_str;
    QString label_down_3_qss;

    int water_value_4;
    QString label_down_4_qt_str;

    QList<QPointF> net_data_send;  // 存储业务数据的 list
    QList<QPointF> net_data_recv;
    QList<QPointF> gpu_data;
};