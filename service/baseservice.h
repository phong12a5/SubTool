#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <QObject>
#include <QThread>
#include "baseworker.h"
#include "log.h"
#include "CommonInforClone.h"

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
    explicit BaseService(SERVICE_TYPE type, BaseWorker* woker, QObject *parent = nullptr);
    virtual ~BaseService();

    int type();
    void startService(QString UID);
    void stopThread();
    void setInfoClone(CommonInforClone* clone);
    CommonInforClone* getInfoClone() const;

    // use this function is dangerous, only use when close app
    virtual void forceStop();

signals:
    void serviceFinished(int serviceId);

public slots:
    void onWorkerFinished();

    //Interface
protected:
    virtual void connectSignalSlots() = 0;

protected:
    QThread* m_workerThread = nullptr;
    BaseWorker* m_worker = nullptr;
    ServiceData* m_service_data = nullptr;
    CommonInforClone* m_infoClone = nullptr;

private:
    int m_type;

signals:
    void startFDriver(QString UID);
};

#endif // BASESERVICE_H
