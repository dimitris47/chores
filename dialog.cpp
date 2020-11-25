#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent, QString data) : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);
    ui->lineEdit->setText(data);
}

Dialog::~Dialog() { delete ui; }

void Dialog::on_lineEdit_returnPressed() {
    edited = ui->lineEdit->text();
}

void Dialog::on_buttonBox_accepted() {
    edited = ui->lineEdit->text();
}
