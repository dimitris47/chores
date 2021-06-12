#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "combo.h"
#include "form.h"
#include "global.h"
#include <QActionGroup>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPrinter>
#include <QScrollArea>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    layout = new QVBoxLayout;
    frame = new QFrame;
    frame->setLayout(layout);
    layout->setAlignment(Qt::AlignTop);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(frame);
    setCentralWidget(scrollArea);
    ui->lineEdit->setAlignment(Qt::AlignHCenter);
    ui->lineEdit->setPlaceholderText("new task");
    layout->addWidget(ui->lineEdit);

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
    for (auto &&loc : locations)
        if (QFileInfo::exists(loc)) {
            dir = loc;
            return dir;
        }
    return QString();
}

void MainWindow::on_lineEdit_returnPressed() {
    QString text = ui->lineEdit->text();
    if (text != "") {
        auto widget = new Form(frame, text);
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
    savePrefs();
}

void MainWindow::on_actionAddTask_triggered() {
    on_lineEdit_returnPressed();
}


void MainWindow::doUpdates() {
    Form *f = qobject_cast<Form *>(sender());
    tasks.clear();
    choreList = f->choresList;
    for (auto &&chore : choreList)
        tasks.append(chore);
    auto labelList = frame->findChildren<QLabel *>();
    int i {0};
    for (auto &&label : labelList) {
        label->setText(tasks[i]);
        label->setToolTip(tasks[i]);
        i++;
    }
    savePrefs();
}

void MainWindow::moveCompleted() {
    Form *f = qobject_cast<Form *>(sender());
    complTasks.append(f->completed);
    savePrefs();
}

void MainWindow::permDelete() {
    Combo *c = qobject_cast<Combo *>(sender());
    complTasks.removeAt(c->p);
    savePrefs();
}

void MainWindow::restoreDeleted() {
    Combo *c = qobject_cast<Combo *>(sender());
    complTasks.removeAt(c->p);
    auto widget = new Form(frame, QString(c->text));
    connect(widget, &Form::valueChanged, this, &MainWindow::doUpdates);
    connect(widget, &Form::taskCompleted, this, &MainWindow::moveCompleted);
    widget->setObjectName(QString::fromUtf8("restItem") + QString::number(restored));
    widget->setAttribute(Qt::WA_DeleteOnClose);
    layout->addWidget(widget);
    widget->show();
    restored++;
    savePrefs();
}

void MainWindow::on_actionShow_Completed_triggered() {
    auto widget = new Combo(this, complTasks);
    connect(widget, &Combo::deleted, this, &MainWindow::permDelete);
    connect(widget, &Combo::restored, this, &MainWindow::restoreDeleted);
    widget->exec();
    savePrefs();
}

void MainWindow::on_actionFont_triggered() {
    QApplication::setFont(QFontDialog::getFont(0, QApplication::font()));
}

void MainWindow::on_actionExport_triggered() {
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", dirToWrite(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
        fileName.append(".pdf");
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fileName);
    QStringList taskList;
    for (auto &&widget : frame->findChildren<QLabel *>())
        taskList.append("<span>&#8226; " + widget->text().replace("<", "&#60;") + "</span>");
    QTextDocument doc;
    doc.setHtml(taskList.join("<br/><br/>"));
    doc.print(&printer);
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("Program Info"),
                       (QApplication::applicationName() + " " + QApplication::applicationVersion() + "<br/><br/>" +
                        APPINFO));
}

void MainWindow::createTrayIcon() {
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/icons/chorespp.png"), this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_toggleShow()));

    QAction *quit_action = new QAction("Exit", trayIcon);
    connect(quit_action, &QAction::triggered, this, [=](){ this->close(); });

    QAction *show_hide_action = new QAction("Show/Hide", trayIcon);
    connect(show_hide_action, SIGNAL(triggered()), this, SLOT(on_toggleShow()));

    QMenu *trayIconMenu = new QMenu;
    trayIconMenu->addAction(show_hide_action);
    trayIconMenu->addAction(quit_action);

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void MainWindow::on_toggleShow() {
    QSettings settings;
    if (isVisible()) {
        settings.setValue("geometry", saveGeometry());
        hide();
    }
    else {
        show();
        const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
        restoreGeometry(geometry);
        raise();
        setFocus();
    }
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
    hasLines ? lines = settings.value("lines").toInt() : lines = 1;
    const QStringList taskList = settings.value("tasks", QStringList()).toStringList();
    for (auto &&task : taskList) {
        tasks.append(task);
        auto widget = new Form(frame, QString(task));
        connect(widget, &Form::valueChanged, this, &MainWindow::doUpdates);
        connect(widget, &Form::taskCompleted, this, &MainWindow::moveCompleted);
        widget->setObjectName(QString::fromUtf8("formItem") + QString::number(lines));
        widget->setAttribute(Qt::WA_DeleteOnClose);
        layout->addWidget(widget);
        widget->show();
    }
    const QStringList completedItems = settings.value("completed", QStringList()).toStringList();
    for (auto &&task : completedItems)
        complTasks.append(task);
    const QString f = settings.value("font", QFont()).toString();
    const int s = settings.value("size", 11).toInt();
    const QFont font(f, s);
    QApplication::setFont(font);
    restored = 0;
}

void MainWindow::savePrefs() {
    QSettings settings;
    settings.setValue("isMaximized", isMaximized());
    if (!isMaximized())
        settings.setValue("geometry", saveGeometry());
    settings.setValue("lines", frame->children().count());
    tasks.clear();
    auto labelList = frame->findChildren<QLabel *>();
    for (auto &&label : labelList)
        tasks.append(label->text());
    settings.setValue("tasks", tasks);
    settings.setValue("completed", complTasks);
    settings.setValue("font", QApplication::font().toString());
    settings.setValue("size", QApplication::font().pointSize());
    settings.sync();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    savePrefs();
    event->accept();
}
