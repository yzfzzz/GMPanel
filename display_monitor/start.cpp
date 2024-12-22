#include <iostream>
#include "start.h"
#include "ui_start.h"

Start::Start(int argc, char** argv, QWidget* parent)
    : QWidget(parent), ui(new Ui::start) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    connect(
        ui->login_w, &Login::loginJump, this,
        [=](std::string account_num,
            std::vector<std::string> machine_name_array) {
            if (machine_name_array.size() == 0) {
                machine_name_array.push_back("No machine!!!");
            }
            for (int i = 0; i < machine_name_array.size(); i++) {
                std::cout << "Run Start func: " << machine_name_array[i]
                          << std::endl;
                monitor_map[machine_name_array[i]] =
                    new Widget(argc, argv, account_num, machine_name_array, i);
                connect(
                    this->monitor_map[machine_name_array[i]]->machine_comboBox,
                    &QComboBox::currentTextChanged, this,
                    [=](const QString& text) {
                        ui->stackedWidget->setCurrentWidget(
                            monitor_map[text.toStdString()]);
                        for (int j = 0; j < machine_name_array.size(); j++) {
                            this->monitor_map[machine_name_array[j]]
                                ->machine_comboBox->setCurrentText(text);
                        }
                    });
                ui->stackedWidget->addWidget(
                    monitor_map[machine_name_array[i]]);
            }

            ui->stackedWidget->setCurrentWidget(
                monitor_map[machine_name_array[0]]);
        });
    // ui->stackedWidget->setCurrentIndex(2);
}

Start::~Start() { delete ui; }
