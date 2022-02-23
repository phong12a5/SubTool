#include "appmain.h"
#include "service/servicemanager.h"
#include "service/chromeservice.h"
#include "fdriver.h"
#include "appmodel.h"
#include <QCoreApplication>
#include <QDir>

using namespace fdriver;

AppMain* AppMain::m_instance = NULL;

AppMain::AppMain(QObject *parent) : QObject(parent)
{
    QCoreApplication::setOrganizationName("AutoFarmer");
    QCoreApplication::setOrganizationDomain("autofarmer.net");
    QCoreApplication::setApplicationName("Subscribe Tool");

    connect(ServiceManager::instance(), &ServiceManager::serviceUpdated, this, &AppMain::onServiceUpdated);
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
    if(FDriver::unlockFDriver("0399843737")) {
        AppModel::instance()->setAppStarted(true);

        if(m_chromeDriverProcess == nullptr) {
            m_chromeDriverProcess = new QProcess(this);
            m_chromeDriverProcess->setWorkingDirectory(QDir::currentPath());
            m_chromeDriverProcess->setProgram("chromedriver.exe");
            connect(m_chromeDriverProcess, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error){
                LOGD << error;
            });
        }
        m_chromeDriverProcess->start();
        m_chromeDriverProcess->waitForFinished();

        emit ServiceManager::instance()->serviceUpdated();
        return true;
    } else {
        LOGD << "unlock Fdriver failed";
        return false;
    }

}

bool AppMain::stop()
{
    LOGD;
    if(m_chromeDriverProcess != nullptr)
        m_chromeDriverProcess->terminate();
    QProcess::execute("Taskkill /IM chromedriver.exe /F");


    foreach(int serviceId, ServiceManager::instance()->getServiceIds()) {
        ServiceManager::instance()->deleteService(serviceId);
    }
    AppModel::instance()->setAppStarted(false);
    return true;
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
