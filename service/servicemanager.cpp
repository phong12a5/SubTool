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
        delete service;
    }

    if(m_listService.isEmpty()) {
        CONTROLLERMAIN->setServiceRunning(false);
    }
}

void ServiceManager::forceDeleteAllService()
{
    LOGD;
    if(!m_listService.isEmpty())
    {
        qDeleteAll(m_listService);
        m_listService.clear();
    }
    CONTROLLERMAIN->setServiceRunning(false);
}

QList<BaseService *> ServiceManager::listService()
{
    return m_listService;
}

void ServiceManager::onServiceFinished(int serviceId)
{
    LOGD << "onServiceFinished serviceId: " << serviceId;
    LOGD << "onServiceFinished---  truoc khi remove: " << m_listService.size();
    BaseService* service = reinterpret_cast<BaseService*>(serviceId);
    QString cloneId = service->getInfoClone()->getCloneId();
    m_listService.removeOne(service);
    delete service;
    LOGD << "onServiceFinished---  sau khi remove: " << m_listService.size();
    if(m_listService.isEmpty()) {
        CONTROLLERMAIN->setServiceRunning(false);
    }
    emit runCloneFinished(cloneId);
}
