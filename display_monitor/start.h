#ifndef START_H
#define START_H

#include <widget.h>
#include <QComboBox>
#include <QWidget>
#include <string>
#include <unordered_map>


namespace Ui {
class start;
}

class Start : public QWidget {
    Q_OBJECT

   public:
    explicit Start(int argc, char** argv, QWidget* parent = nullptr);
    ~Start();
    std::unordered_map<std::string, Widget*> monitor_map;

   private:
    Ui::start* ui;
};

#endif  // START_H
