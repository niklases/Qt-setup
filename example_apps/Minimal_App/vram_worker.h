#ifndef VRAMWORKER_H
#define VRAMWORKER_H

#include <QObject>
#include <QTimer>

class VramWorker : public QObject {
    Q_OBJECT

public:
    explicit VramWorker(QObject *parent = nullptr);
    ~VramWorker();

signals:
    void vramUpdated(const QString &vramInfo);

public slots:
    void startPolling();
    void stopPolling();

private:
    QTimer *pollTimer;
};

#endif // VRAMWORKER_H