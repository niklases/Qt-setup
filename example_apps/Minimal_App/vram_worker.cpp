#include "vram_worker.h"
#include "nvml_vram.h"

VramWorker::VramWorker(QObject *parent) : QObject(parent) {
    pollTimer = new QTimer(this);
    pollTimer->setInterval(100);  // 0.1 seconds
    connect(pollTimer, &QTimer::timeout, this, [=]() {
        QString info = getVramInfo();
        emit vramUpdated(info);
    });
}

VramWorker::~VramWorker() {}

void VramWorker::startPolling() {
    pollTimer->start();
}

void VramWorker::stopPolling() {
    pollTimer->stop();
}
