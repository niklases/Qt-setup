#include <iostream>
#include <typeinfo>
#include <QGridLayout>
#include <QTextCharFormat>
#include <QTextCursor>
#include "mainwindow.h"
#include "vram_worker.h"
#include "nvml_vram.h"


// https://doc.qt.io/qt-6/qtwidgets-layouts-basiclayouts-example.html

const char *button_style = R"(
QPushButton {
	border: 2px solid rgb(52, 59, 72);
	border-radius: 5px;	
	background-color: rgb(52, 59, 72);
	color: white; 
}
QPushButton:hover {
	background-color: rgb(57, 65, 80);
	border: 2px solid rgb(61, 70, 86);
}
QPushButton:pressed {	
	background-color: rgb(35, 40, 49);
	border: 2px solid rgb(43, 50, 61);
}
QPushButton:disabled {
    background-color: grey;
}
)";

const char *text_style = R"(
QLabel {
	color: white;
})";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(QSize(1000, 600));
    this->setStyleSheet("background-color: rgb(1, 10, 29);");

    // Create a central widget
    QWidget *centralWidget = new QWidget(this);
    // Set the central widget of the MainWindow
    setCentralWidget(centralWidget);
    QGridLayout *m_layout = new QGridLayout(centralWidget);


    m_button_abort_work = getDefaultButton("Abort work");
    m_button_abort_work->setParent(centralWidget);
    m_button_abort_work->blockSignals(true);

    m_button_start_task = getDefaultButton("Start Task");
    connect(m_button_start_task, &QPushButton::clicked, this, &MainWindow::startWork);
    
    m_button_do_nothing = getDefaultButton("Do nothing");
    m_button_do_nothing->setParent(centralWidget);

    m_any_text = new QLabel("I am a multi-threading Qt GUI!", centralWidget);
    m_any_text->setStyleSheet(text_style);

    m_device_text_out = new QTextEdit(centralWidget);
    m_device_text_out->setStyleSheet("background-color: rgb(0, 0, 0);");

    m_vramLabel = new QLabel(centralWidget);
    m_vramLabel->setStyleSheet(text_style);

    //                  widget, row, column, rowSpan, columnSpan
    m_layout->addWidget(m_button_start_task, 0, 0, 1, 1);
    m_layout->addWidget(m_button_do_nothing, 0, 1, 1, 1);
    m_layout->addWidget(m_button_abort_work, 0, 2, 1, 1);
    m_layout->addWidget(m_any_text, 1, 0, 1, 1);   
    m_layout->addWidget(m_device_text_out, 2, 0, 3, 3);
    m_layout->addWidget(m_vramLabel, 5, 0, 1, 3);

    QString info = getVramInfo();
    qDebug() << info;

    // VRAM info thread
    m_vram_thread = new QThread(this);
    m_vram_worker = new VramWorker();
    m_vram_worker->moveToThread(m_vram_thread);

    // Connect to update the label from the worker (worker emits signal "vramUpdated")
    // Update label in GUI thread
    connect(m_vram_worker, &VramWorker::vramUpdated, this, [=](const QString &info) {
        m_vramLabel->setText(info);
    });
    // Start polling when thread starts
    connect(m_vram_thread, &QThread::started, m_vram_worker, &VramWorker::startPolling);
    // Clean up
    connect(this, &MainWindow::destroyed, m_vram_thread, &QThread::quit);
    connect(m_vram_thread, &QThread::finished, m_vram_worker, &QObject::deleteLater);
    connect(m_vram_thread, &QThread::finished, m_vram_thread, &QObject::deleteLater);


    // connect aborting button (here?)
    connect(m_button_abort_work, &QPushButton::clicked, this, &MainWindow::abortWork);

    m_vram_thread->start();
}

MainWindow::~MainWindow()
{
}

void MainWindow::startWork()
{
    // Main computing thread
    m_thread = new QThread(this);
    m_worker = new Worker();
    m_worker->moveToThread(m_thread);
    m_button_abort_work->blockSignals(false);

    connect(m_thread, &QThread::started, m_worker, &Worker::doWork);
    connect(m_worker, &Worker::textReady, this, &MainWindow::appendText);
    connect(m_worker, &Worker::finished, this, &MainWindow::workFinished);
    connect(m_worker, &Worker::finished, m_thread, &QThread::quit);
    connect(m_worker, &Worker::finished, m_worker, &Worker::deleteLater);
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);

    m_thread->start();
}

void MainWindow::workFinished()
{
    qDebug() << "Task finished, quitting thread...";
    m_button_abort_work->blockSignals(true);
    m_thread->quit();
    m_thread->wait();
    delete m_thread;  // not required?!
    qDebug() << "Thread quit!";
}

void MainWindow::abortWork()
{
    qDebug() << "Pressed abort button...";
    m_worker->abort();
}

void MainWindow::appendText(const QString &text)
{
    QTextCharFormat format;
    format.setForeground(QBrush(QColor(Qt::white)));
    QTextCursor cursor(m_device_text_out->document());
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(text, format);
}

QPushButton* MainWindow::getDefaultButton(const QString &text)
{
    QPushButton *button = new QPushButton(text, this);
    button->setGeometry(10, 10, 100, 30);
    button->setStyleSheet(button_style);
    return button;
}

