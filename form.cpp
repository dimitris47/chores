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
        emit taskEdited();
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

void Form::on_pushButton_clicked() {
    Tasks::completed.append(ui->label->text());
    Tasks::tasks.removeOne(ui->label->text());
    this->close();
}

