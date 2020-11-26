#ifndef COMBO_H
#define COMBO_H

#include <QDialog>

namespace Ui { class Combo; }

class Combo : public QDialog {
    Q_OBJECT

public:
    explicit Combo(QWidget *parent, QStringList completed);
    ~Combo();
    QString text;
    int p;

signals:
    void deleted();
    void restored();

private slots:
    void on_delButton_clicked();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_restButton_clicked();

private:
    Ui::Combo *ui;
};

#endif // COMBO_H
