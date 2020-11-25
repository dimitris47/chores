#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui { class Dialog; }

class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent, QString data);
    ~Dialog();
    QString edited;

private slots:
    void on_buttonBox_accepted();
    void on_lineEdit_returnPressed();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
