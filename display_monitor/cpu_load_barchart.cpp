#include "cpu_load_barchart.h"
#include <random>


static const QStringList list_bar_state = {
"AVG_1", "AVG_3", "AVG_15",
};

QWidget* CPUlaod_BarWidget::CPUlaod_BarInit()
{
    // QBarSeries �ཫһϵ��������ʾΪ��������Ĵ�ֱ����
    series = new QBarSeries();
       // QBarSet ���ʾ����ͼ�е�һ������
    set1 = new QBarSet("AVG_1");
    set3 = new QBarSet("AVG_3");
    set15 = new QBarSet("AVG_15");

    *set1 << 0.1;
    *set3 << 0.3;
    *set15 << 0.8;

    series->append(set1);
    series->append(set3);
    series->append(set15);


    chart = new QChart();           // ��ȡQChartView��Ĭ�ϵ�QChart

    chart->addSeries(series);                              // �������õ�����ͼseries��ӽ�chart��
    chart->setTitle("Short-term load rate");               // ���ñ���
    chart->setAnimationOptions(QChart::SeriesAnimations);  // ����ͼ��仯ʱ�Ķ���Ч��

    QBarCategoryAxis* axisX = new QBarCategoryAxis();      // QBarCategoryAxis����ͼ�����������
    chart->addAxis(axisX, Qt::AlignBottom);                // ��X�����ͼ��ĵײ�
    series->attachAxis(axisX);
    QValueAxis* axisY = new QValueAxis();     // ����Y��
    axisY->setRange(0, 1);                   // ����Y�᷶Χ��ѧϰ������Ȼ��24Сʱඣ�
    axisY->setTitleText("Average load ratio");  // ����Y�����
    chart->addAxis(axisY, Qt::AlignLeft);     // Y�������
    series->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // ���ÿ������Ⱦ��ʾ

    layout = new QVBoxLayout();
    layout->addWidget(chartView); // ��QChartView��ӵ�������
    
    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    return widget;
}

void CPUlaod_BarWidget::UpdateCPUloadChart(const monitor::proto::MonitorInfo& monitor_info)
{
    set1->replace(0, monitor_info.cpu_load().load_avg_1());
    set3->replace(0, monitor_info.cpu_load().load_avg_3());
    set15->replace(0, monitor_info.cpu_load().load_avg_15());
    series->clear();
    series->append(set1);
    series->append(set3);
    series->append(set15);

    // chart->update();  // ˢ��ͼ��
}





