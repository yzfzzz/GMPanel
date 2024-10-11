#include <random>
#include "cpu_load_barchart.h"

static const QStringList list_bar_state = {
    "AVG_1",
    "AVG_3",
    "AVG_15",
};

QWidget* CPUlaodBar::CPUlaod_BarInit() {
    // QBarSeries �ཫһϵ��������ʾΪ��������Ĵ�ֱ����
    this->series = new QHorizontalBarSeries();
    // QBarSet ���ʾ����ͼ�е�һ������
    this->set1 = new QBarSet("AVG_1");
    this->set3 = new QBarSet("AVG_3");
    this->set15 = new QBarSet("AVG_15");

    *this->set1 << 0.1;
    *this->set3 << 0.3;
    *this->set15 << 0.8;

    this->series->append(this->set1);
    this->series->append(this->set3);
    this->series->append(this->set15);

    this->chart = new QChart();  // ��ȡQChartView��Ĭ�ϵ�QChart

    this->chart->addSeries(this->series);  // �������õ�����ͼseries��ӽ�chart��
    // this->chart->setTitle("Short-term load rate");  // ���ñ���
    this->chart->setAnimationOptions(QChart::SeriesAnimations);  // ����ͼ��仯ʱ�Ķ���Ч��


    QValueAxis* axisY = new QValueAxis();  // ����Y��
    axisY->setRange(0, 1);                 // ����Y�᷶Χ��ѧϰ������Ȼ��24Сʱඣ�
    this->chart->addAxis(axisY, Qt::AlignBottom);  // Y�������
    this->series->attachAxis(axisY);

    this->chart->setTheme(QChart::ChartThemeBlueCerulean);
    this->chart->legend()->setAlignment(Qt::AlignRight);

    this->chartView = new QChartView(this->chart);
    this->chartView->setRenderHint(QPainter::Antialiasing);  // ���ÿ������Ⱦ��ʾ

    this->layout = new QVBoxLayout();
    this->layout->addWidget(this->chartView);  // ��QChartView��ӵ�������

    QWidget* widget = new QWidget();
    widget->setLayout(this->layout);
    return widget;
}

void CPUlaodBar::UpdateCPUloadBar(const monitor::proto::MonitorInfo& monito_info) {
    this->set1->replace(0, monito_info.cpu_load().load_avg_1());
    this->set3->replace(0, monito_info.cpu_load().load_avg_3());
    this->set15->replace(0, monito_info.cpu_load().load_avg_15());
}
