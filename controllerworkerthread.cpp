#include "controllerworkerthread.h"
#include "controllermainthread.h"
#include "log.h"
#include "fdriver.h"
#include <QThread>
#include <QNetworkReply>
#include <QSysInfo>
#include "servicemanager.h"
#include "chromeservice.h"
#include "servicedata.h"
//#include "fity.one-api/fcare/include/WebAPI.hpp"
#include "autoFarmerAPI/include/WebAPI.hpp"

ControllerWorkerThread* ControllerWorkerThread::m_InstancePtr = nullptr;

ControllerWorkerThread::ControllerWorkerThread()
{
    // all connection of worker thread must be QueuedConnection to run in worker thread
    // signal slot of function signal
    connect(this, &ControllerWorkerThread::initialize, this, &ControllerWorkerThread::initializeHandle, Qt::BlockingQueuedConnection);
    connect(this, &ControllerWorkerThread::startClone, this, &ControllerWorkerThread::startCloneHandle, Qt::QueuedConnection);

    connect(ServiceManager::instance(), &ServiceManager::runCloneFinished, this, &ControllerWorkerThread::onRunCloneFinished, Qt::QueuedConnection);

    this->moveToThread(&m_workerThread);
    m_workerThread.start();
}

ControllerWorkerThread *ControllerWorkerThread::getInstance()
{
    if(nullptr == m_InstancePtr) {
        m_InstancePtr = new ControllerWorkerThread();
    }
    return m_InstancePtr;
}

ControllerWorkerThread::~ControllerWorkerThread()
{
    m_workerThread.quit();
    m_workerThread.wait();
    m_workerThread.deleteLater();
}

void ControllerWorkerThread::deleteInstance()
{
    if(nullptr != m_InstancePtr) {
        delete m_InstancePtr;
        m_InstancePtr = nullptr;
    }
}

void ControllerWorkerThread::initializeHandle()
{
    LOGD;
    if(m_initialize){
        LOGD << "Init before!";
        return;
    }
    LOGD << "Start Init";
}

void ControllerWorkerThread::startCloneHandle()
{
    LOGD;
    CONTROLLERMAIN->uiSetStatusInfo("Đang đồng bộ hệ thống");
    if(FDriver::unlockFDriver("0399843737") || 1) {
        registerDevice();
        CONTROLLERMAIN->uiSetStatusInfo("Đang chạy app");

        LOGD;
        m_chromeDriverProcess = new QProcess(this);

        m_chromeDriverProcess->setWorkingDirectory(QDir::currentPath());
        m_chromeDriverProcess->setProgram("chromedriver.exe");
        connect(m_chromeDriverProcess, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error){
            LOGD << error;
        });
        m_chromeDriverProcess->start();
        m_chromeDriverProcess->waitForFinished();

        onRunCloneFinished();
    } else {
        LOGD << "unlock Fdriver failed";
        CONTROLLERMAIN->uiSetStatusInfo("unlock Fdriver failed");
    }
}

void ControllerWorkerThread::onRunCloneFinished(QString lastClone)
{
    LOGD << "start onRunCloneFinished " << lastClone;

    if(!CONTROLLERMAIN->getAppRunning()){
        LOGD << "stop app";
        return;
    }

    QString lastRunningClone = lastClone;
    int countService = ServiceManager::instance()->countService();
    int numberThread = CONTROLLERMAIN->uiGetNumberOfThread();
    QString linkProfile = SQLiteWorker::getInstance()->getProfilePath();
    LOGD << "countService: " << countService;
    LOGD << "numberThread: " << numberThread;
    while(countService < numberThread) {
        if(getCloneToServer()){
            LOGD << "getCloneToServer " << m_cloneRun->getCloneId();
            updateCloneInfo( "stored",
                             m_cloneRun->getCloneId(),
                             m_cloneRun->getUID(),
                             m_cloneRun->getPassword());
            LOGD << "onRunCloneFinished";
        } else if(getCloneToRunFromDB(lastRunningClone)) {
            LOGD << "getCloneToRunFromDB " << m_cloneRun->getCloneId();
            m_cloneRun->checkLogToClear();
        } else {
            LOGD << "No clone for run!";
            if(!CONTROLLERMAIN->getServiceRunning()) {
//                CONTROLLERMAIN->uiSetStatusInfo("Không lấy được clone nào trên server và không có clone nào store");
                CONTROLLERMAIN->setAppRunning(false);
            }
            break;
        }
        if(m_cloneRun->getUserAgent().isEmpty()) {
            m_cloneRun->setUserAgent(getUserAgent());
        }
        LOGD << "Run clone " << m_cloneRun->getCloneId();
        int serviceID = ServiceManager::instance()->createService<ChromeService>();
        LOGD << "Service ID: " << serviceID;
        ChromeService* service = ServiceManager::instance()->getService<ChromeService>(serviceID);
        service->setInfoClone(m_cloneRun);
        service->model()->setNumberChrom(m_countChrom);
        service->model()->setServiceID(serviceID);
        service->model()->setProxyKind(CONTROLLERMAIN->getProxyKind());

        // get proxy
        service->model()->setProxyChromWorker(CONTROLLERMAIN->getProxy());
        service->model()->setLinkProfile(linkProfile);

        service->startService(m_cloneRun->getUID());
        if(m_countChrom <= 6) {
            m_countChrom++;
        } else {
            m_countChrom = 0;
        }
        lastRunningClone = m_cloneRun->getCloneId();
        countService++;
    }
    LOGD << "end onRunCloneFinished";
}

void ControllerWorkerThread::registerDevice()
{
    QByteArray mac = CONTROLLERMAIN->getDeviceName().toLocal8Bit();
    char *macData = mac.data();
    LOGD << "macAdress: " << macData;
    try {
        LOGD << "------initWebAPIs----start";
//        DEVICE_INFO deviceInfo;
//        deviceInfo.device_id = macData;
//        deviceInfo.app_version_name = (char*)APP_VER;
//        WebAPI::getInstance()->initWebAPIs(SQLiteWorker::getInstance()->getToken().toLocal8Bit(), deviceInfo);
        WebAPI::getInstance()->initWebAPIs(nullptr, SQLiteWorker::getInstance()->getToken().toLocal8Bit(), "Chrome", macData, (char*)APP_VER);
        LOGD << "------initWebAPIs----fnish";
//        WebAPI::getInstance()->upsertDevice();
        WebAPI::getInstance()->upsertDevice(nullptr, nullptr);
        LOGD << "------upsertDevice----start";
//        WebAPI::getInstance()->getConfig();
        WebAPI::getInstance()->getConfig(nullptr);
        LOGD << "------getConfig----start";
    } catch (const std::exception& ex) {
        LOGD << "registerDevice Web api error";
    } catch (const std::string& ex) {
        LOGD << "registerDevice Web api error";
    } catch (...) {
        LOGD << "registerDevice Web api error";
    }

}

bool ControllerWorkerThread::getCloneToServer()
{
    try {
        CommonInforClone* newClone;
        QString dataJson = "";
        QString UID = "";
        QString passWord = "";
        QString haiFa = "";
        QString idClone = "";
        QString cookies = "";
        QString message = "";
        LOGD << "------getClone----start";
//        std::string infoClone = WebAPI::getInstance()->getClone();
        std::string infoClone = WebAPI::getInstance()->getClone(nullptr, "facebook");
        LOGD << "------getClone----finish";
        LOGD << "getCloneToServer: " << infoClone.c_str();
        QJsonDocument dataClone = QJsonDocument::fromJson(QString::fromStdString(infoClone).toUtf8());
        QJsonObject getDataClone = dataClone.object();

//        UID = getDataClone.value(QString("data")).operator[]("uid").toString();
//        passWord = getDataClone.value(QString("data")).operator[]("password").toString();
//        haiFa = getDataClone.value(QString("data")).operator[]("secretkey").toString();
//        idClone = getDataClone.value(QString("data")).operator[]("id").toString();
//        cookies = getDataClone.value(QString("data")).operator[]("cookie").toString();
//        message = getDataClone.value(QString("message")).toString();
//        QJsonObject dataJsonObj = getDataClone.value(QString("data")).toObject();

        UID = getDataClone.value(QString("cloneInfo")).operator[]("uid").toString();
        passWord = getDataClone.value(QString("cloneInfo")).operator[]("password").toString();
        haiFa = getDataClone.value(QString("cloneInfo")).operator[]("secretkey").toString();
        idClone = getDataClone.value(QString("cloneInfo")).operator[]("id").toString();
        cookies = getDataClone.value(QString("cloneInfo")).operator[]("cookie").toString();
        message = getDataClone.value(QString("message")).toString();
        QJsonObject dataJsonObj = getDataClone.value(QString("cloneInfo")).toObject();
        QJsonDocument cloneJsonDoc(dataJsonObj);
        std::string cloneJsonPathToString(cloneJsonDoc.toJson(QJsonDocument::Compact));
        LOGD << "infoClone: " << UID << "|" << passWord << "|" << haiFa << "|" << idClone ;
        LOGD << "getCloneToServer dataJson: " << cloneJsonPathToString.c_str();

        if(getDataClone.value(QString("code")).toInt() == 200) {
            LOGD << "info clone: " << idClone << "|" << UID << "|" << passWord << "|" << haiFa;
            newClone = new CommonInforClone(idClone, UID , passWord, haiFa, "", cookies);
            SQLiteWorker::getInstance()->addClone(newClone);
            m_cloneRun = newClone;
            m_cloneRun->setInfoJsonClone(cloneJsonPathToString.c_str());
            return true;
        } else if(getDataClone.value(QString("code")).toInt() == 400 && message.contains("is not approved")){
            QString requestApprove = "vui lòng approve device " + CONTROLLERMAIN->getDeviceName() + " và setup hành động clone trên hệ thống!!!";
            CONTROLLERMAIN->uiSetStatusInfo(requestApprove);
            return false;
        } else if(getDataClone.value(QString("code")).toInt() == 400 && message.contains("Not found Clone live")){
//            CONTROLLERMAIN->uiSetStatusInfo("vui lòng setup device và hành động cho clone trên hệ thống!!!");
            return false;
        } else {
            return false;
        }
    } catch (const std::exception& ex) {
        LOGD << "getCloneToServer Web api error";
    } catch (const std::string& ex) {
        LOGD << "getCloneToServer Web api error";
    } catch (...) {
        LOGD << "getCloneToServerWeb api error";
    }
    return false;

}

void ControllerWorkerThread::updateCloneInfo(QString status, QString idClone, QString UID, QString passWork)
{
    LOGD << "updateCloneInfo data json: " << m_cloneRun->getInfoJsonClone();
    QJsonObject rootObj = QJsonDocument::fromJson(m_cloneRun->getInfoJsonClone().toUtf8()).object();
    rootObj["alive_status"] = status;
    QJsonDocument cloneJsonDoc(rootObj);

    std::string cloneJsonPathToString(cloneJsonDoc.toJson(QJsonDocument::Compact));
    LOGD << "cloneJsonPathToString: " << cloneJsonPathToString.c_str();
    m_cloneRun->setInfoJsonClone(cloneJsonPathToString.c_str());
    LOGD << "------updateClone----start";
//    WebAPI::getInstance()->updateClone("UpdateAliveStatus", cloneJsonPathToString.c_str());
    WebAPI::getInstance()->updateClone(nullptr, "UpdateAliveStatus", "facebook", cloneJsonPathToString.c_str());
    LOGD << "------updateClone----finish";

}

QString ControllerWorkerThread::getUserAgent()
{
    QString userAgen;
    QFile file;
    file.setFileName(":/model/userAgent.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    userAgen = file.readAll();
    file.close();

    QJsonDocument jsonDocUserAgen = QJsonDocument::fromJson(userAgen.toUtf8());
    QJsonObject jsonUserAgent = jsonDocUserAgen.object();
    QJsonValue infoUserAgent = jsonUserAgent.value(QString("useragent"));
    QJsonArray listUserAgen = infoUserAgent.toArray();
    int leng = infoUserAgent.toArray().size() - 1;
    srand(time(NULL));
    int res = rand() % leng;
    return listUserAgen.at(res).toString();
}

QString ControllerWorkerThread::getDoAcction(QString idClone)
{
//    std::string doaction = WebAPI::getInstance()->doAction(idClone.toStdString().c_str());
    std::string doaction;
    LOGD << "doaction: " << doaction.c_str();
    return (QString::fromStdString(doaction));
}

bool ControllerWorkerThread::getCloneToRunFromDB(QString lastClone)
{
    QList<QString> listDeleteClone;
    QList<CommonInforClone *> listClone = SQLiteWorker::getInstance()->getListClone();
    bool findLastClone = lastClone.isEmpty() ? true : false;
    bool findCloneForRun = false;
    for(int i=0; i < listClone.size(); i++) {
        CommonInforClone *clone = listClone[i];
        if(clone->getCheckPoint() && clone->getSttUncheckPoint() == STATUS_UNCHECKPOINT::UNSUCCESS){
            if(!findCloneForRun) {
                if (!checkCloneStoreOnServer(clone)) {
                    listDeleteClone.append(clone->getCloneId());
                }
            }
            continue;
        }
        if(ServiceManager::instance()->checkCloneWorking(clone->getCloneId())){
            continue;
        }
        if(!findCloneForRun) {
            if (!checkCloneStoreOnServer(clone)) {
                listDeleteClone.append(clone->getCloneId());
            } else {
                m_cloneRun = clone;
                findCloneForRun = true;
            }
        }
        if(lastClone == clone->getCloneId()) {
            findLastClone = true;
            continue;
        }

        if(findLastClone) {
            if (!checkCloneStoreOnServer(clone)) {
                listDeleteClone.append(clone->getCloneId());
            } else {
                m_cloneRun = clone;
                break;
            }
        }
    }
    foreach(QString cloneId, listDeleteClone) {
        SQLiteWorker::getInstance()->deleteCloneById(cloneId);
    }
    return findCloneForRun;
}

bool ControllerWorkerThread::checkCloneStoreOnServer(CommonInforClone *clone)
{
    QJsonObject cloneJsonPath;
    cloneJsonPath.insert("alive_status", "stored");
    cloneJsonPath.insert("appname", "facebook");
    cloneJsonPath.insert("id", clone->getCloneId());
    cloneJsonPath.insert("uid", clone->getUID());
    cloneJsonPath.insert("password", clone->getPassword());
    QJsonDocument cloneJsonDoc(cloneJsonPath);
    std::string cloneJsonPathToString(cloneJsonDoc.toJson(QJsonDocument::Compact));
//    std::string infoClone = WebAPI::getInstance()->getCloneInfo(cloneJsonPathToString.c_str());
    std::string infoClone;
    LOGD << "---checkCloneStoreOnServer: " << infoClone.c_str();
    QJsonDocument dataInfoClone = QJsonDocument::fromJson(QString::fromStdString(infoClone).toUtf8());
    QJsonObject getDataInfoClone = dataInfoClone.object();
    QJsonValue code = getDataInfoClone.value(QString("code"));
    QJsonValue message = getDataInfoClone.value(QString("message"));
    if(code.toInt() == 200 && message.toString() == "Success") {
        LOGD << "get clone from server sucess";
        return true;
    } else if(code.toInt() == 400 && message.toString() == "Clone not exist") {
        LOGD << "Clone not exist on server";
        CONTROLLERMAIN->uiSetStatusInfo("không tìm thấy clone " + clone->getUID() + " trên hệ thống");
        return false;
    } else {
        LOGD << "Not handle";
        return true;
    }
}
