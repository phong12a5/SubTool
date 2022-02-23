#ifndef CHROMESERVICE_H
#define CHROMESERVICE_H

#include <QObject>
#include "baseservice.h"
//#include "worker/chromeworker.h"

class ServiceData;

class ChromeService : public BaseService
{
    Q_OBJECT
public:
    explicit ChromeService(QObject *parent = nullptr);
    ~ChromeService();

    void connectSignalSlots() override;
    ServiceData* model();

    void forceStop() override;

public slots:
    void onSeparateThreadStarted() override;

private:
    QThread m_;
};

#endif // CHROMESERVICE_H
