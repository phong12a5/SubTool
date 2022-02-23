#ifndef CONTROLLERWORKERTHREAD_H
#define CONTROLLERWORKERTHREAD_H

#include <QObject>
#include <QNetworkReply>

#include "definition.h"
#include "DefineString.h"
#include "AppEnum.h"
#include "AppDefine.h"
#include "CloneInfo.h"

#define CONTROLLERWORKER ControllerWorkerThread::getInstance()

class ControllerWorkerThread: public QObject
{
    Q_OBJECT

private:
    static ControllerWorkerThread* m_InstancePtr;

    ControllerWorkerThread();
    ~ControllerWorkerThread();

public:
    static ControllerWorkerThread* getInstance();
    static void deleteInstance();

    // worker thread cannot have public function


signals:
    // function as signal to run in worker thread, a woker thread can have only signal functions
    void initialize();
    void startClone();


    // other


private slots:

    // handle function signals
    void initializeHandle();
    void startCloneHandle();

    void onRunCloneFinished(QString lastClone = "");


private:
    void registerDevice();
    bool getCloneToServer();
    bool getCloneToRunFromDB(QString lastClone = "");
    bool checkCloneStoreOnServer(CommonInforClone *clone);
    void updateCloneInfo(QString status, QString idClone, QString UID, QString passWork);

    QString getUserAgent();
    QString getDoAcction(QString idClone);

private:
    bool m_initialize = false;
    QThread m_workerThread;

    QString m_proxy = "";
    QStringList m_listProxy;

    CommonInforClone *m_cloneRun = nullptr;
    int m_countChrom = 0;

    int m_numThreadXProxy = 0;
    int m_xProxyCount = 0;

    QProcess* m_chromeDriverProcess = nullptr;
};

#endif // CONTROLLERWORKERTHREAD_H
