#ifndef smooth_chart_H
#define smooth_chart_H

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


using namespace QtCharts;

class gpu_line_node {
   public:
    int value;
    QDateTime time;
};

class smooth_chart : public QWidget {
    Q_OBJECT

   public:
    explicit smooth_chart(QWidget *parent = 0);
    ~smooth_chart();
    void drawChart(QList<QPointF> data);

   private:
    int maxX;
    int maxY;

    QChart *chart;
    QChartView *chartView;
    QSplineSeries *splineSeries;
    QScatterSeries *scatterSeries;
    QDateTimeAxis *axisX;
};

#endif  // smooth_chart_H
