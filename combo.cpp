#include "combo.h"
#include "ui_combo.h"

Combo::Combo(QWidget *parent, QStringList completed) : QDialog(parent), ui(new Ui::Combo) {
    ui->setupUi(this);
    ui->listWidget->addItems(completed);
}

Combo::~Combo() { delete ui; }

void Combo::on_restButton_clicked() {
    text = ui->listWidget->currentItem()->text();
    p = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->takeItem(p);
    delete item;
    emit restored();
}

void Combo::on_delButton_clicked() {
    text = ui->listWidget->currentItem()->text();
    p = ui->listWidget->currentRow();
    QListWidgetItem *item = ui->listWidget->takeItem(p);
    delete item;
    emit deleted();
}
