#include "appmain.h"
#include "service/servicemanager.h"
#include "service/chromeservice.h"
#include "fdriver.h"
#include "appmodel.h"
#include <QCoreApplication>
#include <QDir>
#include <WebAPI.hpp>
#include <service/apiservices.h>

using namespace fdriver;

AppMain* AppMain::m_instance = NULL;

AppMain::AppMain(QObject *parent) : QObject(parent)
{
    QCoreApplication::setOrganizationName("AutoFarmer");
    QCoreApplication::setOrganizationDomain("autofarmer.net");
    QCoreApplication::setApplicationName("Subscribe Tool");

    connect(ServiceManager::instance(), &ServiceManager::serviceUpdated, this, &AppMain::onServiceUpdated);
    connect(APIServices::instance(), &APIServices::configChanged, this, &AppMain::onConfigChanged);
    APIServices::instance()->startService();
}

AppMain *AppMain::instance()
{
    if(m_instance == NULL) {
        m_instance = new AppMain();
    }
    return m_instance;
}

bool AppMain::start()
{
    LOGD;
    if(m_preconditionChecker == nullptr) {
        m_preconditionChecker = new QTimer();
        m_preconditionChecker->setSingleShot(false);
        m_preconditionChecker->setInterval(2000);
        connect(m_preconditionChecker, &QTimer::timeout, this, &AppMain::onCheckPrecondition);
    }
    m_preconditionChecker->start();
    AppModel::instance()->setAppStarted(true);
    return true;
}

bool AppMain::stop()
{
    m_preconditionChecker->stop();
    foreach(int serviceId, ServiceManager::instance()->getServiceIds()) {
        ServiceManager::instance()->deleteService(serviceId);
    }
    AppModel::instance()->setAppStarted(false);
    return true;
}

void AppMain::onCheckPrecondition()
{
    LOGD;
    if(!APIServices::instance()->isFdriverReady()) {
        LOGD << "Fdriver is not ready";
    } else if(!APIServices::instance()->isAFAPIReady()) {
        LOGD << "Autofarmer APIs is not ready";
    } else if(!APIServices::instance()->isDeviceApproved()) {
        LOGD << "Device is not approved";
    } else {
        if(m_chromeDriverProcess == nullptr) {
            m_chromeDriverProcess = new QProcess(this);
            m_chromeDriverProcess->setWorkingDirectory(QDir::currentPath());
            m_chromeDriverProcess->setProgram("chromedriver.exe");
            connect(m_chromeDriverProcess, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error){
                LOGD << error;
            });
            m_chromeDriverProcess->start();
            m_chromeDriverProcess->waitForStarted(-1);
        }
        emit ServiceManager::instance()->serviceUpdated();
    }
}

void AppMain::onServiceUpdated()
{
    LOGD;
    if(AppModel::instance()->appStarted()) {
        if(ServiceManager::instance()->countService() < AppModel::instance()->maxThread()) {
            ServiceManager::instance()->createService<ChromeService>();
        }
    }
}

void AppMain::onConfigChanged()
{
    LOGD;
    AppModel::instance()->setDeviceStatus(APIServices::instance()->deviceStatus().isEmpty()? "checking" : APIServices::instance()->deviceStatus());
}

void AppMain::initAutofarmerAPIs()
{
//    if(WebAPI::getInstance()->initWebAPIs(nullptr, AppModel::instance()->token().toUtf8().data(),\
//                                       AppModel::instance()->deviceName().toUtf8().data(),\
//                                       AppModel::instance()->deviceName().toUtf8().data(),\
//                                       AppModel::instance()->appVersion().toUtf8().data())) {

//    } else {
//        LOGD << "init api failed";
//    }
}
