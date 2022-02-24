#include "baseservice.h"
#include "model/servicedata.h"
#include "servicemanager.h"
#include <QDir>
#include <QProcess>

BaseService::BaseService(SERVICE_TYPE type, QObject *parent)
    : QObject(parent),
      m_type(type)
{
    m_workerThread = new QThread();
    this->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::finished, m_workerThread, &QThread::deleteLater);
    connect(m_workerThread, &QThread::finished, this, &QObject::deleteLater);
    connect(m_workerThread, &QThread::started, this, &BaseService::onThreadStarted);
}

BaseService::~BaseService()
{
    LOGD;
    emit finished();
    if(nullptr != driver) {
        delete driver;
    }

    if(nullptr != m_service_data) {
        delete m_service_data;
        m_service_data = nullptr;
    }

    if(main_process_repeater != nullptr) {
        delete main_process_repeater;
        main_process_repeater = nullptr;
    }
}

int BaseService::type()
{
    return m_type;
}

void BaseService::start()
{
    m_workerThread->start();
}

void BaseService::dispose()
{
    m_workerThread->quit();
    m_workerThread->wait();
}

void BaseService::startMainProcess()
{
    if(!main_process_repeater->isActive())
        main_process_repeater->start();
}

void BaseService::stopMainProcess()
{
    if(main_process_repeater->isActive())
        main_process_repeater->stop();
}

ServiceData *BaseService::serviceData()
{
    return m_service_data;
}

void BaseService::setServiceData(ServiceData* data)
{
    m_service_data = data;
}

void BaseService::onThreadStarted()
{
    main_process_repeater = new QTimer();
    main_process_repeater->setInterval(2000);
    main_process_repeater->setSingleShot(false);
    connect(main_process_repeater, &QTimer::timeout, this, &BaseService::onMainProcess);

    onStarted();
    emit started();
}
