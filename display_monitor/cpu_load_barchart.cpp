#include <random>
#include "cpu_load_barchart.h"

static const QStringList list_bar_state = {
    "AVG_1",
    "AVG_3",
    "AVG_15",
};

QWidget* CPUlaodBar::CPUlaod_BarInit(){
    // QBarSeries �ཫһϵ��������ʾΪ��������Ĵ�ֱ����
    this->series = new QBarSeries();
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

    this->chart->addSeries(
        this->series);  // �������õ�����ͼseries��ӽ�chart��
    this->chart->setTitle("Short-term load rate");  // ���ñ���
    this->chart->setAnimationOptions(
        QChart::SeriesAnimations);  // ����ͼ��仯ʱ�Ķ���Ч��

    QBarCategoryAxis* axisX =
        new QBarCategoryAxis();  // QBarCategoryAxis����ͼ�����������
    this->chart->addAxis(axisX, Qt::AlignBottom);  // ��X�����ͼ��ĵײ�
    this->series->attachAxis(axisX);
    QValueAxis* axisY = new QValueAxis();  // ����Y��
    axisY->setRange(0, 1);  // ����Y�᷶Χ��ѧϰ������Ȼ��24Сʱඣ�
    axisY->setTitleText("Average load ratio");   // ����Y�����
    this->chart->addAxis(axisY, Qt::AlignLeft);  // Y�������
    this->series->attachAxis(axisY);

    this->chartView = new QChartView(this->chart);
    this->chartView->setRenderHint(
        QPainter::Antialiasing);  // ���ÿ������Ⱦ��ʾ

    this->layout = new QVBoxLayout();
    this->layout->addWidget(this->chartView);  // ��QChartView��ӵ�������

    QWidget* widget = new QWidget();
    widget->setLayout(this->layout);
    return widget;
}

void CPUlaodBar::UpdateCPUloadBar(const monitor::proto::MonitorInfo &monito_info)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0); // ���� 0 �� 100֮������������ float randomNumber = dis(gen);

    this->set1->replace(0,dis(gen));
    this->set3->replace(0,dis(gen));
    this->set15->replace(0,dis(gen));
}
