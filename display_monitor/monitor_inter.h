#ifndef MONITOR_INTERH_H
#define MONITOR_INTERH_H

#include <QAbstractTableModel>
#include <QObject>

namespace monitor {
class MonitorInterModel : public QAbstractTableModel {
    //    �ඨ�������Q_OBJECT�꣬��������ʹ���ź���ۻ���
    Q_OBJECT;

   public:
    explicit MonitorInterModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent) {}
    //ʹ����override����������ô�ú�����������������е�ͬ������
    // ��ȡindex����������
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    // ��ȡ��ͷ����
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;

    virtual ~MonitorInterModel() {}
};
}  // namespace monitor

#endif  // MONITOR_INTERH_H
