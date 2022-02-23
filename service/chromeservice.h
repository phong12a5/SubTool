#ifndef CHROMESERVICE_H
#define CHROMESERVICE_H

#include <QObject>
#include "baseservice.h"
#include "chromeworker.h"

class ServiceData;

class ChromeService : public BaseService
{
    Q_OBJECT
public:
    explicit ChromeService(QObject *parent = nullptr);
    ~ChromeService();

    void connectSignalSlots();
    ServiceData* model();

    void forceStop() override;
};

#endif // CHROMESERVICE_H
