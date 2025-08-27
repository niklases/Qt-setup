#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QObject>

class Worker : public QObject {
    Q_OBJECT

public slots:
    void doWork() {
        qDebug() << "Hello from worker thread:" << QThread::currentThreadId();
        emit workFinished();
    }

signals:
    void workFinished();
};

/* "SRC:/hello_qt.cpp"
contains a "Q_OBJECT" macro, but does not include "hello_qt.moc"!
Consider to
  - add #include "hello_qt.moc"
  - enable SKIP_AUTOMOC for this file
*/
#include "hello_qt.moc" // Required for MOC to process Q_OBJECT macros


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QThread workerThread;
    workerThread.setObjectName("WorkerThread");

    // 1. Create the worker on the HEAP using 'new'.
    Worker *worker = new Worker();

    // 2. Move the worker to the thread.
    worker->moveToThread(&workerThread);

    // 3. Connect signals and slots (using the worker pointer).
    QObject::connect(&workerThread, &QThread::started, worker, &Worker::doWork);
    QObject::connect(worker, &Worker::workFinished, &workerThread, &QThread::quit);
    
    // This connection is now ESSENTIAL to prevent a memory leak.
    // It ensures the heap-allocated worker is deleted correctly when the thread finishes.
    QObject::connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    QObject::connect(&workerThread, &QThread::finished, &app, &QCoreApplication::quit);

    qDebug() << "Hello from main thread:" << QThread::currentThreadId();

    workerThread.start();
    return app.exec();
}