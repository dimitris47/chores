#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "combo.h"
#include "form.h"
#include <QActionGroup>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPrinter>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    layout = new QVBoxLayout;
    ui->frame->setLayout(layout);
    layout->setAlignment(Qt::AlignTop);
    readSettings();
    createTrayIcon();
}

MainWindow::~MainWindow() { delete ui; }

QString dirToWrite() {
    static QString dir;
    if (!dir.isEmpty())
        return dir;
    QStringList locations = (QStringList()
                             << QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
                             << QStandardPaths::standardLocations(QStandardPaths::HomeLocation));
    for (const QString & loc : locations)
        if (QFileInfo::exists(loc)) {
            dir = loc;
            return dir;
        }
    return QString();
}

void MainWindow::on_addButton_clicked() {
    QString text = ui->lineEdit->text();
    if (text != "") {
        auto widget = new Form(ui->frame, text);
        connect(widget, &Form::valueChanged, this, &MainWindow::doUpdates);
        connect(widget, &Form::taskCompleted, this, &MainWindow::moveCompleted);
        tasks.append(text);
        widget->setObjectName(QString::fromUtf8("formItem") + QString::number(lines));
        widget->setAttribute(Qt::WA_DeleteOnClose);
        layout->addWidget(widget);
        widget->show();
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
        lines++;
    }
}

void MainWindow::on_lineEdit_returnPressed() { on_addButton_clicked(); }

void MainWindow::doUpdates() {
    Form *f = qobject_cast<Form *>(sender());
    tasks.clear();
    choreList = f->choresList;
    for (QString chore : choreList)
        tasks.append(chore);
    auto labelList = ui->frame->findChildren<QLabel *>();
    int i {0};
    for (auto label : labelList) {
        label->setText(tasks[i]);
        i++;
    }
}

void MainWindow::moveCompleted() {
    Form *f = qobject_cast<Form *>(sender());
    complTasks.append(f->completed);
}

void MainWindow::permDelete() {
    Combo *c = qobject_cast<Combo *>(sender());
    complTasks.removeAt(c->p);
}

void MainWindow::restoreDeleted() {
    Combo *c = qobject_cast<Combo *>(sender());
    complTasks.removeAt(c->p);
    auto widget = new Form(ui->frame, QString(c->text));
    connect(widget, &Form::valueChanged, this, &MainWindow::doUpdates);
    connect(widget, &Form::taskCompleted, this, &MainWindow::moveCompleted);
    widget->setObjectName(QString::fromUtf8("restItem") + QString::number(restored));
    widget->setAttribute(Qt::WA_DeleteOnClose);
    layout->addWidget(widget);
    widget->show();
    restored++;
}

void MainWindow::on_actionShow_Completed_triggered() {
    auto widget = new Combo(this, complTasks);
    connect(widget, &Combo::deleted, this, &MainWindow::permDelete);
    connect(widget, &Combo::restored, this, &MainWindow::restoreDeleted);
    widget->exec();
}

void MainWindow::on_actionExport_triggered() {
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", dirToWrite(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
        fileName.append(".pdf");
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);
    QStringList taskList;
    for (auto widget : ui->frame->findChildren<QLabel *>())
        taskList.append("<span>&#8226; " + widget->text() + "</span>");
    QTextDocument doc;
    doc.setHtml(taskList.join("<br/><br/>"));
    doc.print(&printer);
}

void MainWindow::createTrayIcon() {
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/icons/chorespp.png"), this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_toggle_show(QSystemTrayIcon::ActivationReason)));
    QAction *quit_action = new QAction("Exit", trayIcon);
    connect(quit_action, SIGNAL(triggered()), this, SLOT(on_exit()));
    QAction *show_hide_action = new QAction("Show/Hide", trayIcon);
    connect(show_hide_action, SIGNAL(triggered()), this, SLOT(on_show_hide()));

    QMenu *trayIconMenu = new QMenu;
    trayIconMenu->addAction(show_hide_action);
    trayIconMenu->addAction(quit_action);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void MainWindow::on_toggle_show(QSystemTrayIcon::ActivationReason r) {
    if (r)
        if (r != QSystemTrayIcon::DoubleClick)
            return;
    if (isVisible())
        hide();
    else {
        show();
        raise();
        setFocus();
    }
}

void MainWindow::on_show_hide() {
    if (isVisible())
        hide();
    else {
        show();
        raise();
        setFocus();
    }
}

void MainWindow::on_exit() {
    this->close();
}

void MainWindow::readSettings() {
    QSettings settings;
    bool isMax = settings.value("isMaximized", false).toBool();
    if (isMax)
        showMaximized();
    else {
        const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
        restoreGeometry(geometry);
    }
    bool hasLines = settings.value("lines", true).toBool();
    if (hasLines)
        lines = settings.value("lines").toInt();
    else
        lines = 1;
    const QStringList taskList = settings.value("tasks", QStringList()).toStringList();
    for (auto task : taskList) {
        tasks.append(task);
        auto widget = new Form(ui->frame, QString(task));
        connect(widget, &Form::valueChanged, this, &MainWindow::doUpdates);
        connect(widget, &Form::taskCompleted, this, &MainWindow::moveCompleted);
        widget->setObjectName(QString::fromUtf8("formItem") + QString::number(lines));
        widget->setAttribute(Qt::WA_DeleteOnClose);
        layout->addWidget(widget);
        widget->show();
    }
    const QStringList completedItems = settings.value("completed", QStringList()).toStringList();
    for (QString task : completedItems)
        complTasks.append(task);
    restored = 0;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings;
    settings.setValue("isMaximized", isMaximized());
    if (!isMaximized())
        settings.setValue("geometry", saveGeometry());
    settings.setValue("lines", ui->frame->children().count());
    tasks.clear();
    auto labelList = ui->frame->findChildren<QLabel *>();
    for (auto label : labelList)
        tasks.append(label->text());
    settings.setValue("tasks", tasks);
    settings.setValue("completed", complTasks);
    event->accept();
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("Program Info"),
                       (QApplication::applicationName() + " " + QApplication::applicationVersion() + "<br/><br/>" +
                        tr("Program created by Dimitris Psathas<br/><br/>"
                           "Written in C++, built with the Qt5 toolkit<br/><br/>"
                           "Published under the GNU General Public License v3.0<br/>"
                           "Using Qt libraries under (L)GPL3<br/><br/>"
                           "&copy; Dimitris Psathas, 2020")));
}
