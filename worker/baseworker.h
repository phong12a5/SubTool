#ifndef BASEWORKER_H
#define BASEWORKER_H

#include <QObject>
#include "log.h"
#include "fdriver/include/fdriver.h"
#include "fdriver/include/browsers/chrome.h"
#include "CommonInforClone.h"

using namespace fdriver;

class ServiceData;

class BaseWorker : public QObject
{
    Q_OBJECT
public:
    explicit BaseWorker(QObject *parent = nullptr);
    virtual ~BaseWorker();
    void setSerivceData(ServiceData*);
    void setInfoClone(CommonInforClone* clone);

signals:
    void workerFinished();

public slots:
    virtual void onStartFDriver(QString UID) = 0;

protected:
    FDriver *driver = nullptr;
    ServiceData* m_service_data = nullptr;
    CommonInforClone* m_infoClone = nullptr;
};

#endif // BASEWORKER_H
