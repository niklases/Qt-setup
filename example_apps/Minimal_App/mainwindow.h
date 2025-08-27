#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QThread>
#include <QDebug>
#include "vram_worker.h"

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker() : m_abortRun(false) {}

public slots:
    void doWork()
    {
        for (int i = 0; i < 100; ++i)
        {
            qDebug() << "Working..." << i << "Abort status:" << m_abortRun;
            if (m_abortRun) 
                break;
            emit textReady(QString("Appending text %1\n").arg(i));
            QThread::msleep(10);
        }
        emit finished();
    }

    void abort() 
    {
        qDebug() << "Aborting...";
        QMutexLocker locker(&m_mutex);
        m_abortRun = true;
    }

signals:
    void textReady(const QString &text);
    void finished();

private:
    bool m_abortRun;
    QMutex m_mutex;
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startWork();
    void abortWork();
    void workFinished();
    void appendText(const QString &text);
    QPushButton* getDefaultButton(const QString &text);

private:
    bool m_abortRun;
    QPushButton *m_button_start_task;
    QPushButton *m_button_do_nothing;
    QPushButton *m_button_abort_work;
    QThread *m_thread;
    QThread *m_vram_thread;
    QLabel *m_any_text;
    QTextEdit *m_device_text_out;
    QLabel *m_vramLabel;
    Worker *m_worker;
    VramWorker *m_vram_worker;
};

#endif // MAINWINDOW_H

