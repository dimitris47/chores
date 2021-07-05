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
