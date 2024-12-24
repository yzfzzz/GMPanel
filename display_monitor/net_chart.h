#ifndef net_chart_H
#define net_chart_H

#include <QList>
#include <QWidget>

#include <QAreaSeries>
#include <QChart>
#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QScatterSeries>
#include <QSplineSeries>
#include <string>
#include <iostream>
#include <QtCore/QPointF>
using namespace QtCharts;

class line_node {
   public:
    float value;
    QDateTime time;
};

class net_chart : public QWidget {
    Q_OBJECT

   public:
    explicit net_chart(QWidget *parent = 0);
    ~net_chart();
    /**
     * 接收到数据源发送来的数据，数据源可以下位机，采集卡，传感器等。
     */
    void drawChart(QList<QPointF> data_send, QList<QPointF> data_recv);

   private:
    int maxX;
    int maxY;

    QChart *chart;
    QChartView *chartView;
    QSplineSeries *splineSeries_send;
    QSplineSeries *splineSeries_recv;

    QDateTimeAxis *axisX;
};

#endif  // net_chart_H
