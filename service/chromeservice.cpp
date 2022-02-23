#include "chromeservice.h"

ChromeService::ChromeService(QObject *parent) :
    BaseService(SERVICE_TYPE::TYPE_CHROME_SERVICE, new ChromeWorker(), parent)
{
    LOGD << "--------- ChromeService ---------";
    m_worker->setSerivceData(m_service_data);
    connectSignalSlots();
}

ChromeService::~ChromeService()
{
    LOGD;
}

void ChromeService::connectSignalSlots()
{
    LOGD;
}

ServiceData *ChromeService::model()
{
    return m_service_data;
}

void ChromeService::forceStop()
{
    static_cast<ChromeWorker*>(m_worker)->forceCloseChrome();
}
