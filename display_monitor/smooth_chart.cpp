#include <QDateTime>
#include <QHBoxLayout>
#include "smooth_chart.h"

smooth_chart::smooth_chart(QWidget *parent) : QWidget(parent) {
    maxY = 100;

    splineSeries = new QSplineSeries();
    QPen linePen(QColor("#BB30E2"), 2, Qt::SolidLine, Qt::RoundCap,
                 Qt::RoundJoin);
    splineSeries->setPen(linePen);

    chart = new QChart();
    chart->addSeries(splineSeries);

    chart->legend()->hide();
    chart->setTitle("GPU平均使用率");
    chart->createDefaultAxes();

    axisX = new QDateTimeAxis();
    axisX->setFormat("mm:ss");
    axisX->setTickCount(5);

    if (chart->axisX()) {
        chart->removeAxis(chart->axisX());
    }

    chart->setAxisX(axisX);
    splineSeries->attachAxis(axisX);

    chart->axisY()->setRange(0, maxY);
    chart->setBackgroundVisible(false);

    if (chart->axisX()) {
        chart->axisX()->setGridLineVisible(false);  // 隐藏X轴网格线
    }
    if (chart->axisY()) {
        chart->axisY()->setGridLineVisible(false);  // 隐藏Y轴网格线
    }

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    setLayout(layout);
}

smooth_chart::~smooth_chart() {}

void smooth_chart::drawChart(QList<QPointF> data) {
    // 界面被隐藏后就没有必要绘制数据的曲线了
    if (isVisible()) {
        splineSeries->replace(data);

        axisX->setRange(QDateTime::fromMSecsSinceEpoch(data.begin()->x()),
                        QDateTime::fromMSecsSinceEpoch(data.rbegin()->x()));
    }
}
