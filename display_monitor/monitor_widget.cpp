#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include "monitor_widget.h"

namespace monitor {

MonitorWidget::MonitorWidget(QWidget* parent) {}

QWidget* MonitorWidget::ShowAllMonitorWidget(const string& name) {
    QWidget* widget = new QWidget();
    stack_menu_ = new QStackedLayout();
    // ����񴰿ڼ��뵽ջ��
    stack_menu_->addWidget(InitCpuMonitorWidget());
    stack_menu_->addWidget(InitSoftIrqMonitorWidget());
    stack_menu_->addWidget(InitMemMonitorWidget());
    stack_menu_->addWidget(InitNetMonitorWidget());
    stack_menu_->addWidget(InitGpuMonitorWidget());

    // ��ʼ�����Բ��֣������ǰ�ť���ڣ������Ǳ�񴰿�(ʹ��ջ���л�)
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(InitButtonMenu(name), 1, 0);
    layout->addLayout(stack_menu_, 2, 0);
    widget->setLayout(layout);
    return widget;
}

QWidget* MonitorWidget::InitButtonMenu(const string& name) {
    // ��������
    QPushButton* cpu_button = new QPushButton(QString::fromStdString(name) + "-cpu", this);
    QPushButton* soft_irq_button =
        new QPushButton(QString::fromStdString(name) + "-soft irq", this);
    QPushButton* mem_button = new QPushButton(QString::fromStdString(name) + "-mem", this);
    QPushButton* net_button = new QPushButton(QString::fromStdString(name) + "-net", this);
    QPushButton* gpu_button = new QPushButton(QString::fromStdString(name) + "-gpu", this);

    QString button_style =
        "QPushButton{background-color:white;color: black; border-radius: 10px; border: 2px groove "
        "gray;border-style: outset;}"
        "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset; }";

    cpu_button->setStyleSheet(button_style);
    soft_irq_button->setStyleSheet(button_style);
    mem_button->setStyleSheet(button_style);
    net_button->setStyleSheet(button_style);
    gpu_button->setStyleSheet(button_style);
    // ��������
    QFont* font = new QFont("Meiryo", 15, 40);
    cpu_button->setFont(*font);
    soft_irq_button->setFont(*font);
    mem_button->setFont(*font);
    net_button->setFont(*font);
    gpu_button->setFont(*font);

    // ʹ��ˮƽ�Ų�
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(cpu_button);
    layout->addWidget(soft_irq_button);
    layout->addWidget(mem_button);
    layout->addWidget(net_button);
    layout->addWidget(gpu_button);

    // �浽������
    QWidget* widget = new QWidget();
    widget->setLayout(layout);

    // ���òۺ���
    connect(cpu_button, SIGNAL(clicked()), this, SLOT(ClickCpuButton()));
    connect(soft_irq_button, SIGNAL(clicked()), this, SLOT(ClickSoftIrqButton()));
    connect(mem_button, SIGNAL(clicked()), this, SLOT(ClickMemButton()));
    connect(net_button, SIGNAL(clicked()), this, SLOT(ClickNetButton()));
    connect(gpu_button, SIGNAL(clicked()), this, SLOT(ClickGpuButton()));
    return widget;
}

QWidget* MonitorWidget::InitCpuMonitorWidget() {
    QWidget* widget = new QWidget();
    // ���ñ�ǩ
    QLabel* cpu_load_label = new QLabel(this);
    cpu_load_label->setText(tr("Monitor CpuLoad:"));
    cpu_load_label->setFont(QFont("Microsoft YaHei", 10, 40));

    // ���ñ��
    cpu_load_monitor_view_ = new QTableView;
    // �������ģ��
    cpu_load_model_ = new CpuLoadModel;
    cpu_load_monitor_view_->setModel(cpu_load_model_);
    cpu_load_monitor_view_->show();

    // ���ñ�ǩ
    QLabel* cpu_stat_label = new QLabel(this);
    cpu_stat_label->setText(tr("Monitor CpuStat:"));
    cpu_stat_label->setFont(QFont("Microsoft YaHei", 10, 40));

    // ���ñ��
    cpu_stat_monitor_view_ = new QTableView;
    // �������ģ��
    cpu_stat_model_ = new CpuStatModel;
    cpu_stat_monitor_view_->setModel(cpu_stat_model_);
    cpu_stat_monitor_view_->show();

    // ������״ͼ
    cpu_load_bar = new CPUlaodBar();
    QWidget* cpu_load_bar_w = cpu_load_bar->CPUlaod_BarInit();
    cpu_stat_bar = new CPUstatBar();
    QWidget* cpu_stat_bar_w = cpu_stat_bar->CPUstat_BarInit();

    // ���ø��Բ���
    QGridLayout* layout = new QGridLayout();

    // �����1��2��ʾ���ڴ�С
    layout->addWidget(cpu_stat_label, 0, 0);
    layout->addWidget(cpu_stat_monitor_view_, 1, 0, 1, 1);
    layout->addWidget(cpu_load_label, 2, 0);
    layout->addWidget(cpu_load_monitor_view_, 3, 0, 1, 1);
    layout->addWidget(cpu_stat_bar_w, 0, 1, 2, 1);
    layout->addWidget(cpu_load_bar_w, 2, 1, 2, 1);

    widget->setLayout(layout);
    return widget;
}

QWidget* MonitorWidget::InitGpuMonitorWidget() {
    QWidget* widget = new QWidget();
    // ���ñ�ǩ
    QLabel* gpu_info_label = new QLabel(this);
    gpu_info_label->setText(tr("Monitor Gpu_Info:"));
    gpu_info_label->setFont(QFont("Microsoft YaHei", 10, 40));

    // ���ñ��
    gpu_monitor_view_ = new QTableView;
    // �������ģ��
    gpu_model_ = new GpuInfoModel;

    gpu_monitor_view_->setModel(gpu_model_);
    gpu_monitor_view_->show();

    // ���ø��Բ���
    QGridLayout* layout = new QGridLayout();

    // �����1��2��ʾ���ڴ�С
    layout->addWidget(gpu_info_label, 0, 0);
    layout->addWidget(gpu_monitor_view_, 1, 0);

    widget->setLayout(layout);
    return widget;
}

QWidget* MonitorWidget::InitSoftIrqMonitorWidget() {
    // �½�һ������
    QWidget* widget = new QWidget();
    // ���ñ�ǩ
    QLabel* monitor_label = new QLabel(this);
    monitor_label->setText(tr("Monitor softirq:"));
    monitor_label->setFont(QFont("Microsoft YaHei", 10, 40));

    // ���ñ��
    monitor_view_ = new QTableView;
    monitor_model_ = new MonitorBaseModel;
    // sort_proxyģ�Ϳ��Զ����ݽ�������͹���
    QSortFilterProxyModel* sort_proxy = new QSortFilterProxyModel(this);
    // ʹģ�;����Զ�����Ĺ���
    sort_proxy->setSourceModel(monitor_model_);
    // �������������ģ��
    monitor_view_->setModel(sort_proxy);
    // �����Զ�������
    monitor_view_->setSortingEnabled(true);
    monitor_view_->show();

    // ���뵽���Բ�����
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(monitor_label, 1, 0);
    layout->addWidget(monitor_view_, 2, 0, 1, 2);

    widget->setLayout(layout);
    return widget;
}

QWidget* MonitorWidget::InitMemMonitorWidget() {
    QWidget* widget = new QWidget();

    QLabel* mem_label = new QLabel();
    mem_label->setText(tr("Monitor mem:"));
    mem_label->setFont(QFont("Microsoft YaHei", 10, 40));

    mem_monitor_view_ = new QTableView;
    mem_model_ = new MemModel;
    mem_monitor_view_->setModel(mem_model_);
    mem_monitor_view_->show();

    mem_pie = new MemPie();
    QWidget* mem_pie_w = mem_pie->MemPie_Init();

    QGridLayout* layout = new QGridLayout();

    layout->addWidget(mem_label, 0, 0);
    layout->addWidget(mem_monitor_view_, 1, 0, 1, 1);
    layout->addWidget(mem_pie_w, 1, 1, 1, 1);

    // layout->setRowStretch(1, 1);
    // layout->setRowStretch(2, 2);

    widget->setLayout(layout);
    return widget;
}

QWidget* MonitorWidget::InitNetMonitorWidget() {
    QWidget* widget = new QWidget();

    QLabel* net_label = new QLabel(this);
    net_label->setText(tr("Monitor net:"));
    net_label->setFont(QFont("Microsoft YaHei", 10, 40));

    net_monitor_view_ = new QTableView;
    net_model_ = new NetModel;
    net_monitor_view_->setModel(net_model_);
    net_monitor_view_->show();

    QGridLayout* layout = new QGridLayout();

    layout->addWidget(net_label, 1, 0);
    layout->addWidget(net_monitor_view_, 2, 0, 1, 1);

    widget->setLayout(layout);
    return widget;
}

// ��������
void MonitorWidget::UpdateData(const monitor::proto::MonitorInfo& monitor_info) {
    monitor_model_->UpdateMonitorInfo(monitor_info);
    cpu_load_model_->UpdateMonitorInfo(monitor_info);
    
    // ��������ֶδ���
    cpu_stat_model_->UpdateMonitorInfo(monitor_info);

    mem_model_->UpdateMonitorInfo(monitor_info);
    net_model_->UpdateMonitorInfo(monitor_info);

    gpu_model_->UpdateMonitorInfo(monitor_info);
    mem_pie->UpdateMemChart(monitor_info);

    cpu_load_bar->UpdateCPUloadBar(monitor_info);
    cpu_stat_bar->UpdateCPUstatBar(monitor_info);
}

// ջ���л�
void MonitorWidget::ClickCpuButton() { stack_menu_->setCurrentIndex(0); }
void MonitorWidget::ClickSoftIrqButton() { stack_menu_->setCurrentIndex(1); }
void MonitorWidget::ClickMemButton() { stack_menu_->setCurrentIndex(2); }
void MonitorWidget::ClickNetButton() { stack_menu_->setCurrentIndex(3); }
void MonitorWidget::ClickGpuButton() { stack_menu_->setCurrentIndex(4); }

}  // namespace monitor
