/**
 * Copyright 2020-2021 Dimitris Psathas <dimitrisinbox@gmail.com>
 *
 * This file is part of Chores.
 *
 * Chores is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License  as  published by  the  Free Software
 * Foundation,  either version 3 of the License,  or (at your option)  any later
 * version.
 *
 * Chores is distributed in the hope that it will be useful,  but  WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the  GNU General Public License  for more details.
 *
 * You should have received a copy of the  GNU General Public License along with
 * Chores. If not, see <http://www.gnu.org/licenses/>.
 */


#include "combo.h"
#include "ui_combo.h"

Combo::Combo(QWidget *parent, QStringList completed) : QDialog(parent), ui(new Ui::Combo) {
    ui->setupUi(this);
    ui->listWidget->addItems(completed);
    ui->listWidget->setCurrentRow(0);
}

Combo::~Combo() { delete ui; }

void Combo::on_listWidget_currentRowChanged(int currentRow) {
    ui->listWidget->setCurrentRow(currentRow);
}

void Combo::on_restButton_clicked() {
    if (ui->listWidget->count() == 0)
        return;
    else {
        text = ui->listWidget->currentItem()->text();
        p = ui->listWidget->currentRow();
        QListWidgetItem *item = ui->listWidget->takeItem(p);
        delete item;
        emit restored();
    }
}

void Combo::on_delButton_clicked() {
    if (ui->listWidget->count() == 0)
        return;
    else {
        text = ui->listWidget->currentItem()->text();
        p = ui->listWidget->currentRow();
        QListWidgetItem *item = ui->listWidget->takeItem(p);
        delete item;
        emit deleted();
    }
}
