#include "net_chart.h"

NetChart::NetChart(QWidget *parent) : QWidget(parent) {
    max_y_ = 1200;

    spline_series_send_ = new QSplineSeries();
    QPen line_pen_send(QColor("#05D0D7"), 2, Qt::SolidLine, Qt::RoundCap,
                       Qt::RoundJoin);
    spline_series_send_->setPen(line_pen_send);

    spline_series_recv_ = new QSplineSeries();
    QPen line_pen_recv(QColor("#DB901E"), 2, Qt::SolidLine, Qt::RoundCap,
                       Qt::RoundJoin);
    spline_series_recv_->setPen(line_pen_recv);

    spline_series_send_->setName("上传");
    spline_series_recv_->setName("下载");

    chart_ = new QChart();
    chart_->addSeries(spline_series_send_);
    chart_->addSeries(spline_series_recv_);

    chart_->legend()->hide();
    chart_->setTitle("流量收发情况");
    chart_->createDefaultAxes();

    axis_x_ = new QDateTimeAxis();
    axis_x_->setFormat("mm:ss");
    axis_x_->setTickCount(5);

    if (chart_->axisX()) {
        chart_->removeAxis(chart_->axisX());
    }

    chart_->setAxisX(axis_x_);
    spline_series_send_->attachAxis(axis_x_);
    spline_series_recv_->attachAxis(axis_x_);

    chart_->axisY()->setRange(0, max_y_);

    // 隐藏网格线
    if (chart_->axisX()) {
        chart_->axisX()->setGridLineVisible(false);
    }
    if (chart_->axisY()) {
        chart_->axisY()->setGridLineVisible(false);
    }
    chart_->legend()->setVisible(true);

    chart_view_ = new QChartView(chart_);
    chart_view_->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chart_view_);
    setLayout(layout);
}

NetChart::~NetChart() {}

void NetChart::drawChart(QList<QPointF> data_send, QList<QPointF> data_recv) {
    if (isVisible()) {
        spline_series_send_->replace(data_send);
        spline_series_recv_->replace(data_recv);
        axis_x_->setRange(
            QDateTime::fromMSecsSinceEpoch(data_send.begin()->x()),
            QDateTime::fromMSecsSinceEpoch(data_send.rbegin()->x()));
        std::cout << "NetChart::drawChart: start "
                  << QDateTime::fromMSecsSinceEpoch(data_send.begin()->x())
                         .toString()
                         .toStdString()
                  << " end "
                  << QDateTime::fromMSecsSinceEpoch(data_send.rbegin()->x())
                         .toString()
                         .toStdString()
                  << std::endl;
    }
}
