#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QCloseEvent>
#include <QSettings>
#include <QObject>
#include "log.h"
//#include "fity.one-api/fcare/include/WebAPI.hpp"
#include "autoFarmerAPI/include/WebAPI.hpp"
#include "AppDefine.h"
#include "controllermainthread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    viewClone = new ViewClone();
    connect(viewClone, SIGNAL(closeSubWindow()), this, SLOT(closeViewCloneMenu()));
    QObject::connect(viewClone, &ViewClone::rowSelected,this, &MainWindow::showStatusClone);
    windowViewClone = loadSubWindow(viewClone);
    windowViewClone->show();

    cloneConfig = new CloneConfig();
    connect(cloneConfig, SIGNAL(closeSubWindow()), this, SLOT(closeCloneConfigMenu()));
    windowClone = loadSubWindow(cloneConfig);

    proxyConfig = new ProxyConfig();
    connect(proxyConfig, SIGNAL(closeSubWindow()), this, SLOT(closeProxyConfigMenu()));
    windowProxy = loadSubWindow(proxyConfig);

    interactConfig = new InteractConfig();
    connect(interactConfig, SIGNAL(closeSubWindow()), this, SLOT(closeInteractConfigMenu()));
    windowInteract = loadSubWindow(interactConfig);

    checkPoint = new CheckPoint();
    connect(checkPoint, SIGNAL(closeSubWindow()), this, SLOT(closeCheckPoint()));
    windowCheckPoint = loadSubWindow(checkPoint);

    pixmapPlay = QPixmap(":images/round-play-button.png").scaled(30,30);
    pixmapStop = QPixmap(":images/icons8_stop_1.ico").scaled(30,30);
    pixmapToken = QPixmap(":/images/icons8_settings.png").scaled(30,30);
    pixmapTokenDisplay = QPixmap(":/images/icons8_settings_disable").scaled(30,30);

    action = new QPushButton(this);
    action->setIconSize(QSize(pixmapPlay.width(), pixmapPlay.height()));
    action->setIcon(pixmapPlay);
    ui->toolBar->addWidget(action);
    action->setStyleSheet("border-style: outset;border-width: 0px;padding: 0px;");
    connect(action,SIGNAL(clicked()),this, SLOT(doAction()));

    labelPlayStop = new QPushButton(this);
    labelPlayStop->setText("Play");
    labelPlayStop->setStyleSheet("border-style: outset;border-width: 0px;padding: 0px;");
    ui->toolBar->addWidget(labelPlayStop);
    connect(labelPlayStop,SIGNAL(clicked()),this, SLOT(doAction()));

    ui->toolBar->addSeparator();

    token = new QPushButton(this);
    token->setIconSize(QSize(pixmapToken.width(), pixmapToken.height()));
    token->setIcon(pixmapToken);
    ui->toolBar->addWidget(token);
    token->setStyleSheet("border-style: outset;border-width: 0px;padding: 0px;");
    connect(token,SIGNAL(clicked()),this, SLOT(doToken()));

    labelToken = new QPushButton(this);
    labelToken->setText("Token");
    ui->toolBar->addWidget(labelToken);
    labelToken->setStyleSheet("border-style: outset;border-width: 0px;padding: 0px;");
    connect(labelToken,SIGNAL(clicked()),this, SLOT(doToken()));

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // toolBar is a pointer to an existing toolbar
    ui->toolBar->addWidget(spacer);

    tokenDialog = new TokenDialog(this);
    connect(tokenDialog, SIGNAL(token(QString, bool)), this, SLOT(setToken(QString, bool)));
    tokenLable = new QLabel();
    ui->toolBar->addWidget(tokenLable);

    statusInfo = new QLabel();
    ui->statusbar->addPermanentWidget(statusInfo,10);
    setStatusInfo(" ");

    cloneInfo = new QLabel();
    ui->statusbar->addPermanentWidget(cloneInfo, 4);
    setCloneInfo(0,0,0);

    processDownload = new ProcessDownload();

    versionInfo = new QLabel();
    QString ver = APP_VER;
    versionInfo->setText("version "+ver);
    versionInfo->setStyleSheet("color : #8f8f8f");
    versionInfo->setAlignment(Qt::AlignBottom);
    ui->statusbar->addPermanentWidget(versionInfo, 1);

    infoVersion = new InfoVersion(this);
}
MainWindow::~MainWindow()
{
    LOGD << "huy mainwindow ___";



    delete ui;
    CONTROLLERMAIN->stop();

}

void MainWindow::reloadTable()
{
    viewClone->reloadTable();
}

void MainWindow::onChangeReloadTable(QString cloneId)
{
     viewClone->onChangeReloadTable(cloneId);
}

void MainWindow::showTable()
{
    windowViewClone->show();
}

InteractConfig *MainWindow::getInteractConfig() const
{
    return interactConfig;
}

CloneConfig *MainWindow::getCloneConfig() const
{
    return cloneConfig;
}

ProxyConfig *MainWindow::getProxyConfig() const
{
    return proxyConfig;
}

ViewClone *MainWindow::getViewClone() const
{
    return viewClone;
}

void MainWindow::closeInteractConfigMenu()
{
    ui->actionConfigInteract->setEnabled(true);
    windowInteract->hide();
}

void MainWindow::closeCheckPoint()
{
    ui->actionCheckPoint->setEnabled(true);
    windowCheckPoint->hide();
}

void MainWindow::closeProxyConfigMenu()
{
    ui->actionConfigProxy->setEnabled(true);
    windowProxy->hide();
}

void MainWindow::closeCloneConfigMenu()
{
    ui->actionConfigClone->setEnabled(true);
    windowClone->hide();
}

void MainWindow::closeViewCloneMenu()
{
    windowViewClone->hide();
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    IPConfig iPConfig = proxyConfig->getIPConfig();
    DcomSetting dcomSetting = proxyConfig->getDcomSetting();
    TinSoftProxySetting tinSoftProxySetting = proxyConfig->getTinSoftProxySetting();
    TmProxySetting tmProxySetting = proxyConfig->getTmProxySetting();
    XProxyShoplikeSetting xProxyShoplikeSetting = proxyConfig->getXProxyShoplikeSetting();
    Proxyv6Setting proxyv6Setting = proxyConfig->getProxyv6Setting();
    XProxySetting xProxy = proxyConfig->getXProxySetting();

    BaseConfig *baseConfig = interactConfig->getBaseConfig();
    Thread *thread = interactConfig->getThread();

    SQLiteWorker::getInstance()->setNumberThread(thread->numberThread);
    SQLiteWorker::getInstance()->setProfilePath(baseConfig->profileLink);

    SQLiteWorker::getInstance()->setCheckIpBeforeRun(iPConfig.checkIpBeforeRun);
    SQLiteWorker::getInstance()->setDelayIpAfterChanged(iPConfig.delayIpAfterChanged);
    SQLiteWorker::getInstance()->setNumberTurnToChangeIp(iPConfig.numberTurnToChangeIp);
    SQLiteWorker::getInstance()->setChangeIp(iPConfig.changeIP);
    SQLiteWorker::getInstance()->setUseProxy(iPConfig.useProxy);
    SQLiteWorker::getInstance()->setChangeIpHma(iPConfig.changeIpHMA);

    SQLiteWorker::getInstance()->setNommalSettingDcom(dcomSetting.nommalSettingDcom);
    SQLiteWorker::getInstance()->setHilinkSettingDcom(dcomSetting.hilinkSettingDcom);
    SQLiteWorker::getInstance()->setNameDcom(dcomSetting.nameDcom);
    SQLiteWorker::getInstance()->setLinkUrl(dcomSetting.linkUrl);

    SQLiteWorker::getInstance()->setUseApiUser(tinSoftProxySetting.useAPIUser);
    SQLiteWorker::getInstance()->setUseApiProxy(tinSoftProxySetting.useAPIProxy);
    SQLiteWorker::getInstance()->setApiUser(tinSoftProxySetting.apiUser);
    SQLiteWorker::getInstance()->setApiProxy(tinSoftProxySetting.apiProxy);
    SQLiteWorker::getInstance()->setLocation(tinSoftProxySetting.location);
    SQLiteWorker::getInstance()->setNumberThreadTinSoft(tinSoftProxySetting.numberThread);

    SQLiteWorker::getInstance()->setApiKeyTm(tmProxySetting.apiKey);
    SQLiteWorker::getInstance()->setNumberThreadTm(tmProxySetting.numberThread);
    SQLiteWorker::getInstance()->setHttpTmproxy(tmProxySetting.httpProxy);
    SQLiteWorker::getInstance()->setSocketTmproxy(tmProxySetting.socketProxy);

    SQLiteWorker::getInstance()->setApiKeyXproxyShoplike(xProxyShoplikeSetting.apiKey);
    SQLiteWorker::getInstance()->setNumberThreadXproxyShoplike(xProxyShoplikeSetting.numberThread);

    SQLiteWorker::getInstance()->setApiKeyProxyv6(proxyv6Setting.apiKey);
    SQLiteWorker::getInstance()->setTypeHttpProxyv6(proxyv6Setting.typeHttp);
    SQLiteWorker::getInstance()->setTypeSock5sProxyv6(proxyv6Setting.typeSock5s);
    SQLiteWorker::getInstance()->setProxyv6(proxyv6Setting.proxy);
    SQLiteWorker::getInstance()->setNumberThreadProxyv6(proxyv6Setting.numberThread);

    SQLiteWorker::getInstance()->setHttpXroxy(xProxy.httpXproxy);
    SQLiteWorker::getInstance()->setSocksXroxy(xProxy.socksXproxy);
    SQLiteWorker::getInstance()->setXproxy(xProxy.proxy);
    SQLiteWorker::getInstance()->setNumberThreadXproxy(xProxy.numberThread);
    CONTROLLERMAIN->stop();

}

// display subwindow into mdiarea
QMdiSubWindow* MainWindow::loadSubWindow(QWidget* widget)
{
    QMdiSubWindow * window;
    window = ui->mdiArea->addSubWindow(widget);
    window->setWindowTitle(widget->windowTitle());
    window->setWindowIcon(widget->windowIcon());
    window->resize(widget->size());
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->hide();
    return window;

}

// click "Cấu hình clone" trên menu
void MainWindow::on_actionConfigClone_triggered()
{
    windowClone->show();
    ui->actionConfigClone->setEnabled(0);
}

// click "Cấu hình proxy" trên menu
void MainWindow::on_actionConfigProxy_triggered()
{
    windowProxy->show();
    ui->actionConfigProxy->setEnabled(0);
}

// click "Cấu hình tương tác"
void MainWindow::on_actionConfigInteract_triggered()
{
    windowInteract->show();
    ui->actionConfigInteract->setEnabled(0);
}

void MainWindow::on_actionTile_triggered()
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_actionCascade_triggered()
{
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::getTextFromFile()
{
    QString filter = "All File (*.*) ;; Text File (*.txt) ;; XML File (*.xml)";
    QString filename = QFileDialog::getOpenFileName(this, "open a file", "C://", filter);
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        //        QMessageBox::warning(this, "title", "file not open");
    }
    QTextStream in ( & file);
    QString text = in .readAll();

    list = text.split("\n");
    QList<CommonInforClone*> newList;
    for (int row = 0; row < list.size(); row++) {
        QString UID = "";
        QString passWord = "";
        QString haiFa = "";
        if (list[row].contains("|") && !list[row].contains("c_user")) {
            QStringList listInforClone = list[row].split("|");
            for (int i=0;i<listInforClone.size();i++ ) {
                if( i == 0) {
                    UID = listInforClone[i];
                } else if( i == 1) {
                    passWord = listInforClone[i];
                } else if (i == 2) {
                    haiFa = listInforClone[i];
                } else {

                }
            }
            newList.append(new CommonInforClone(UID, UID, passWord, haiFa));

        }

    }
    file.close();
}



// click "Run -> chạy -> Chạy tất cả tài khoản" hoặc click button play
void MainWindow::on_actionRunAllAccount_triggered()
{
    if(CONTROLLERMAIN->getUpdating()){
        setStatusInfo("Đang update, không thể chạy app");
        CONTROLLERMAIN->setAppRunning(false);
        return;
    }
    CONTROLLERMAIN->setAppRunning(true);

    TinSoftProxySetting tinsoftProxy = proxyConfig->getTinSoftProxySetting();
    TmProxySetting tmProxy = proxyConfig->getTmProxySetting();
    XProxySetting xProxy = proxyConfig->getXProxySetting();

    //anhdxv check tick == true & khac rong
    if(tinsoftProxy.apiUser != "" && tinsoftProxy.useAPIUser) {
        LOGD << "tinsoft user API";
        CONTROLLERMAIN->setProxyType(PROXY_TYPE::TINSOFT_PROXY);
        CONTROLLERMAIN->setProxyKind(PROXY_KIND::HTTP);
        CONTROLLERMAIN->startCloneByTinsoftProxyUserKeyApi(tinsoftProxy.apiUser);
    } else if(tinsoftProxy.apiProxy != "" && tinsoftProxy.useAPIProxy) {
        LOGD << "tinsoft proxy API";
    } else if(tmProxy.apiKey != "" && tmProxy.httpProxy) {
        LOGD << "tm API http";
        CONTROLLERMAIN->setProxyType(PROXY_TYPE::TMPROXY);
        CONTROLLERMAIN->setProxyKind(PROXY_KIND::HTTP);
        CONTROLLERMAIN->startCloneByTmProxy(tmProxy.apiKey);
    } else if(tmProxy.apiKey != "" && tmProxy.socketProxy) {
        LOGD << "tm API socks";
        CONTROLLERMAIN->setProxyType(PROXY_TYPE::TMPROXY);
        CONTROLLERMAIN->setProxyKind(PROXY_KIND::SOCKS);
        CONTROLLERMAIN->startCloneByTmProxy(tmProxy.apiKey);
    } else if(xProxy.proxy != "" && xProxy.httpXproxy){
        LOGD << "xproxy http";
        CONTROLLERMAIN->setProxyType(PROXY_TYPE::XPROXY);
        CONTROLLERMAIN->setProxyKind(PROXY_KIND::HTTP);
        CONTROLLERMAIN->startCloneByXProxy(xProxy.proxy, xProxy.numberThread);
    } else if(xProxy.proxy != "" && xProxy.socksXproxy){
        LOGD << "xproxy socks";
        CONTROLLERMAIN->setProxyType(PROXY_TYPE::XPROXY);
        CONTROLLERMAIN->setProxyKind(PROXY_KIND::SOCKS);
        CONTROLLERMAIN->startCloneByXProxy(xProxy.proxy, xProxy.numberThread);
    } else {
        LOGD << "start clone";
        CONTROLLERMAIN->setProxyType(PROXY_TYPE::NONE_PROXY);
        CONTROLLERMAIN->setProxyKind(PROXY_KIND::HTTP);
        CONTROLLERMAIN->startCloneWithoutProxy();
    }
}

// click "Run -> dừng -> dừng tất cả tài khoản" hoặc click button dừng
void MainWindow::on_actionStopAllAccount_triggered()
{
    CONTROLLERMAIN->stop();
}

// click "File -> Import account" hoặc click button import
void MainWindow::on_actionImport_account_triggered()
{
    getTextFromFile();
    reloadTable();
    windowViewClone->show();
}

// click "Update account"
void MainWindow::on_actionUpdate_account_triggered()
{

}

// Click "File->Token"
void MainWindow::on_actionToken_triggered()
{
    tokenDialog->setModal(true);
    //tokenDialog->button->setEnabled(1);
    tokenDialog->exec();
}

// Hiển thị token ra UI và set vào DB
void MainWindow::setToken(QString token, bool x)
{

    if(x == true)
    {
        if(token == NULL)
        {
            tokenLable->setText("     Vui lòng nhập lại token!!!!!!! ");
        }
        else
        {
            tokenLable->setText("Token: " +token);
            SQLiteWorker::getInstance()->setToken(token);
            CONTROLLERMAIN->uiUpdateStatusCloneToStatusBar();
        }
    }
    else
    {
        //tokenLable->setText("     Vui lòng nhập lại token!!!!!!! ");
    }
    reloadTable();
    windowViewClone->show();
}

void MainWindow::onAppRunningChanged(bool appRunning)
{
    if(appRunning) {
        action->setIconSize(QSize(pixmapStop.width(), pixmapStop.height()));
        action->setIcon(pixmapStop);
        labelPlayStop->setText("Stop");
        action->update();

        token->setEnabled(0);
        labelToken->setEnabled(0);
    } else {
        action->setIconSize(QSize(pixmapPlay.width(), pixmapPlay.height()));
        action->setIcon(pixmapPlay);
        labelPlayStop->setText("Start");
        action->update();

        if(CONTROLLERMAIN->getServiceRunning()){
            setStatusInfo("App sẽ dừng khi chạy xong lượt chạy này");
        } else {
            setStatusInfo("App đã dừng");
            token->setEnabled(1);
            labelToken->setEnabled(1);
        }
    }
}

void MainWindow::onServiceRunningChanged(bool serviceRunning)
{
    if(serviceRunning) {
        token->setEnabled(0);
        labelToken->setEnabled(0);
    } else {
        setStatusInfo("App đã dừng");
        token->setEnabled(1);
        labelToken->setEnabled(1);
    }
}

ProcessDownload *MainWindow::getProcessDownload()
{
    return processDownload;
}

void MainWindow::showStatusClone(int ordinalNumberOfClone)
{
    StatusClone *statusClone = new StatusClone(ordinalNumberOfClone);
    QMdiSubWindow * windowStatusClone = loadSubWindow(statusClone);
    windowStatusClone->show();
    CommonInforClone* clone = SQLiteWorker::getInstance()->getListClone().at(ordinalNumberOfClone);
    QString id = clone->getUID();
    windowStatusClone->setWindowTitle("UID " + id);
    statusClone->reloadView(clone->getLog());
    QObject::connect(statusClone, &StatusClone::closeStatusSubwindow,viewClone, &ViewClone::removedSelectionStatusSubwindow);
    QObject::connect(clone, &CommonInforClone::logChanged, statusClone, &StatusClone::reloadView);
}

void MainWindow::on_actionViewClone_triggered()
{
    windowViewClone->show();
}


void MainWindow::on_actionView_log_triggered()
{
    viewLog = new ViewLog();
    viewLog->setModal(true);
    viewLog->exec();
}

void MainWindow::setStatusInfo(QString message)
{
    statusInfo->setText(message);
    statusInfo->update();
}

void MainWindow::setCloneInfo(int totalClone, int liveClone, int checkpointClone)
{
    QString totalClone_s = QString::fromStdString(std::to_string(totalClone));
    QString liveClone_s = QString::fromStdString(std::to_string(liveClone));
    QString checkpointClone_s = QString::fromStdString(std::to_string(checkpointClone));

    cloneInfo->setText("Tổng số clone: " + totalClone_s + "   Live clone: " + liveClone_s + "   Checkpoint clone: " + checkpointClone_s);
}

void MainWindow::initialize()
{
    // QueuedConnection to avoid dead lock
    connect(CONTROLLERMAIN, &ControllerMainThread::appRunningChanged, this, &MainWindow::onAppRunningChanged, Qt::QueuedConnection);
    connect(CONTROLLERMAIN, &ControllerMainThread::serviceRunningChanged, this, &MainWindow::onServiceRunningChanged, Qt::QueuedConnection);


    QString token = SQLiteWorker::getInstance()->getToken();
    if(token != NULL)
    {
        tokenLable->setText("Token: " +token);
    }
    else
    {
       on_actionToken_triggered();
    }
}

void MainWindow::doAction()
{
    if(CONTROLLERMAIN->getAppRunning())
    {
        on_actionStopAllAccount_triggered();
    }
    else
    {
        on_actionRunAllAccount_triggered();

    }

}

void MainWindow::doToken()
{
    on_actionToken_triggered();
}

void MainWindow::on_actionCheckPoint_triggered()
{
    windowCheckPoint->show();
    ui->actionCheckPoint->setEnabled(0);
}


void MainWindow::on_action_F_care_Info_triggered()
{
    infoVersion->setModal(true);
    infoVersion->exec();
}

