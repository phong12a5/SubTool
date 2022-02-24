#include "servicemanager.h"
#include <QMutex>

ServiceManager* ServiceManager::sInstance = nullptr;

ServiceManager *ServiceManager::instance()
{
    QMutex mutex;
    mutex.lock();
    if(sInstance == NULL) {
        sInstance = new ServiceManager();
    }
    mutex.unlock();
    return sInstance;
}

QList<BaseService*> ServiceManager::getServiceIds()
{
    QList<BaseService*> serivceIds;
    foreach(BaseService* service, m_listService) {
        serivceIds.append(service);
    }
    return serivceIds;
}

ServiceManager::ServiceManager(QObject *parent) : QObject(parent)
{
    m_listService.clear();
}

ServiceManager::~ServiceManager()
{
    foreach(BaseService* serviceId, getServiceIds()) {
        deleteService(serviceId);
    }
    LOGD;
}

int ServiceManager::countService()
{
    return m_listService.count();
}

void ServiceManager::deleteService(BaseService * service)
{
    if(m_listService.removeOne(service)) {
        service->dispose();
    }
}

QList<BaseService *> ServiceManager::listService()
{
    return m_listService;
}
