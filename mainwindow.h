#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QSettings>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStringList choreList;
    QStringList complTasks;

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void doUpdates();
    void moveCompleted();
    void permDelete();
    void restoreDeleted();

private slots:
    void on_actionAbout_triggered();
    void on_actionExport_triggered();
    void on_actionShow_Completed_triggered();
    void on_addButton_clicked();
    void on_lineEdit_returnPressed();
    void readSettings();

signals:
    void formClosed();

private:
    Ui::MainWindow *ui;
    int lines;
    QStringList tasks;
    QVBoxLayout *layout = new QVBoxLayout;
};

#endif // MAINWINDOW_H
