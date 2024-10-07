#include "mem_piechart.h"
#include <random>

//�����������Ƭ����ɫ
static const QStringList list_pie_color = {
    "#6480D6","#A1DC85","#FFAD25","#FF7777","#84D1EF","#4CB383",
};

static const QStringList list_pie_state = {
"Free", "Used",
};

PieWidget::PieWidget(QWidget *parent)
    : QWidget(parent)
    
{
    QVector<qreal> data(2,50);
    series = new QPieSeries(this);            // ����һ����ͼ�������ÿ׾�����Բ����
    for(int i = 0; i < data.size(); i++)
    {
        QPieSlice* pie_slice = new QPieSlice(this);
        pie_slice->setLabelVisible(true);
        pie_slice->setValue(data[i]);
        pie_slice->setLabel(list_pie_state[i]);
        pie_slice->setColor(list_pie_color[i]);
        pie_slice->setLabelColor(list_pie_color[i]);
        pie_slice->setBorderColor(list_pie_color[i]);
        series->append(pie_slice);
    }

    chart = new QChart();           // ��ȡQChartView��Ĭ�ϵ�QChart
    chart->setTitle("Memory Usage");                      // ����ͼ������
    chart->setTheme(QChart::ChartThemeDark);        //���ð�������
    chart->addSeries(series);                        // �������õı�ͼ�������ӽ�QChart

    //ͼ��
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(false);


    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // ���ÿ������Ⱦ��ʾ

    layout = new QVBoxLayout(this);
    layout->addWidget(chartView); // ��QChartView���ӵ�������

}

void PieWidget::UpdateMemChart(const monitor::proto::MonitorInfo& monitor_info)
{
    QVector<qreal> data;
    data.push_back(monitor_info.mem_info().used_percent());
    data.push_back(100-monitor_info.mem_info().used_percent());
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_real_distribution<float> dis(0.0, 100.0); // ���� 0 �� 100 ֮������������
    // float randomNumber = dis(gen);
    // data.push_back(randomNumber);
    // data.push_back(100-randomNumber);
    for(int i = 0; i < data.size(); i++)
    {
        series->slices().at(i)->setValue(data[i]);
    }

    chart->update();  // ˢ��ͼ��
}

