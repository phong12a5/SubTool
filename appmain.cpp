#include "appmain.h"
#include "service/servicemanager.h"
#include "service/chromeservice.h"

AppMain* AppMain::m_instance = NULL;

AppMain::AppMain(QObject *parent) : QObject(parent)
{

}

AppMain *AppMain::instance()
{
    if(m_instance == NULL) {
        m_instance = new AppMain();
    }
    return m_instance;
}

bool AppMain::startTest()
{
    LOGD;
    if(FDriver::unlockFDriver("0399843737")) {
        if(ServiceManager::instance()->countService() == 0) {
            int serviceID1 = ServiceManager::instance()->createService<ChromeService>();
            ChromeService* service1 = ServiceManager::instance()->getService<ChromeService>(serviceID1);
            service1->startService("aaaa");

        }
        return true;
    } else {
        LOGD << "unlock Fdriver failed";
        return false;
    }
}

bool AppMain::stopTest()
{
    LOGD;
    foreach(int serviceId, ServiceManager::instance()->getServiceIds()) {
        ServiceManager::instance()->deleteService(serviceId);
    }
    return true;
}

void AppMain::onClickedStart(bool state)
{
    LOGD << "clicked: " << state;
    if(state)
        startTest();
    else
        stopTest();
}
