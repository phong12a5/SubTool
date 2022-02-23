#include "baseservice.h"
#include "model/servicedata.h"
#include "servicemanager.h"
#include <QDir>
#include <QProcess>

BaseService::BaseService(SERVICE_TYPE type, QObject *parent)
    : QObject(parent),
      m_type(type)
{
//    LOGD << "--------- BaseService ---------";
//    m_service_data = new ServiceData(this);
    m_workerThread = new QThread();
    this->moveToThread(m_workerThread);
    connect(m_workerThread, &QThread::finished, m_workerThread, &QThread::deleteLater);
    connect(m_workerThread, &QThread::finished, this, &QObject::deleteLater);
    connect(m_workerThread, &QThread::started, this, &BaseService::started);
    connect(m_workerThread, &QThread::started, this, &BaseService::onSeparateThreadStarted);
    m_workerThread->start();
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
}

int BaseService::type()
{
    return m_type;
}

void BaseService::dispose()
{
    LOGD << "delete thread";
//    m_workerThread->terminate();
    m_workerThread->quit();
    m_workerThread->wait();
}

//void BaseService::setInfoClone(CloneInfo *clone)
//{
//    m_infoClone = clone;
//    m_worker->setInfoClone(m_infoClone);
//    m_service_data->set2Fa(m_infoClone->get2Fa());
//}

//CloneInfo *BaseService::getInfoClone() const
//{
//    return m_infoClone;
//}

void BaseService::forceStop()
{

}

void BaseService::onSeparateThreadStarted()
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

//void BaseService::onWorkerFinished()
//{
//    LOGD << "ngoc anh=============>onWorkerFinished: " << m_service_data->getServiceID();
//    emit serviceFinished(m_service_data->getServiceID());
//}
