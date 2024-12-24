#include <QDateTime>
#include <QThread>
#include <QString>
#include <iostream>
#include <string>
#include "ui_widget.h"
#include "widget.h"
#include "work_thread.h"

Widget::Widget(int argc, char **argv,QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->label_up_1->setText("显卡使用率");
    ui->label_up_2->setText("显卡内存");
    ui->label_up_3->setText("CPU使用率");
    ui->label_up_4->setText("运行内存");

    QThread* sub = new QThread;
    WorkThread* work_thread_ = new WorkThread;
    work_thread_->moveToThread(sub);
    connect(this, &Widget::starting, work_thread_, &WorkThread::run);
    emit starting(argc, argv);
    sub->start();
    
    connect(work_thread_, &WorkThread::send_label_down_str, this,
            &Widget::recv_label_down_str);
    connect(work_thread_, &WorkThread::send_label_down_qss, this,
            &Widget::recv_label_down_qss);
    connect(work_thread_, &WorkThread::send_water_value, this,
            &Widget::recv_water_value);
    connect(work_thread_, &WorkThread::send_gpu_infos, this,
            &Widget::recv_gpu_infos);
    connect(work_thread_, &WorkThread::send_net_data, this,
            &Widget::recv_net_data);
    connect(work_thread_, &WorkThread::send_gpu_list, this,
            &Widget::recv_gpu_list);
}

void Widget::recv_label_down_str(QVector<QString> qstr_array) {
    ui->label_down_1->setText(qstr_array[0]);
    ui->label_down_2->setText(qstr_array[1]);
    ui->label_down_3->setText(qstr_array[2]);
    ui->label_down_4->setText(qstr_array[3]);
}

void Widget::recv_label_down_qss(QVector<QString> qss_array) {
    ui->label_down_1->setStyleSheet(qss_array[0]);
    ui->label_down_3->setStyleSheet(qss_array[1]);
}

void Widget::recv_water_value(QVector<int> water_array) {
    ui->water1->setValue(water_array[0]);
    ui->water2->setValue(water_array[1]);
    ui->water3->setValue(water_array[2]);
    ui->water4->setValue(water_array[3]);
}

void Widget::recv_gpu_infos(QVector<QString> gpu_infos_array) {
    ui->gpu_name->setText(gpu_infos_array[0]);
    ui->gpu_num->setText(gpu_infos_array[1]);
}

void Widget::recv_net_data(QList<QPointF> send_list, QList<QPointF> recv_list) {
    ui->schart_right->drawChart(send_list, recv_list);
}

void Widget::recv_gpu_list(QList<QPointF> gpu_list) {
    ui->schart_left->drawChart(gpu_list);
}

Widget::~Widget() { delete ui; }
