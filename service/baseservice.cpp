#include "baseservice.h"
#include "servicedata.h"
#include "servicemanager.h"

BaseService::BaseService(SERVICE_TYPE type, BaseWorker* woker, QObject *parent)
    : QObject(parent),
      m_worker(woker),
      m_type(type)
{
    LOGD << "--------- BaseService ---------";
    m_service_data = new ServiceData(this);
    m_workerThread = new QThread();
    m_worker->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::finished, m_worker, &BaseWorker::deleteLater);

    connect(this, &BaseService::startFDriver, m_worker, &BaseWorker::onStartFDriver, Qt::QueuedConnection);

    connect(m_worker, &BaseWorker::workerFinished, this, &BaseService::onWorkerFinished, Qt::QueuedConnection);
    connect(this, &BaseService::serviceFinished, ServiceManager::instance(), &ServiceManager::onServiceFinished, Qt::QueuedConnection);



//    connect(m_workerThread, &QThread::finished, m_worker, &BaseWorker::deleteLater);
//    connect(m_workerThread, &QThread::finished, m_workerThread, &QThread::deleteLater);

    m_workerThread->start();
}

BaseService::~BaseService()
{
    LOGD << "ngoc anh=============>BaseService::~BaseService";
//    LOGD;
//    if(nullptr != m_worker) {
//        delete m_worker;
//        m_worker = nullptr;
//    }

    if(nullptr != m_service_data) {
        delete m_service_data;
        m_service_data = nullptr;
    }
    m_workerThread->quit();
    m_workerThread->wait();
    m_workerThread->deleteLater();
    LOGD << "ngoc anh=============>BaseService::~BaseService 2";
}

int BaseService::type()
{
    return m_type;
}

void BaseService::startService(QString UID)
{
    emit startFDriver(UID);
}

void BaseService::stopThread()
{
    LOGD << "delete thread";
//    m_workerThread->terminate();
    m_workerThread->quit();
    m_workerThread->wait();
}

void BaseService::setInfoClone(CommonInforClone *clone)
{
    m_infoClone = clone;
    m_worker->setInfoClone(m_infoClone);
    m_service_data->set2Fa(m_infoClone->get2Fa());
}

CommonInforClone *BaseService::getInfoClone() const
{
    return m_infoClone;
}

void BaseService::forceStop()
{

}

void BaseService::onWorkerFinished()
{
    LOGD << "ngoc anh=============>onWorkerFinished: " << m_service_data->getServiceID();
    emit serviceFinished(m_service_data->getServiceID());
}
