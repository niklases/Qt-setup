// nvml_vram.cpp
#include <nvml.h>
#include <QDebug>
#include <QString>
#include <QDateTime> 


QString getVramInfo() 
{
    nvmlReturn_t result;
    nvmlDevice_t device;
    nvmlMemory_t memory;

    result = nvmlInit();
    if (NVML_SUCCESS != result) {
        return "Failed to initialize NVML";
    }

    result = nvmlDeviceGetHandleByIndex(0, &device);  // GPU 0
    if (NVML_SUCCESS != result) {
        nvmlShutdown();
        return "Failed to get device handle";
    }

    result = nvmlDeviceGetMemoryInfo(device, &memory);
    if (NVML_SUCCESS != result) {
        nvmlShutdown();
        return "Failed to get memory info";
    }
    QString timestamp = QDateTime::currentDateTime()
                            .toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString info = QString("%1  |  VRAM Used: %2 MB  /  %3 MB")
        .arg(timestamp)
        .arg(memory.used / (1024 * 1024))
        .arg(memory.total / (1024 * 1024));

    nvmlShutdown();
    return info;
}
