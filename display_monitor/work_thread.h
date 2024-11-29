#pragma once

#include <QDateTime>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QThread>
#include <QVector>
#include <iostream>
#include "client/rpc_client.h"
#include "mprpcapplication.h"
#include "query_data.h"

class WorkThread : public QObject {
    Q_OBJECT
   public:
    explicit WorkThread(QObject* parent = nullptr);

    void run(int argc, char** argv);

   signals:
    void sendLabelDownStr(QVector<QString> qstr_array);
    void sendLabelDownQss(QVector<QString> qss_array);
    void sendWaterValue(QVector<int> water_array);
    void sendGpuInfos(QVector<QString> gpu_infos_array);
    void sendNetList(QList<QPointF> send_list, QList<QPointF> recv_list);
    void sendGpuList(QList<QPointF> gpu_list);

   private:
    void dataReceived(int value_gpu, int value_send, int value_recv,
                      std::string cur_time);
    void sendData();
    void updateData(std::vector<monitor::MidInfo>& midinfo_array);

    int max_size_;

    QString gpu_name_qt_str_;
    QString gpu_num_qt_str_;

    QString label_down_1_qt_str_;
    QString label_down_1_qss_;
    int water_value_1_;

    int water_value_2_;
    QString label_down_2_qt_str_;

    int water_value_3_;
    QString label_down_3_qt_str_;
    QString label_down_3_qss_;

    int water_value_4_;
    QString label_down_4_qt_str_;

    QList<QPointF> net_list_send_;  // 存储业务数据的 list
    QList<QPointF> net_list_recv_;
    QList<QPointF> gpu_list_;
};