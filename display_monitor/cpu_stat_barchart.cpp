#include <random>
#include "cpu_stat_barchart.h"

static const QStringList list_bar_state = {
    "AVG_1",
    "AVG_3",
    "AVG_15",
};

QWidget* CPUstatBar::CPUstat_BarInit() {
    // QBarSeries �ཫһϵ��������ʾΪ��������Ĵ�ֱ����
    this->series = new QBarSeries();
    // QBarSet ���ʾ����ͼ�е�һ������
    this->cpu_percent_set = new QBarSet("cpu");
    this->user_set = new QBarSet("user");
    this->sys_set = new QBarSet("system");

    *this->cpu_percent_set << 1 << 5 << 10;
    *this->user_set << 3 << 6 << 4;
    *this->sys_set << 9 << 7 << 2;

    this->series->append(this->cpu_percent_set);
    this->series->append(this->user_set);
    this->series->append(this->sys_set);

    this->chart = new QChart();  // ��ȡQChartView��Ĭ�ϵ�QChart

    this->chart->addSeries(this->series);  // �������õ�����ͼseries��ӽ�chart��
    // this->chart->setTitle("cpu current load rate");               // ���ñ���
    this->chart->setAnimationOptions(QChart::SeriesAnimations);  // ����ͼ��仯ʱ�Ķ���Ч��

    QStringList x_label;                                // X����࣬һ����QBarSet����ӵ����ݸ�����ͬ��������������ʾ��ȫ,���˲�Ӱ�죬���ǲ����ظ�
    x_label << "cpu" << "cpu0"  << "cpu1";  // �����쵱Ȼ����Ϣ��...

    QBarCategoryAxis* axisX = new QBarCategoryAxis();  // QBarCategoryAxis����ͼ�����������
    axisX->append(x_label);
    
    this->chart->addAxis(axisX, Qt::AlignBottom);  // ��X�����ͼ��ĵײ�
    this->series->attachAxis(axisX);
    QValueAxis* axisY = new QValueAxis();  // ����Y��
    axisY->setRange(0, 10);                 // ����Y�᷶Χ
    // axisY->setTitleText("load ratio");   // ����Y�����
    this->chart->addAxis(axisY, Qt::AlignLeft);  // Y�������
    this->series->attachAxis(axisY);

    this->chartView = new QChartView(this->chart);
    this->chartView->setRenderHint(QPainter::Antialiasing);  // ���ÿ������Ⱦ��ʾ

    this->layout = new QVBoxLayout();
    this->layout->addWidget(this->chartView);  // ��QChartView��ӵ�������

    QWidget* widget = new QWidget();
    widget->setLayout(this->layout);
    return widget;
}

void CPUstatBar::UpdateCPUstatBar(const monitor::proto::MonitorInfo& monito_info) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 7.0); // ���� 0 �� 100֮������������ 
    
    float randomNumber = dis(gen);
    this->cpu_percent_set->replace(0, randomNumber);
    this->user_set->replace(1, randomNumber+2);
    this->sys_set->replace(2, 10-randomNumber);
}
