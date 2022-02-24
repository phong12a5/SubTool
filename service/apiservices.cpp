#include "apiservices.h"
#include "log.h"
#include "fdriver.h"
#include <WebAPI.hpp>
#include <appmodel.h>

using namespace fdriver;

static APIServices* sInstance = nullptr;

APIServices *APIServices::instance()
{
    if(!sInstance) {
        sInstance = new APIServices();
    }
    return sInstance;
}

APIServices::APIServices(QObject *parent) : QObject(parent)
{
    m_checker = nullptr;
}

void APIServices::startService()
{
    m_subThread = new QThread();
    this->moveToThread(m_subThread);
    connect(m_subThread , &QThread::started, this, &APIServices::onStarted);
    m_subThread->start();
}

bool APIServices::isFdriverReady()
{
    return m_isFdriverReady;
}

bool APIServices::isAFAPIReady()
{
    return m_isAFAPIReady;
}

bool APIServices::isDeviceApproved()
{
    return m_config.value("device_info").toObject().value("status") == "Approved";
}

QString APIServices::deviceStatus()
{
    return m_config.value("device_info").toObject().value("status").toString();
}

QJsonObject APIServices::config()
{
    return m_config;
}

void APIServices::setConfig(QJsonObject config)
{
    if(m_config != config) {
        m_config = config;
        emit configChanged();
    }
}

void APIServices::onStarted()
{
    LOGD;
    if(m_checker == nullptr) {
        m_checker  = new QTimer();
        m_checker->setInterval(2000);
        m_checker->setSingleShot(false);
        connect(m_checker , &QTimer::timeout, this, &APIServices::onChecking);
        m_checker->start();
        LOGD << "Timer started";
    }
}

void APIServices::onChecking()
{
    LOGD;
    if(!m_isFdriverReady && !FDriver::unlockFDriver("0399843737")) {
        m_isFdriverReady = false;
    } else {
        m_isFdriverReady = true;

        if(!m_isAFAPIReady && !WebAPI::getInstance()->initWebAPIs(nullptr, AppModel::instance()->token().toUtf8().data(),\
                                                                  AppModel::instance()->deviceName().toUtf8().data(),\
                                                                  AppModel::instance()->deviceName().toUtf8().data(),\
                                                                  AppModel::instance()->appVersion().toUtf8().data())) {
            m_isAFAPIReady = false;
        } else {
            m_isAFAPIReady = true;
            std::string configStr = WebAPI::getInstance()->getConfig(nullptr);
            QJsonObject config = QJsonDocument::fromJson(configStr.c_str()).object().value("config").toObject();
            setConfig(config);
            QJsonObject deviceInfo = m_config.value("device_info").toObject();
            if((!deviceInfo.empty()) && !deviceInfo.contains("status")) {
                WebAPI::getInstance()->upsertDevice(nullptr, nullptr);
            } else if(isDeviceApproved()) {
                m_checker->stop();
            }
        }
    }
}
