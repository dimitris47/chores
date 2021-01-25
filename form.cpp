#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"
#include <QRadioButton>

Form::Form() {}

Form::Form(QWidget *parent, QString text) : QWidget(parent), ui(new Ui::Form) {
    ui->setupUi(this);
    ui->label->setText(text);
}

Form::~Form() { delete ui; }

void Form::on_editButton_clicked() {
    auto widget = new Dialog(this, ui->label->text());
    int ret = widget->exec();
    if (ret == QDialog::Rejected)
        return;
    if (ret)
        ui->label->setText(widget->edited);
}

void Form::on_deleteButton_clicked() {
    this->close();
}

void Form::on_downButton_clicked() {
    auto formsList = this->parent()->findChildren<QLabel *>();
    for (auto label : formsList)
        choresList.append(label->text());
    QString chore = ui->label->text();
    int i = choresList.indexOf(chore);
    if (i < choresList.size()-1)
        #if QT_VERSION >= 0x050E02
            choresList.swapItemsAt(i, i+1);
        #else
            choresList.swap(i, i+1);
        #endif
    emit valueChanged();
    choresList.clear();
}

void Form::on_upButton_clicked() {
    auto formsList = this->parent()->findChildren<QLabel *>();
    for (auto label : formsList)
        choresList.append(label->text());
    QString chore = ui->label->text();
    int i = choresList.indexOf(chore);
    if (i)
        #if QT_VERSION >= 0x050E02
            choresList.swapItemsAt(i, i-1);
        #else
            choresList.swap(i, i-1);
        #endif
    emit valueChanged();
    choresList.clear();
}

void Form::on_radioButton_clicked() {
    completed.append(ui->label->text());
    emit taskCompleted();
    this->close();
}
