#ifndef FORM_H
#define FORM_H

#include "dialog.h"
#include <QWidget>

namespace Ui { class Form; }

class Form : public QWidget {
    Q_OBJECT

public:
    explicit Form();
    explicit Form(QWidget *parent, QString text);
    ~Form();
    QStringList choresList;
    QStringList completed;

signals:
    void valueChanged();
    void taskCompleted();

private slots:
    void on_deleteButton_clicked();
    void on_downButton_clicked();
    void on_editButton_clicked();
    void on_radioButton_clicked();
    void on_upButton_clicked();

private:
    Ui::Form *ui;
};

#endif // FORM_H
