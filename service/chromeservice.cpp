#include "chromeservice.h"
#include "model/servicedata.h"
#include <QTimer>
#include <WebAPI.hpp>

ChromeService::ChromeService(QObject *parent) :
    BaseService(SERVICE_TYPE::TYPE_CHROME_SERVICE, parent)
{
//    LOGD << "--------- ChromeService ---------";
//    m_worker->setSerivceData(m_service_data);
    connectSignalSlots();
}

ChromeService::~ChromeService()
{
//    LOGD;
}

void ChromeService::connectSignalSlots()
{
//    LOGD;
}

ServiceData *ChromeService::model()
{
    return m_service_data;
}

void ChromeService::onStarted()
{
    LOGD;
    Chrome chrome;
    JsonObject chromeOptions;

    std::vector<std::string> args;
    args.push_back( "--user-data-dir=" + serviceData()->profilePath().toStdString());
    args.push_back("--ignore-certificate-errors");

    //    switch(m_service_data->getProxyKind()){
    //    case PROXY_KIND::HTTP:
    //        args.push_back("--proxy-server= " + proxy.toStdString());
    //        LOGD << "-------------proxy:  " << proxy;
    //        break;
    //    case PROXY_KIND::SOCKS:
    //        args.push_back("--proxy-server= socks5://" + proxy.toStdString());
    //        LOGD << "-------------proxy:  " << proxy;
    //        break;
    //    }

    //            args.push_back("--user-agent=" + m_infoClone->getUserAgent().toStdString());
        args.push_back("--disable-notifications");


    chromeOptions.Set<std::vector<std::string>>("args",args);
    chrome.SetChromeOptions(chromeOptions);

    driver = new FDriver(chrome);
    //    driver->GetCurrentWindow().SetPosition(Point(m_service_data->getX(), m_service_data->getY()));
    //    driver->GetCurrentWindow().SetSize(size);
    driver->Navigate("http://m.facebook.com");
    if(serviceData()->cloneInfo() != nullptr &&
            !serviceData()->cloneInfo()->cookies().isEmpty()) {
        setCookies(serviceData()->cloneInfo()->cookies());
    }
    driver->Navigate("http://m.facebook.com");

    startMainProcess();
}

void ChromeService::onMainProcess()
{
    LOGD;
    if(serviceData()->cloneInfo() == nullptr) {
        std::string result = WebAPI::getInstance()->getClone(nullptr, "facebook");
        LOGD << "clone: " << result.c_str();
        QJsonObject cloneInfo = QJsonDocument::fromJson(result.c_str()).object().value("cloneInfo").toObject();
        LOGD << "cloneInfo: " <<cloneInfo;
        if(!cloneInfo.isEmpty()) {
            serviceData()->setCloneInfo(new CloneInfo(cloneInfo));
            if(serviceData()->cloneInfo()) {
                serviceData()->cloneInfo()->setAliveStatus("stored");
            }

            stopMainProcess();
        }
    }
}
