#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"
#include "tasks.h"
#include <QRadioButton>

Form::Form() {}

Form::Form(QWidget *parent, QString text) : QWidget(parent), ui(new Ui::Form) {
    ui->setupUi(this);
    ui->label->setText(text);
    ui->label->setToolTip(text);
    ui->label->setWordWrap(true);
    this->adjustSize();
}

Form::~Form() { delete ui; }

void Form::on_editButton_clicked() {
    auto widget = new Dialog(this, ui->label->text());
    int ret = widget->exec();
    if (ret == QDialog::Rejected)
        return;
    if (ret) {
        ui->label->setText(widget->edited);
        ui->label->setToolTip(ui->label->text());
    }
}

void Form::on_deleteButton_clicked() {
    emit deleted();
    this->close();
}

void Form::on_downButton_clicked() {
    auto formsList = this->parent()->findChildren<QLabel *>();
    Tasks::tasks.clear();
    for (auto &&label : formsList)
        Tasks::tasks.append(label->text());
    QString chore = ui->label->text();
    int i = Tasks::tasks.indexOf(chore);
    if (i < Tasks::tasks.size()-1)
        #if QT_VERSION >= 0x050E02
            Tasks::tasks.swapItemsAt(i, i+1);
        #else
            Tasks::tasks.swap(i, i+1);
        #endif
    emit valueChanged();
}

void Form::on_upButton_clicked() {
    auto formsList = this->parent()->findChildren<QLabel *>();
    Tasks::tasks.clear();
    for (auto &&label : formsList)
        Tasks::tasks.append(label->text());
    QString chore = ui->label->text();
    int i = Tasks::tasks.indexOf(chore);
    if (i)
        #if QT_VERSION >= 0x050E02
            Tasks::tasks.swapItemsAt(i, i-1);
        #else
            Tasks::tasks.swap(i, i-1);
        #endif
    emit valueChanged();
}

void Form::on_radioButton_clicked() {
    Tasks::completed.append(ui->label->text());
    this->close();
}
