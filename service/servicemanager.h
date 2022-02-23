#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <QObject>
#include "baseservice.h"
//#include "controllermainthread.h"

class ServiceManager : public QObject
{
    Q_OBJECT
public:
    static ServiceManager* instance();
    void start();
    void stop();

    QList<int> getServiceIds();
    int countService();
    void deleteService(int serviceId);
    QList<BaseService *> listService();

    template<typename T>
    T* getService(int serviceId){
        T* service = reinterpret_cast<T*>(serviceId);
        LOGD << "ServiceManager serviceId: " << serviceId;
        return service;
   }


    template<typename T>
    int createService() {
        T* service = new T();
        LOGD << " : " << service;
        m_listService.append(service);
        connect(service, &BaseService::started, this,&ServiceManager::serviceUpdated);
        connect(service, &BaseService::finished, this,&ServiceManager::serviceUpdated);
        return reinterpret_cast<int>(service);
    }

signals:
    void serviceUpdated();

private:
    explicit ServiceManager(QObject *parent = nullptr);
    ~ServiceManager();
private:
    static ServiceManager* sInstance;

    QList<BaseService*> m_listService;

signals:
};

#endif // SERVICEMANAGER_H
