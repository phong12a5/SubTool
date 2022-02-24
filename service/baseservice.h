#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <QObject>
#include <QThread>
#include "log.h"
#include "model/cloneinfo.h"
#include "fdriver/include/fdriver.h"
#include "fdriver/include/browsers/chrome.h"
#include <QTimer>

using namespace fdriver;

class ServiceData;

class BaseService : public QObject
{
    Q_OBJECT
public:
    enum SERVICE_TYPE: int {
      TYPE_CHROME_SERVICE = 0,
      TYPE_FIREFOX_SERVICE
    };
public:
    explicit BaseService(SERVICE_TYPE type, QObject *parent = nullptr);
    virtual ~BaseService();

    int type();
    void start();
    void dispose();
    void startMainProcess();
    void stopMainProcess();

    ServiceData* serviceData();
    void setServiceData(ServiceData* data);

signals:
    void serviceFinished(int serviceId);

private slots:
    void onThreadStarted();

public slots:
    virtual void onStarted() = 0;
    virtual void onMainProcess() = 0;

    //Interface
protected:
    virtual void connectSignalSlots() = 0;

protected:
    QThread* m_workerThread = nullptr;
    ServiceData* m_service_data = nullptr;
    QTimer* main_process_repeater = nullptr;

private:
    int m_type;

protected:
    FDriver *driver = nullptr;

signals:
    void finished();
    void started();
};

#endif // BASESERVICE_H
