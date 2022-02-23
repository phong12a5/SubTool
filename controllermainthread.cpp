#include "controllermainthread.h"
#include "controllerworkerthread.h"
#include <QtConcurrent/QtConcurrent>
#include "JlCompress.h"
#include "mainwindow.h"
#include "servicemanager.h"


// global variant
QMap<DOWNLOAD_TYPE, QString> downloadFileID {
#if __DEBUG_MODE__
    {DOWNLOAD_TYPE::FILE_VERSION, "1_4Qy62JBdyX7WvWIgMfPocF8s7KgjL4h"},
#else
    {DOWNLOAD_TYPE::FILE_VERSION, "1GtSEypl_vmw3LkGhK1aGJmFvy1JegbZr"},
#endif
    {DOWNLOAD_TYPE::FILE_RELEASE, ""}
};

ControllerMainThread* ControllerMainThread::m_InstancePtr = nullptr;

ControllerMainThread::ControllerMainThread(): m_initialize(false), m_appRunning(false), m_serviceRunning(false), m_updating(false)
{
    // signal slot of function signal, must be QueuedConnection to run in this thread
    connect(this, &ControllerMainThread::uiShowHideProcessDialog, this, &ControllerMainThread::uiShowHideProcessDialogHandle, Qt::QueuedConnection);
    connect(this, &ControllerMainThread::reloadTable, this, &ControllerMainThread::reloadTableHandle, Qt::QueuedConnection);
     connect(this, &ControllerMainThread::sigOnchangeReloadTable, this, &ControllerMainThread::onChangeReloadTableHandle, Qt::QueuedConnection);

    connect(this, &ControllerMainThread::setTimerStatus, this, &ControllerMainThread::onSetTimerStatus, Qt::QueuedConnection);


    m_startAppTimer.setInterval(180000);
    m_startAppTimer.setSingleShot(false);
    connect(&m_startAppTimer, &QTimer::timeout, this, &ControllerMainThread::onStartAppTimerTimeout);
#if __DEBUG_MODE__
    m_updateTimer.setInterval(150000);
#else
    m_updateTimer.setInterval(300000);
#endif
    m_updateTimer.setSingleShot(false);
    connect(&m_updateTimer, &QTimer::timeout, this, &ControllerMainThread::startCheckAppVersion);

    connect(&m_networkAccess, &NetworkAccessControllerSupporter::onCheckUserKeyApiTinSoft, this, &ControllerMainThread::checkUserKeyApiTinsoftFinished,Qt::QueuedConnection);
    connect(&m_networkAccess, &NetworkAccessControllerSupporter::onCheckProxyKeyApiTinSoft, this, &ControllerMainThread::checkProxyKeyApiTinsoftFinished,Qt::QueuedConnection);
    connect(&m_networkAccess, &NetworkAccessControllerSupporter::onGetProxyFromUserKeyApiTinSoft, this, &ControllerMainThread::getProxyFromUserKeyApiTinSoftFinished,Qt::QueuedConnection);

    connect(&m_networkAccess, &NetworkAccessControllerSupporter::onCheckUserKeyApiTm, this, &ControllerMainThread::checkUserKeyApiTmFinished);
    connect(&m_networkAccess, &NetworkAccessControllerSupporter::onGetProxyFromProxyKeyApiTm, this, &ControllerMainThread::getProxyFromProxyKeyApiTmFinished);


    connect(&m_downloadManager, &DownLoadManager::sigFileVersion, this, &ControllerMainThread::checkVerAppToUpdate, Qt::QueuedConnection);
    connect(&m_downloadManager, &DownLoadManager::sigFileRelease, this, &ControllerMainThread::updateAppRelease, Qt::QueuedConnection);

}

ControllerMainThread::~ControllerMainThread()
{
    ServiceManager::instance()->forceDeleteAllService();

    if(nullptr != m_mainUI) {
        delete m_mainUI;
        m_mainUI = nullptr;
    }
}

ControllerMainThread *ControllerMainThread::getInstance()
{
    if(nullptr == m_InstancePtr) {
        m_InstancePtr = new ControllerMainThread();
    }
    return m_InstancePtr;
}

void ControllerMainThread::deleteInstance()
{
    if(nullptr != m_InstancePtr) {
        delete m_InstancePtr;
        m_InstancePtr = nullptr;
    }
}

void ControllerMainThread::initialize()
{
    LOGD;
    if(m_initialize){
        return;
    }
    // create UI
    if(nullptr == m_mainUI) {
        SQLiteWorker::getInstance()->checkLogToClear();
        m_mainUI = new MainWindow();
        m_mainUI->initialize();
        m_mainUI->onAppRunningChanged(getAppRunning());
        uiUpdateStatusCloneToStatusBar();
    }

    // init app
    m_deviceName = "CHROME_" + QSysInfo::machineHostName();
    QString fileVerLink = QDir::currentPath() + "/downloads";
    QDir dir(fileVerLink);
    dir.removeRecursively();

    m_startAppTimer.start();
    m_updateTimer.start();
    startCheckAppVersion();
}

void ControllerMainThread::startApplication()
{
    if(nullptr != m_mainUI) {
        m_mainUI->show();
    }
}

void ControllerMainThread::uiSetStatusInfo(QString message)
{
    if(nullptr != m_mainUI) {
        m_mainUI->setStatusInfo(message);
    }
}

void ControllerMainThread::uiUpdateProcessDialogValue(int value)
{
    if(nullptr != m_mainUI) {
        m_mainUI->getProcessDownload()->setValue(value);
    }
}

void ControllerMainThread::uiUpdateProcessDialogTitle(QString title)
{
    if(nullptr != m_mainUI) {
        m_mainUI->getProcessDownload()->setTitle(title);
    }
}

int ControllerMainThread::uiGetNumberOfThread()
{
    if(nullptr == m_mainUI) {
        return 0;
    }
    return m_mainUI->getInteractConfig()->getThread()->numberThread;
}

void ControllerMainThread::startCloneByTinsoftProxyUserKeyApi(QString keyUser)
{
    LOGD;
    uiSetStatusInfo("Đang lấy proxy Tinsoft...");
    m_networkAccess.getProxyFromUserKeyApiTinsoft(keyUser);
}

void ControllerMainThread::startCloneByTmProxy(QString key)
{
    LOGD;
    uiSetStatusInfo("Đang lấy proxy Tm...");
    m_networkAccess.getProxyFromProxyKeyApiTm(key);
}

void ControllerMainThread::startCloneByXProxy(QString xproxy, int numThreadXProxy)
{
    setListProxy(xproxy.split("\n"));
    if(numThreadXProxy > 0){
        setNumThreadPerProxy(numThreadXProxy);
    } else {
        setNumThreadPerProxy(1);
    }
    CONTROLLERWORKER->startClone();
}

void ControllerMainThread::startCloneWithoutProxy()
{
    CONTROLLERWORKER->startClone();
}

void ControllerMainThread::checkUserKeyApiTinsoftAvailable(QString keyUser)
{
    m_networkAccess.checkUserKeyApiTinSoft(keyUser);
}

void ControllerMainThread::checkProxyKeyApiTinsoftAvailable(QString key)
{
    m_networkAccess.checkProxyKeyApiTinsoft(key);
}

void ControllerMainThread::checkApiTm(QString key)
{
    m_networkAccess.checkApiKeyTm(key);
}

void ControllerMainThread::uiUpdateStatusCloneToStatusBar()
{
    if(nullptr != m_mainUI) {
        QList<CommonInforClone *> listClone = SQLiteWorker::getInstance()->getListClone();
        int checkpoint = 0;
        int total = listClone.size();
        for(int i=0; i < total; i++) {
            CommonInforClone *clone = listClone[i];
            if(clone->getCheckPoint()){
                checkpoint++;
            }
        }
        m_mainUI->setCloneInfo(total,total-checkpoint,checkpoint);
    }
}

void ControllerMainThread::setAppRunning(const bool appRunning)
{
    LOGD << appRunning;
    QMutexLocker locker(&mutex);
    if(appRunning != m_appRunning) {
        m_appRunning = appRunning;
        emit appRunningChanged(appRunning);
        locker.unlock();

        if(appRunning) {
            emit setTimerStatus(&m_startAppTimer, false);
        } else {
            emit setTimerStatus(&m_startAppTimer, true);
        }
    }
}

bool ControllerMainThread::getAppRunning()
{
    bool running = false;
    QMutexLocker locker(&mutex);
    running = m_appRunning;
    return running;
}

void ControllerMainThread::setServiceRunning(const bool serviceRunning)
{
    LOGD << serviceRunning;
    QMutexLocker locker(&mutex);
    if(serviceRunning != m_serviceRunning) {
        m_serviceRunning = serviceRunning;
        emit serviceRunningChanged(serviceRunning);
        locker.unlock();

        if(!serviceRunning) {
            if(getUpdating()) {
                startUpdateVersion();
            }
        }
        if(!getAppRunning()) {
            uiSetStatusInfo("Đã dừng app");
        }
    }
}

bool ControllerMainThread::getServiceRunning()
{
    bool running = false;
    QMutexLocker locker(&mutex);
    running = m_serviceRunning;
    return running;
}

void ControllerMainThread::setUpdating(const bool updating)
{
    LOGD << updating;
    QMutexLocker locker(&mutex);
    if(updating != m_updating) {
        m_updating = updating;
        emit updatingChanged(updating);
    }
}

bool ControllerMainThread::getUpdating()
{
    bool updating = false;
    QMutexLocker locker(&mutex);
    updating = m_updating;
    return updating;
}

void ControllerMainThread::setProxyType(const PROXY_TYPE proxyType)
{
    LOGD << (int)proxyType;
    QMutexLocker locker(&mutex);
    if(proxyType != m_proxyType) {
        m_proxyType = proxyType;
    }
}

PROXY_TYPE ControllerMainThread::getProxyType()
{
    PROXY_TYPE proxyType = PROXY_TYPE::NONE_PROXY;
    QMutexLocker locker(&mutex);
    proxyType = m_proxyType;
    return proxyType;
}

void ControllerMainThread::setProxyKind(const PROXY_KIND proxyKind)
{
    LOGD << (int)proxyKind;
    QMutexLocker locker(&mutex);
    if(proxyKind != m_proxyKind) {
        m_proxyKind = proxyKind;
    }
}

PROXY_KIND ControllerMainThread::getProxyKind()
{
    PROXY_KIND proxyKind = PROXY_KIND::HTTP;
    QMutexLocker locker(&mutex);
    proxyKind = m_proxyKind;
    return proxyKind;
}

QString ControllerMainThread::getProxy()
{
    QString proxy;
    switch (getProxyType()) {
    case PROXY_TYPE::TINSOFT_PROXY:
        proxy = m_networkAccess.getProxyTinsoft();
        break;
    case PROXY_TYPE::TMPROXY:
        proxy = m_networkAccess.getProxyTm(getProxyKind());
        break;
    case PROXY_TYPE::XPROXY:
        proxy = getProxyFromListProxy();
        break;
    case PROXY_TYPE::NONE_PROXY:
    default:
        break;
    }
    LOGD << proxy;
    return proxy;
}

void ControllerMainThread::stop()
{
    setAppRunning(false);
}

QString ControllerMainThread::getDeviceName() const
{
    return m_deviceName;
}

void ControllerMainThread::startUpdateVersion()
{
    LOGD;
    m_downloadManager.startDownload(DOWNLOAD_TYPE::FILE_RELEASE);
}

void ControllerMainThread::setListProxy(QStringList listProxy)
{
    LOGD << listProxy;
    QMutexLocker locker(&mutex);
    if(listProxy != m_listProxy) {
        m_proxyCount = 0;
        m_listProxy = listProxy;
    }
}

QStringList ControllerMainThread::getListProxy()
{
    QStringList listProxy;
    QMutexLocker locker(&mutex);
    listProxy = m_listProxy;
    return listProxy;
}

QString ControllerMainThread::getProxyFromListProxy()
{
    QString proxy;
    QMutexLocker locker(&mutex);
    int size = m_listProxy.size();
    if(m_listProxy.isEmpty() == false) {
        LOGD << "list not empty";
        proxy = m_listProxy.at(0);
        m_proxyCount++;
        if(m_proxyCount >= m_numThreadPerProxy) {
            m_proxyCount = 0;
            if(size > 1){
                m_listProxy.move(0, (size-1));
            }
        }
    }
    return proxy;
}

void ControllerMainThread::setNumThreadPerProxy(int numThread)
{
    LOGD << numThread;
    QMutexLocker locker(&mutex);
    if(numThread != m_numThreadPerProxy) {
        m_numThreadPerProxy = numThread;
    }
}

int ControllerMainThread::getNumThreadPerProxy()
{
    int numThread;
    QMutexLocker locker(&mutex);
    numThread = m_numThreadPerProxy;
    return numThread;
}

void ControllerMainThread::onStartAppTimerTimeout()
{
    if(!getAppRunning()){
        m_mainUI->doAction();
    }
}

void ControllerMainThread::startCheckAppVersion()
{
    LOGD;
    m_downloadManager.startDownload(DOWNLOAD_TYPE::FILE_VERSION);
}

void ControllerMainThread::checkVerAppToUpdate()
{
    QString inforVer;
#if __DEBUG_MODE__
    QString fileVerLink = QDir::currentPath() + "/downloads/ver-test.txt";
#else
    QString fileVerLink = QDir::currentPath() + "/downloads/updateVer.txt";
#endif
    LOGD << "fileVerLink: " << fileVerLink;
    QFile file(fileVerLink);
    if(!file.open(QIODevice::ReadOnly)) {
        //        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        inforVer = line;
    }

    file.close();
    LOGD << "checkVerAppToupdate: " << inforVer;
    QStringList fields = inforVer.split("|");
    if(fields.size() == 2) {
        if(fields.at(0) != APP_VER) {
            m_updateTimer.stop();
            LOGD << "update new version: " << fields.at(0);
            downloadFileID[DOWNLOAD_TYPE::FILE_RELEASE] = fields.at(1);
            setUpdating(true);
            uiSetStatusInfo("Update version mới, vui lòng không thao tác trong quá trình update!");
            stop();
            if(!getServiceRunning()){
                LOGD << "update";
                startUpdateVersion();
            } else {
                LOGD << "nothings";
            }
        } else {
            LOGD << "not found new ver";
        }
    } else {
        LOGD << "receive wrong format";
    }
}

void ControllerMainThread::updateAppRelease()
{
    LOGD;
    QString fileUnzip = "";
    QString fileVerLink = QDir::currentPath() + "/downloads";
    QDir dir(fileVerLink);
    QStringList finters;
    finters << "*.zip";
    dir.setNameFilters(finters);
    QFileInfoList files = dir.entryInfoList();
    QStringList lisImage;
    foreach (QFileInfo fileInfo, files) {
        QString path=fileInfo.absoluteFilePath();
        lisImage.append(path);
        QFile file(path);
        if(!file.exists()){
            LOGD << "This file is not exist";
        }else{
            LOGD << "This file was found " << file.fileName();
            fileUnzip = file.fileName();
        }
    }
    if(fileUnzip.contains(".zip")) {
        LOGD << "start unzip";
        QtConcurrent::run([=](){
            QString extractDir = QDir::currentPath();
            if(!QDir(QDir::currentPath() + "/tmp/").exists())
            {
                QDir::current().mkpath(QDir::currentPath() + "/tmp/");
            }
            extractDir = QDir::currentPath() + "/tmp/";

            JlCompress::extractDir(fileUnzip, extractDir);
        });
    }
    QProcess *proc = new QProcess();
    QString appName;
#ifdef __DEBUG_MODE__
    appName = "debug/F-Care";
#else
    appName = "F-Care";
#endif
    proc->setWorkingDirectory(QDir::currentPath() + "/updater/");
    proc->setProgram("cmd.exe");
    proc->setArguments(QStringList()<<"/c"<<"START"
                               << "F_Updater.exe"
                               << appName
                               << QString("'") + QDir::toNativeSeparators(QDir::currentPath()) + QString("'")
                               << QString("'") + QDir::toNativeSeparators(QDir::currentPath() + "/tmp/") + QString("'"));
    connect(proc, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error){
        LOGD << error;
        uiSetStatusInfo("Update fail, thiếu app update!");
        setUpdating(false);
        m_updateTimer.start();
    });
    proc->start();
    proc->waitForFinished();
    delete proc;
}

bool ControllerMainThread::checkUserKeyApiTinsoftFinished(QNetworkReply *reply)
{
    bool turnBack = false;
    QString answer = reply->readAll();
    LOGD << "controler API user proxy json: " << answer;
    QJsonDocument dataUser = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject getDataUser = dataUser.object();
    QJsonValue dataJson = getDataUser.value(QString("data"));
    int leng = dataJson.toArray().size();
    for (int i = 0; i < leng; i++) {
        qDebug() << "anhdxv log check";
        QJsonValue checkKeySusses = getDataUser.value(QString("data"))[i].operator[]("success");
        QJsonValue key = getDataUser.value(QString("data"))[i].operator[]("key");
        if(checkKeySusses.toBool() == true) {
            //anhdvx gọi show popup (bạn có thể sử dụng User api này)
            turnBack = true;
            break;
        } else {
            //anhdxv gọi show popup (API này đã hết hạn hoặc không tồn tại)
            //qDebug() << "anhdxv log check";
            turnBack = false;
        }
    }
    if (turnBack == true) {
        uiSetStatusInfo("Bạn có thể sử dụng User api này");
    } else {
        uiSetStatusInfo("User API này đã hết hạn hoặc không tồn tại");
    }
    qDebug()<< " turnBack " << turnBack;
    return turnBack;
}

bool ControllerMainThread::checkProxyKeyApiTinsoftFinished(QNetworkReply *reply)
{
    QString answer = reply->readAll();
    LOGD << "controler API proxy: " << answer;
    QJsonDocument dataProxy = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject getDataProxy = dataProxy.object();
    QJsonValue dataJson = getDataProxy.value(QString("success"));
    QJsonValue getProxy = getDataProxy.value(QString("proxy"));
    LOGD << "dataJson: " << dataJson << "| getProxy: " << getProxy;
    if(dataJson.toBool() == true) {
        LOGD << "getProxy.toString: " << getProxy.toString();
        // anhdxv bạn có thể sử API proxy này
         uiSetStatusInfo("Bạn có thể sử dụng API proxy này");
        return true;
    } else {
        //anhdxv api proxy này không khả dụng
         uiSetStatusInfo("API proxy này đã hết hạn hoặc không tồn tại");
        return false;
    }
}

void ControllerMainThread::getProxyFromUserKeyApiTinSoftFinished()
{
    LOGD;
    CONTROLLERWORKER->startClone();
}

void ControllerMainThread::checkUserKeyApiTmFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError){
        QString response = QString::fromUtf8(reply->readAll());
        LOGD << "response: " << response;
        QJsonDocument dataResponseObj = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject dataResponse = dataResponseObj.object();
        QString message = dataResponse.value(QString("message")).toString();
        if(message.isEmpty()){
            uiSetStatusInfo("Bạn có thể sử dụng API proxy này");
        } else {
            uiSetStatusInfo("API proxy này đã hết hạn hoặc không tồn tại");
        }
    }
    else{
        QString err = reply->errorString();
        LOGD << "Request tm server error: " << err;
    }
}

void ControllerMainThread::getProxyFromProxyKeyApiTmFinished(bool success, QString err)
{
    if(success){
        LOGD << "get api success";
        CONTROLLERWORKER->startClone();
    } else {
        LOGD << "request api fail" << err;
        uiSetStatusInfo("Lấy proxy tm thất bại!");
        setAppRunning(false);
    }
}

void ControllerMainThread::onSetTimerStatus(QTimer *timer, bool status)
{
    if(status){
        timer->start();
    } else {
        timer->stop();
    }
}

void ControllerMainThread::uiShowHideProcessDialogHandle(bool show)
{
    if(nullptr != m_mainUI) {
        if(show) {
            m_mainUI->getProcessDownload()->show();
        } else {
            m_mainUI->getProcessDownload()->hide();
        }
    }
}

void ControllerMainThread::reloadTableHandle()
{
    LOGD;
    if(nullptr != m_mainUI) {
        m_mainUI->reloadTable();
    }
    LOGD << "finish";
}

void ControllerMainThread::onChangeReloadTableHandle(QString cloneId)
{
    LOGD;
    if(nullptr != m_mainUI) {
        m_mainUI->onChangeReloadTable(cloneId);
    }
    LOGD << "finish";
}
