#include "mainwindow_bak.h"
#include "ui_mainwindow_bak.h"
#include "appmain.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QAction>
#include <QCloseEvent>
#include <QSettings>
#include <QMdiSubWindow>
#include <QScreen>
#include <QComboBox>
#include <QWidgetAction>
#include <QToolButton>
#include "service/servicemanager.h"
#include "service/chromeservice.h"
#include "log.h"
#include "model/servicedata.h"
#include "QMutex"
#include "DefineString.h"

#include <stdio.h>
#include <curl/curl.h>


MainWindow_bak::MainWindow_bak(QWidget *parent)
    : QMainWindow(parent)
    , mdiArea(new QMdiArea)
{

    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setWindowIcon(QIcon(":/images/good_quality_25px.png"));

    createActions();
    createMenus();
    createToolButtons();
    createToolBars();
    createStatusBar();
    updateMenus();

//    readSettings();
// //   newFile();
//    setWindowTitle(tr("F-Care"));
//    setUnifiedTitleAndToolBarOnMac(true);
////    settingcnDialog = new SettingcnDialog();
////    settingDialog = new SettingTTDialog();
////    aboutAutolike = new AboutAutolike();
////    loadSubWindow(aboutAutolike);
//    m_thread = new Thread;
//    m_cloneContinue = 0;
//    m_managerKeyUser = new QNetworkAccessManager(this);
//    m_managerKeyProxy = new QNetworkAccessManager(this);
//    connect(m_managerKeyUser, &QNetworkAccessManager::finished, this, &MainWindow_bak::ReplyUserKeyApiFinished);
//    connect(m_managerKeyProxy, &QNetworkAccessManager::finished, this, &MainWindow_bak::ReplyProxyKeyApiFinished);

}

MainWindow_bak::~MainWindow_bak()
{
    LOGD << "MainWindow_bak::~MainWindow_bak";
    stop();
}

void MainWindow_bak::ReplyUserKeyApiFinished(QNetworkReply *reply)
{
    QString answer = reply->readAll();
    LOGD << "ReplyUserKeyApiFinished json: " << answer;
    QJsonDocument dataUser = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject getDataUser = dataUser.object();
    QJsonValue dataJson = getDataUser.value(QString("data"));
    int leng = dataJson.toArray().size();
    for (int i = 0; i < leng; i++) {
        QJsonValue checkKeySusses = getDataUser.value(QString("data"))[i].operator[]("success");
        QJsonValue key = getDataUser.value(QString("data"))[i].operator[]("key");
        if(checkKeySusses.toBool() == true) {
            getAPIProxy(key.toString());
            break;
        }
    }
}


void MainWindow_bak::startCloneByUserKeyApi(QString keyUser)
{
    LOGD << "startCloneByUserKeyApi";
    QString userAPI = CHECK_KEY_USER_API;
    userAPI.append(keyUser);
    m_managerKeyUser->get(QNetworkRequest(QUrl(userAPI)));
}

void MainWindow_bak::ReplyProxyKeyApiFinished(QNetworkReply *reply)
{
    QString answer = reply->readAll();
    LOGD << "ReplyProxyKeyApiFinished: " << answer;
    QJsonDocument dataProxy = QJsonDocument::fromJson(answer.toUtf8());
    QJsonObject getDataProxy = dataProxy.object();
    QJsonValue dataJson = getDataProxy.value(QString("success"));
    QJsonValue getProxy = getDataProxy.value(QString("proxy"));
    LOGD << "dataJson: " << dataJson << "| getProxy: " << getProxy;
    if(dataJson.toBool() == true) {
        LOGD << "getProxy.toString: " << getProxy.toString();
        setProxy(getProxy.toString());
        startClone();
    }
}

void MainWindow_bak::getAPIProxy(QString key)
{
    m_managerKeyProxy->get(QNetworkRequest(QUrl(GET_PROXY_API + key + "&location=0")));
}

void MainWindow_bak::setProxy(QString proxy)
{
    m_proxy = proxy;
}

QString MainWindow_bak::getProxy()
{
    LOGD << "getProxy: " << m_proxy;
    return m_proxy;
}

void MainWindow_bak::postRequest()
{
//    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
//    const QUrl url(QStringLiteral("https://tmproxy.com/api/proxy/stats"));
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//    QByteArray data("{\"api_key\": \"b0ed9e4000f4f4e64283594dc9e1310d\"}");
//    QNetworkReply *reply = mgr->post(request, data);

//    QObject::connect(reply, &QNetworkReply::finished, [=](){
//        if(reply->error() == QNetworkReply::NoError){
//            QString contents = QString::fromUtf8(reply->readAll());
//            LOGD << "response: " << contents;
//        }
//        else{
//            QString err = reply->errorString();
//            LOGD << err;
//        }
//        reply->deleteLater();
//    });
}

void MainWindow_bak::onFinish(QNetworkReply *rep)
{
    LOGD << rep->readAll();
}

void MainWindow_bak::closeEvent(QCloseEvent *event)
{
    LOGD;
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}

void MainWindow_bak::newFile()
{
    LOGD;
//    aboutAutolike->loadSubWindow();
}


void MainWindow_bak::createService()
{
    QMutex mutex;
    mutex.lock();
    int getServiceContinue = ServiceManager::instance()->getNumberContinueClone();
    int serviceID = ServiceManager::instance()->createService<ChromeService>();
    ChromeService* service = ServiceManager::instance()->getService<ChromeService>(serviceID);
//    service->model()->setInfoClone(SQLiteWorker::getInstance()->getListClone().at(getServiceContinue));
    service->model()->setNumberChrom(getServiceContinue);
    service->model()->setServiceID(serviceID);
    service->startService(SQLiteWorker::getInstance()->getListClone().at(getServiceContinue)->getUID());
    getServiceContinue++;
    ServiceManager::instance()->setNumberContinueClone(getServiceContinue);
    mutex.unlock();
}

void MainWindow_bak::startApp()
{
//    QString tinsoftAPI = settingDialog->getTinSoftProxySetting().apiUser;
//    QString tmAPI = settingDialog->getTmProxySetting().apiKey;
//    LOGD << "userAPI: " << tinsoftAPI;
//    if(tinsoftAPI != "") {
//        startCloneByUserKeyApi(tinsoftAPI);
//    } else if(tmAPI != "") {
//        postRequest();
//    } else {
//        startClone();
//    }

}

void MainWindow_bak::startClone()
{
//    m_thread = settingcnDialog->getThread();
//    LOGD  << "m_thread = " << m_thread->numberThread;
//    int numberThread = m_thread->numberThread;
//    QList<CommonInforClone *> listClone = aboutAutolike->getListClone();
//    if(FDriver::unlockFDriver("0399843737") || 1) {
//        if(ServiceManager::instance()->countService() == 0) {
//            for (int i = 0; i < numberThread; ++i) {
//                int serviceID = ServiceManager::instance()->createService<ChromeService>();
//                LOGD << "MainWindow_bak service ID: " << serviceID;
//                ChromeService* service = ServiceManager::instance()->getService<ChromeService>(serviceID);
//                service->setInfoClone(listClone.at(i));
//                emit setProxyServer(getProxy());
//                service->model()->setInfoClone(listClone.at(i));
//                service->model()->setNumberthread(i);
//                service->model()->setServiceID(serviceID);
//                service->startService(listClone.at(i)->getUID());
//                ServiceManager::instance()->setNumberContinueClone(i + 1);
//            }

//        }
//    } else {
//        LOGD << "unlock Fdriver failed";
//    }
}

void MainWindow_bak::importClone()
{
//    LOGD;
//    QString filter = "All File (*.*) ;; Text File (*.txt) ;; XML File (*.xml)";
//    QString filename = QFileDialog::getOpenFileName(this, "open a file", "C://", filter);
//    QFile file(filename);

//    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        //        QMessageBox::warning(this, "title", "file not open");
//    }
//    QTextStream in ( & file);
//    QString text = in .readAll();

//    QStringList list = text.split("\n");

//    for (int row = 0; row < list.size(); row++) {
//        if (list[row].contains("|")) {
//            QStringList list1;
//        list1 = list[0].split("|");
//                newFile();
//        }

//    }
}

void MainWindow_bak::open()
{
//    LOGD;
//    deviceDialog = new DeviceDialog(this);
//    loadSubWindow(deviceDialog);
//    if (aboutAutolike != nullptr) {
//        connect(aboutAutolike, &AboutAutolike::selectedFile, deviceDialog,
//                &DeviceDialog::addFile);
//    }


}

void MainWindow_bak::openLogClone()
{
//    LOGD;
//    logTableDialog = new LogTableDialog(this);
//    loadSubWindow(logTableDialog);
//    if (aboutAutolike != nullptr) {
//        connect(aboutAutolike, &AboutAutolike::selectedFile, deviceDialog,
//                &DeviceDialog::addFile);
//    }
}

bool MainWindow_bak::openFile(const QString &fileName)
{

}


bool MainWindow_bak::loadFile(const QString &fileName)
{

}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }

static QStringList readRecentFiles(QSettings &settings)
{
    LOGD;
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}

static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
    LOGD;
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

bool MainWindow_bak::hasRecentFiles()
{
    LOGD;
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}

void MainWindow_bak::prependToRecentFiles(const QString &fileName)
{
    LOGD;
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentFilesVisible(!recentFiles.isEmpty());
}

void MainWindow_bak::setRecentFilesVisible(bool visible)
{
    LOGD;
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}

void MainWindow_bak::updateRecentFileActions()
{
    LOGD;
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for ( ; i < count; ++i) {
        const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for ( ; i < MaxRecentFiles; ++i)
        recentFileActs[i]->setVisible(false);
}

void MainWindow_bak::loadSubWindow(QWidget *widget)
{
    LOGD;
    auto window = mdiArea->addSubWindow(widget);
    window->setWindowTitle(widget->windowTitle());
    window->setWindowIcon(widget->windowIcon());
    window->resize(widget->size());
    window->show();

}

void MainWindow_bak::openRecentFile()
{
//    LOGD;
//    if (const QAction *action = qobject_cast<const QAction *>(sender()))
//        openFile(action->data().toString());
}

void MainWindow_bak::save()
{

}

void MainWindow_bak::stop()
{
    emit stopApp();
    if(ServiceManager::instance()->countService() != 0)
    {
        foreach(int serviceId, ServiceManager::instance()->getServiceIds()) {
            ServiceManager::instance()->deleteService(serviceId);
//            ServiceManager::instance()->deleteService(serviceId);
        }
    }
    qDebug()<< "xoa ServiceManager" << ServiceManager::instance()->countService();
}

void MainWindow_bak::saveAs()
{

}

#ifndef QT_NO_CLIPBOARD
void MainWindow_bak::cut()
{

}

void MainWindow_bak::copy()
{

}

void MainWindow_bak::paste(bool state)
{
    LOGD << "state: " << state;
    if(state){
        AppMain::instance()->startTest();
    }
    else {
        AppMain::instance()->stopTest();
    }
}
#endif

void MainWindow_bak::about()
{
    LOGD;
//    QMessageBox::about(this, tr("About MDI"),
//                       tr("The <b>MDI</b> example demonstrates how to write multiple "
//               "document interface applications using Qt."));
}

void MainWindow_bak::setting()
{
    //    settingDialog = new SettingTTDialog(this);
//    //         settingDialog->show();
//    LOGD;
//    settingDialog = new SettingTTDialog(this);
//    loadSubWindow(settingDialog);
}

void MainWindow_bak::settingcn()
{
    //    settingcnDialog = new SettingcnDialog(this);
    //         settingcnDialog->show();
//    LOGD;
//    settingcnDialog = new SettingcnDialog(this);
//    loadSubWindow(settingcnDialog);
}

void MainWindow_bak::settingvc()
{
    LOGD;
//    settingviewCloneDialog = new SettingviewCloneDialog(this);
//    loadSubWindow(settingviewCloneDialog);
}

void MainWindow_bak::settinght()
{
    //    settingcnDialog = new SettingcnDialog(this);
    //         settingcnDialog->show();
    LOGD;
//    settingcnDialog = new SettingcnDialog(this);
//    loadSubWindow(settingcnDialog);
}

void MainWindow_bak::updateMenus()
{

}

void MainWindow_bak::updateWindowMenu()
{
    LOGD;
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(windowMenuSeparatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    windowMenuSeparatorAct->setVisible(!windows.isEmpty());

    //    for (int i = 0; i < windows.size(); ++i) {
    //        QMdiSubWindow *mdiSubWindow = windows.at(i);
    //        MdiChild *child = qobject_cast<MdiChild *>(mdiSubWindow->widget());

    //        QString text;
    //        if (i < 9) {
    //            text = tr("&%1 %2").arg(i + 1)
    //                               .arg(child->userFriendlyCurrentFile());
    //        } else {
    //            text = tr("%1 %2").arg(i + 1)
    //                              .arg(child->userFriendlyCurrentFile());
    //        }
    //        QAction *action = windowMenu->addAction(text, mdiSubWindow, [this, mdiSubWindow]() {
    //            mdiArea->setActiveSubWindow(mdiSubWindow);
    //        });
    //        action->setCheckable(true);
    //        action ->setChecked(child == activeMdiChild());
    //    }
}

QTableView *MainWindow_bak::createMdiChild()
{
    QTableView *child = new QTableView;
    mdiArea->addSubWindow(child);
    child->setWindowTitle("QTableview");


#ifndef QT_NO_CLIPBOARD
    //    connect(child, &QTableView::copyAvailable, cutAct, &QAction::setEnabled);
    //    connect(child, &QTableView::copyAvailable, copyAct, &QAction::setEnabled);
#endif

    return child;
}

void MainWindow_bak::createActions()
{
//    //menu dat
//    LOGD;
//    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
//    QToolBar *fileToolBar = addToolBar(tr("File"));

//    const QIcon newIcon = QIcon::fromTheme("folder", QIcon(":/images/icons8_add_25px.png"));
//    newAct = new QAction(newIcon, tr("&ImportClone"), this);
////    newAct->setShortcuts(QKeySequence::ImportClone);
//    newAct->setStatusTip(tr("Create import clone file"));
//    connect(newAct, &QAction::triggered, this, &MainWindow_bak::newFile);
//    fileMenu->addAction(newAct);
//    //    fileToolBar->addAction(newAct);


//    //DROPDOWN MENU START
////    QToolButton *saveButton=new QToolButton(this);

//////    saveButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
////    saveButton->setIcon(QIcon(":/images/icons8_true_false1.ico"));
//////    saveButton->setText("Action");

////    saveButton->setPopupMode(QToolButton::InstantPopup);
////    QMenu *saveMenu=new QMenu(saveButton);
////    const QIcon newIconStart = QIcon::fromTheme("folder", QIcon(":/images/icons8_play_11.ico"));
////    newActStart = new QAction(newIcon,"&Start", this);
////    saveMenu->addAction(newActStart);
////    connect(newActStart, &QAction::triggered, this, &MainWindow_bak::startClone);

////    const QIcon newIconStartSelected = QIcon::fromTheme("folder", QIcon(":/images/icons8_approval1.ico"));
////    saveMenu->addAction(new QAction(newIconStartSelected,"Start se&lected clone", this));
////    saveButton->setMenu(saveMenu);
////    fileToolBar->addWidget(saveButton);

//    alignLeftAction = new QAction("Selected Clone", this);
//    alignLtAction = new QAction("Start", this);
//    alignCenterAction = new QAction("Align center", this);
//    alignRightAction = new QAction("Align right", this);


//    alignLeftAction->setIcon(QIcon("C:/Users/admin/Desktop/toolbutton/untitled1/alignLeft.png"));
//    alignLtAction->setIcon(QIcon(":/images/round-play-button.png"));

////    alignCenterAction->setIcon(QIcon("C:/Users/admin/Desktop/toolbutton/untitled1/alignCenter.png"));
////    alignRightAction->setIcon(QIcon("C:/Users/admin/Desktop/toolbutton/untitled1/alignRight.png"));

//    QObject::connect(alignLeftAction, SIGNAL(triggered()), this, SLOT(alignLeft()));
////    QObject::connect(alignCenterAction, SIGNAL(triggered()), this, SLOT(alignCenter()));
////    QObject::connect(alignRightAction, SIGNAL(triggered()), this, SLOT(alignRight()));
//    QObject::connect(alignLtAction, &QAction::triggered, this, &MainWindow_bak::startApp);



//    const QIcon openIcon = QIcon::fromTheme("modem", QIcon(":/images/icons8_add_30px.png"));
//    QAction *openAct = new QAction(openIcon, tr("&Device"), this);
////    openAct->setShortcuts(QKeySequence::Device);
//    openAct->setStatusTip(tr("Create device file"));
//    connect(openAct, &QAction::triggered, this, &MainWindow_bak::open);
//    fileMenu->addAction(openAct);
//    //    fileToolBar->addAction(openAct);

//    const QIcon openLogIcon = QIcon::fromTheme("folder-remote", QIcon(":/images/Add_Tab_25px.png"));
//    QAction *openLogAct = new QAction(openLogIcon, tr("&LogClone"), this);
////    openLogAct->setShortcuts(QKeySequence::LogClone);
//    openLogAct->setStatusTip(tr("Create Log Clone file"));
//    connect(openLogAct, &QAction::triggered, this, &MainWindow_bak::openLogClone);
//    fileMenu->addAction(openLogAct);
//    //    fileToolBar->addAction(openLogAct);




//    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
//    saveAct = new QAction(saveIcon, tr("&Save"), this);
//    saveAct->setShortcuts(QKeySequence::Save);
//    saveAct->setStatusTip(tr("Save the document to disk"));
//    connect(saveAct, &QAction::triggered, this, &MainWindow_bak::save);
//    //   fileToolBar->addAction(saveAct);

//    const QIcon stopIcon = QIcon::fromTheme("document-save", QIcon(":/images/icons8_stop_1.ico"));
//    stopAct = new QAction(stopIcon, tr("&Stop"), this);
//    stopAct->setShortcuts(QKeySequence::Cancel);
//    stopAct->setStatusTip(tr("Stop"));
//    connect(stopAct, &QAction::triggered, this, &MainWindow_bak::stop);
//    fileToolBar->addAction(stopAct);

//    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
//    saveAsAct = new QAction(saveAsIcon, tr("Save &As..."), this);
//    saveAsAct->setShortcuts(QKeySequence::SaveAs);
//    saveAsAct->setStatusTip(tr("Save the document under a new name"));
//    connect(saveAsAct, &QAction::triggered, this, &MainWindow_bak::saveAs);
//    //   fileMenu->addAction(saveAsAct);

//    fileMenu->addSeparator();

//    QMenu *recentMenu = fileMenu->addMenu(tr("Recent..."));
//    connect(recentMenu, &QMenu::aboutToShow, this, &MainWindow_bak::updateRecentFileActions);
//    recentFileSubMenuAct = recentMenu->menuAction();

//    for (int i = 0; i < MaxRecentFiles; ++i) {
//        recentFileActs[i] = recentMenu->addAction(QString(), this, &MainWindow_bak::openRecentFile);
//        recentFileActs[i]->setVisible(false);
//    }


//    recentFileSeparator = fileMenu->addSeparator();

//    setRecentFilesVisible(MainWindow_bak::hasRecentFiles());

//    //    fileMenu->addAction(tr("Switch layout direction"), this, &MainWindow_bak::switchLayoutDirection);

//    fileMenu->addSeparator();

//    //! [0]
//    const QIcon exitIcon = QIcon::fromTheme("application-exit");
//    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), qApp, &QApplication::closeAllWindows);
//    exitAct->setShortcuts(QKeySequence::Quit);
//    exitAct->setStatusTip(tr("Exit the application"));
//    //    fileMenu->addAction(exitAct);
//    //! [0]

//#ifndef QT_NO_CLIPBOARD
//    QMenu *editMenu = menuBar()->addMenu(tr("&View"));
//    QToolBar *editToolBar = addToolBar(tr("View"));


//    //view clone
//    const QIcon cutIcon = QIcon::fromTheme("View-Clone", QIcon(":/images/circled_user_male_25px.png"));
//    cutAct = new QAction(cutIcon, tr("Clon&e"), this);
////    cutAct->setShortcuts(QKeySequence::Clone);
//    cutAct->setStatusTip(tr("Hiển thị view Clone"));
//    connect(cutAct, &QAction::triggered, this, &MainWindow_bak::newFile);
//    editMenu->addAction(cutAct);
//    editToolBar->addAction(cutAct);
//     fileToolBar->addAction(newAct);

//    const QIcon copyIcon = QIcon::fromTheme("View-Device", QIcon(":/images/copy.png"));
//    copyAct = new QAction(copyIcon, tr("Devic&e"), this);
////    copyAct->setShortcuts(QKeySequence::Device);
//    copyAct->setStatusTip(tr("Hiển thị view Device"));
//    connect(copyAct, &QAction::triggered, this, &MainWindow_bak::open);
//    editMenu->addAction(copyAct);
////    editToolBar->addAction(copyAct);

//    const QIcon pasteIcon = QIcon::fromTheme("View-Log", QIcon(":/images/paste.png"));
//    pasteAct = new QAction(pasteIcon, tr("Lo&g"), this);
////    pasteAct->setShortcuts(QKeySequence::Log);
//    pasteAct->setStatusTip(tr("Hiển thị view Log"));
//    pasteAct->setCheckable(true);
//    connect(pasteAct, &QAction::triggered, this, &MainWindow_bak::openLogClone);
//    editMenu->addAction(pasteAct);
////    editToolBar->addAction(pasteAct);

//    //View Config
//    QMenu *setting = menuBar()->addMenu(tr("Con&fig"));
//    QToolBar *fileToolBarSetting = addToolBar(tr("Config"));

//    const QIcon newIconsetting = QIcon::fromTheme("setting-cn", QIcon(":/images/icons8_job.png"));
//    newAct = new QAction(newIconsetting, tr("&Cấu hình thiết lập Proxy"), this);
////    newAct->setShortcuts(QKeySequence::Setting);
//    newAct->setStatusTip(tr("Cấu hình thiết lập proxy"));
//    connect(newAct, &QAction::triggered, this, &MainWindow_bak::setting);
//    setting->addAction(newAct);
//    //    fileToolBarSetting->addAction(newAct);

//    const QIcon newIconsettingcn = QIcon::fromTheme("setting-tt", QIcon(":/images/icons8_services_25px.png"));
//    newActtt = new QAction(newIconsettingcn, tr("&Cấu hình tương tác"), this);
////    newActtt->setShortcuts(QKeySequence::Setting);
//    newActtt->setStatusTip(tr("Cấu hình tương tác"));
//    connect(newActtt, &QAction::triggered, this, &MainWindow_bak::settingcn);
//    setting->addAction(newActtt);
//    //    fileToolBarSetting->addAction(newActtt);

//    const QIcon newIconsettingviewclone = QIcon::fromTheme("setting-vc", QIcon(":/images/icons8_services_25px.png"));
//    newActvc = new QAction(newIconsettingviewclone, tr("Cấu hình view Clone"), this);
////    newActvc->setShortcuts(QKeySequence::Setting);
//    newActvc->setStatusTip(tr("Cấu hình view Clone"));
//    connect(newActvc, &QAction::triggered, this, &MainWindow_bak::settingvc);
//    setting->addAction(newActvc);
//    //    fileToolBarSetting->addAction(newActvc);

//    const QIcon newIconsettingviewdevice = QIcon::fromTheme("setting-vd", QIcon(":/images/icons8_services_25px.png"));
//    newActvd = new QAction(newIconsettingviewclone, tr("Cấu hình view Device"), this);
////    newActvd->setShortcuts(QKeySequence::Setting);
//    newActvd->setStatusTip(tr("Cấu hình view Device"));
//    connect(newActvd, &QAction::triggered, this, &MainWindow_bak::settingcn);
//    setting->addAction(newActvd);

//#endif

//    windowMenu = menuBar()->addMenu(tr("&Window"));
//    connect(windowMenu, &QMenu::aboutToShow, this, &MainWindow_bak::updateWindowMenu);

//    closeAct = new QAction(tr("Cl&ose"), this);
//    closeAct->setStatusTip(tr("Close the active window"));
//    connect(closeAct, &QAction::triggered,
//            mdiArea, &QMdiArea::closeActiveSubWindow);

//    closeAllAct = new QAction(tr("Close &All"), this);
//    closeAllAct->setStatusTip(tr("Close all the windows"));
//    connect(closeAllAct, &QAction::triggered, mdiArea, &QMdiArea::closeAllSubWindows);

//    tileAct = new QAction(tr("&Tile"), this);
//    tileAct->setStatusTip(tr("Tile the windows"));
//    connect(tileAct, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);

//    cascadeAct = new QAction(tr("&Cascade"), this);
//    cascadeAct->setStatusTip(tr("Cascade the windows"));
//    connect(cascadeAct, &QAction::triggered, mdiArea, &QMdiArea::cascadeSubWindows);

//    nextAct = new QAction(tr("Ne&xt"), this);
//    nextAct->setShortcuts(QKeySequence::NextChild);
//    nextAct->setStatusTip(tr("Move the focus to the next window"));
//    connect(nextAct, &QAction::triggered, mdiArea, &QMdiArea::activateNextSubWindow);

//    previousAct = new QAction(tr("Pre&vious"), this);
//    previousAct->setShortcuts(QKeySequence::PreviousChild);
//    previousAct->setStatusTip(tr("Move the focus to the previous "
//                                 "window"));
//    connect(previousAct, &QAction::triggered, mdiArea, &QMdiArea::activatePreviousSubWindow);

//    windowMenuSeparatorAct = new QAction(this);
//    windowMenuSeparatorAct->setSeparator(true);

//    updateWindowMenu();

//    menuBar()->addSeparator();



//    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

//    QAction *aboutAct = helpMenu->addAction(tr("&Hướng dẫn sử dụng"), this, &MainWindow_bak::about);
//    aboutAct->setStatusTip(tr("Hướng dẫn sử dụng"));

//    QAction *aboutQtAct = helpMenu->addAction(tr("About"), qApp, &QApplication::aboutQt);
//    aboutQtAct->setStatusTip(tr("About"));
}

void MainWindow_bak::createStatusBar()
{
//    statusBar()->showMessage(tr("Ready"));
}

void MainWindow_bak::readSettings()
{
//    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
//    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
//    if (geometry.isEmpty()) {
//        const QRect availableGeometry = screen()->availableGeometry();
//        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
//        move((availableGeometry.width() - width()) / 2,
//             (availableGeometry.height() - height()) / 2);
//    } else {
//        restoreGeometry(geometry);
//    }
}

void MainWindow_bak::writeSettings()
{
//    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
//    settings.setValue("geometry", saveGeometry());
}

QTableView *MainWindow_bak::activeMdiChild1() const
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<QTableView *>(activeSubWindow->widget());
    return nullptr;
}


QMdiSubWindow *MainWindow_bak::findMdiChild(const QString &fileName) const
{
    //    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    //    const QList<QMdiSubWindow *> subWindows = mdiArea->subWindowList();
    //    for (QMdiSubWindow *window : subWindows) {
    //        QTextEdit *mdiChild = qobject_cast<QTextEdit *>(window->widget());
    //        if (mdiChild->currentFile() == canonicalFilePath)
    //            return window;
    //    }
    return nullptr;
}

void MainWindow_bak::switchLayoutDirection()
{
//    if (layoutDirection() == Qt::LeftToRight)
//        QGuiApplication::setLayoutDirection(Qt::RightToLeft);
//    else
//        QGuiApplication::setLayoutDirection(Qt::LeftToRight);
}

void MainWindow_bak::createMenus(){
    alignMenu = new QMenu;
//    alignMenu->addAction(alignRightAction);
//    alignMenu->addAction(alignCenterAction);
    alignMenu->addAction(alignLeftAction);
//    alignMenu->addAction(alignLtAction);
}

void MainWindow_bak::createToolButtons(){
//    alignToolButton = new Customtoolbutton;
//    alignToolButton->setMenu(alignMenu);
//    alignToolButton->setDefaultAction(alignLtAction);
}

void MainWindow_bak::createToolBars(){
//    QToolBar *fileToolBar1 = addToolBar(tr("File1"));
//    fileToolBar1->addWidget(alignToolButton);
}

void MainWindow_bak::alignLeft(){
    qDebug() << "Selected Clone";
}

void MainWindow_bak::alignLt(){
    qDebug() << "MainWindow_bak::alignL()";
}

void MainWindow_bak::alignCenter(){
    qDebug() << "MainWindow_bak::alignCenter()";
}

void MainWindow_bak::alignRight(){
    qDebug() << "MainWindow_bak::alignRight()";
}



