#ifndef WIDGET_H
#define WIDGET_H

#include <QString>
#include <QWidget>
#include <mutex>
#include "midinfo.h"
#include "water_progress.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

   public:
    Widget(int argc, char **argv,QWidget *parent = nullptr);
    ~Widget();
    void Update(std::vector<monitor::MidInfo> &midinfo_array);

   public:
    void recv_label_down_str(QVector<QString> qstr_array);
    void recv_label_down_qss(QVector<QString> qss_array);
    void recv_water_value(QVector<int> water_array);
    void recv_gpu_infos(QVector<QString> gpu_infos_array);
    void recv_net_data(QList<QPointF> send_list, QList<QPointF> recv_list);
    void recv_gpu_list(QList<QPointF> gpu_list);
    signals:
    void starting(int argc, char **argv);

   private:
    Ui::Widget *ui;
};
#endif  // WIDGET_H
