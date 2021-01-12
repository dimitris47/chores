#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QSettings>
#include <QSystemTrayIcon>
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
    void on_exit();
    void on_lineEdit_returnPressed();
    void on_show_hide();
    void on_toggle_show(QSystemTrayIcon::ActivationReason r);

private:
    Ui::MainWindow *ui;
    int lines;
    int restored;
    QStringList tasks;
    QVBoxLayout *layout;
    void createTrayIcon();
    void readSettings();
};

#endif // MAINWINDOW_H
