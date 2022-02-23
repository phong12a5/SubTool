#include "chromeservice.h"

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

void ChromeService::forceStop()
{
    //    static_cast<ChromeWorker*>(m_worker)->forceCloseChrome();
}

void ChromeService::onSeparateThreadStarted()
{
    LOGD;
    Chrome chrome;
    JsonObject chromeOptions;

    std::vector<std::string> args;
    //    if(m_service_data->getLinkProfile().toStdString() == "") {
    //        args.push_back( "--user-data-dir=" + QDir::currentPath().toStdString() + "/profiles/" + m_infoClone->getUID().toStdString());
    //    } else {
    //        args.push_back( "--user-data-dir=" + m_service_data->getLinkProfile().toStdString() + "/profiles/" + m_infoClone->getUID().toStdString());
    //    }
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


    //    chromeOptions.Set<std::vector<std::string>>("args",args);
    //    chrome.SetChromeOptions(chromeOptions);
    //    fdriver::Size size;
    //    size.width = 300;
    //    size.height = 500;
    //    QProcess m_process;
    //    m_process.start("chromedriver.exe");
    //    m_process.waitForFinished(-1);
    //    QString output = m_process.readAllStandardOutput();
    //    QString error = m_process.readAllStandardError();

    //    LOGD <<output;
    //    LOGD <<error;


        LOGD << "Ahihi";

        driver = new FDriver(chrome);
    //    driver->GetCurrentWindow().SetPosition(Point(m_service_data->getX(), m_service_data->getY()));
    //    driver->GetCurrentWindow().SetSize(size);
        driver->Navigate("http://google.com")
                .FindElement(ByCss("input[name=q]"))
                .SendKeys("Hello, world!")
                .Submit();
}
