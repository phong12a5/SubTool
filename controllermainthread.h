#ifndef CONTROLLERMAINTHREAD_H
#define CONTROLLERMAINTHREAD_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QNetworkReply>
#include <QStringList>

#include "definition.h"
#include "AppEnum.h"
#include "AppDefine.h"
#include "networkaccesscontrollersupporter.h"
#include "downloadmanager.h"

#define CONTROLLERMAIN ControllerMainThread::getInstance()

class MainWindow;

class ControllerMainThread : public QObject
{
    Q_OBJECT

private:
    static ControllerMainThread* m_InstancePtr;

    ControllerMainThread();
    ~ControllerMainThread();


public:
    static ControllerMainThread* getInstance();
    static void deleteInstance();

    void initialize();
    void startApplication();

public:
    // UI function
    void uiUpdateStatusCloneToStatusBar();
    void uiSetStatusInfo(QString message);
    void uiUpdateProcessDialogValue(int value);
    void uiUpdateProcessDialogTitle(QString title);
    int uiGetNumberOfThread();


    // backend function
    void startCloneByTinsoftProxyUserKeyApi(QString keyUser);
    void startCloneByTmProxy(QString key);
    void startCloneByXProxy(QString xproxy, int numThreadXProxy);
    void startCloneWithoutProxy();

    void checkUserKeyApiTinsoftAvailable(QString keyUser);
    void checkProxyKeyApiTinsoftAvailable(QString key);
    void checkApiTm(QString key);

    void setAppRunning(const bool appRunning);
    bool getAppRunning();

    void setServiceRunning(const bool serviceRunning);
    bool getServiceRunning();

    void setUpdating(const bool updating);
    bool getUpdating();

    void setProxyType(const PROXY_TYPE proxyType);
    PROXY_TYPE getProxyType();

    void setProxyKind(const PROXY_KIND proxyKind);
    PROXY_KIND getProxyKind();

    QString getProxy();

    void stop();

    QString getDeviceName() const;


private:
    void startUpdateVersion();

    void setListProxy(QStringList listProxy);
    QStringList getListProxy();
    QString getProxyFromListProxy();

    void setNumThreadPerProxy(int numThread);
    int getNumThreadPerProxy();

signals:
    // function as signal to run in main thread
    void uiShowHideProcessDialog(bool show);
    void reloadTable();
    void sigOnchangeReloadTable(QString cloneId);


    // other
    void appRunningChanged(bool appRunning);
    void serviceRunningChanged(bool serviceRunning);
    void updatingChanged(bool updating);

    void setTimerStatus(QTimer* timer, bool status);


public slots:


private slots:
    void onStartAppTimerTimeout();
    void startCheckAppVersion();
    void checkVerAppToUpdate();
    void updateAppRelease();
    bool checkUserKeyApiTinsoftFinished(QNetworkReply *reply);
    bool checkProxyKeyApiTinsoftFinished(QNetworkReply *reply);
    void getProxyFromUserKeyApiTinSoftFinished();
    void checkUserKeyApiTmFinished(QNetworkReply *reply);
    void getProxyFromProxyKeyApiTmFinished(bool success, QString err);

    void onSetTimerStatus(QTimer* timer, bool status);

    // handle function signals
    void uiShowHideProcessDialogHandle(bool show);
    void reloadTableHandle();
    void onChangeReloadTableHandle(QString cloneId);

private:
    bool m_initialize = false;
    QMutex mutex;

    QString m_deviceName;
    MainWindow* m_mainUI = nullptr;

    QTimer m_startAppTimer;
    QTimer m_updateTimer;

    // supporter
    NetworkAccessControllerSupporter m_networkAccess;
    DownLoadManager m_downloadManager;

    // other
    bool m_appRunning = false;
    bool m_serviceRunning = false;
    bool m_updating = false;

    //proxy
    PROXY_TYPE m_proxyType = PROXY_TYPE::NONE_PROXY;
    PROXY_KIND m_proxyKind = PROXY_KIND::HTTP;

    int m_proxyCount = 0;
    int m_numThreadPerProxy = 0;
    QString m_proxy = "";
    QStringList m_listProxy;

};

#endif // CONTROLLERMAINTHREAD_H
