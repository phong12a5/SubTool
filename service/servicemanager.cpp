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

QList<int> ServiceManager::getServiceIds()
{
    QList<int> serivceIds;
    foreach(BaseService* service, m_listService) {
        serivceIds.append(reinterpret_cast<int>(service));
    }
    return serivceIds;
}

ServiceManager::ServiceManager(QObject *parent) : QObject(parent)
{
    m_listService.clear();
}

ServiceManager::~ServiceManager()
{
    foreach(int serviceId, getServiceIds()) {
        deleteService(serviceId);
    }
    LOGD;
}

int ServiceManager::countService()
{
    return m_listService.count();
}

void ServiceManager::deleteService(int serviceId)
{
    LOGD << serviceId;
    BaseService* service = reinterpret_cast<BaseService*>(serviceId);
    if(m_listService.removeOne(service)) {
        service->dispose();
    }
}

QList<BaseService *> ServiceManager::listService()
{
    return m_listService;
}
