#include <QCommandLinkButton>
#include <QDateTime>
#include <QDesktopServices>
#include <QString>
#include <QThread>
#include <QUrl>
#include <iostream>
#include <QDebug>
#include <string>
#include "ui_widget.h"
#include "widget.h"
#include "work_thread.h"

Widget::Widget(int argc, char** argv, std::string account_num,
               std::vector<std::string> machine_name_array, int index,
               QWidget* parent)
    : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->label_up_1->setText("显卡使用率");
    ui->label_up_2->setText("显卡内存");
    ui->label_up_3->setText("CPU使用率");
    ui->label_up_4->setText("运行内存");
    ui->gpu_name->setText("暂无监控的服务器\n请检查监控端连接");
    ui->gpu_num->setText("暂无监控的服务器\n请检查监控端连接");
    ui->label_down_1->setText("???");
    ui->label_down_2->setText("???");
    ui->label_down_3->setText("???");
    ui->label_down_4->setText("???");

    QString user_name = QString::fromStdString("用户" + account_num);
    ui->label_user->setText(user_name);
    machine_comboBox = ui->machine_comboBox;

    QStringList qstringlist;
    for (std::string str : machine_name_array) {
        qstringlist.append(QString::fromStdString(str));
    }

    ui->machine_comboBox->addItems(qstringlist);

    QThread* sub_thread = new QThread;
    WorkThread* work_thread = new WorkThread;
    work_thread->moveToThread(sub_thread);
    connect(this, &Widget::set_workthread_arg, work_thread, &WorkThread::run);
    emit set_workthread_arg(argc, argv, account_num, machine_name_array[index]);
    sub_thread->start();

    connect(work_thread, &WorkThread::sendLabelDownStr, this,
            &Widget::recvLabelDownStr);
    connect(work_thread, &WorkThread::sendLabelDownQss, this,
            &Widget::recvLabelDownQss);
    connect(work_thread, &WorkThread::sendWaterValue, this,
            &Widget::recvWaterValue);
    connect(work_thread, &WorkThread::sendGpuInfos, this,
            &Widget::recvGpuInfos);
    connect(work_thread, &WorkThread::sendNetList, this, &Widget::recvNetList);
    connect(work_thread, &WorkThread::sendGpuList, this, &Widget::recvGpuList);
}

void Widget::recvLabelDownStr(QVector<QString> qstr_array) {
    ui->label_down_1->setText(qstr_array[0]);
    ui->label_down_2->setText(qstr_array[1]);
    ui->label_down_3->setText(qstr_array[2]);
    ui->label_down_4->setText(qstr_array[3]);
}

void Widget::recvLabelDownQss(QVector<QString> qss_array) {
    ui->label_down_1->setStyleSheet(qss_array[0]);
    ui->label_down_3->setStyleSheet(qss_array[1]);
}

void Widget::recvWaterValue(QVector<int> water_array) {
    ui->water1->setValue(water_array[0]);
    ui->water2->setValue(water_array[1]);
    ui->water3->setValue(water_array[2]);
    ui->water4->setValue(water_array[3]);
}

void Widget::recvGpuInfos(QVector<QString> gpu_infos_array) {
    ui->gpu_name->setText(gpu_infos_array[0]);
    ui->gpu_num->setText(gpu_infos_array[1]);
}

void Widget::recvNetList(QList<QPointF> send_list, QList<QPointF> recv_list) {
    ui->schart_right->drawChart(send_list, recv_list);
}

void Widget::recvGpuList(QList<QPointF> gpu_list) {
    ui->schart_left->drawChart(gpu_list);
}

Widget::~Widget() { delete ui; }
