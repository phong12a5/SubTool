#ifndef CHROMESERVICE_H
#define CHROMESERVICE_H

#include <QObject>
#include "baseservice.h"
//#include "worker/chromeworker.h"

class ServiceData;
class AFAction;

class ChromeService : public BaseService
{
    Q_OBJECT
public:
    explicit ChromeService(int profileId, QObject *parent = nullptr);
    ~ChromeService();

    void connectSignalSlots() override;
    ServiceData* model();

private:
    void initChromeDriver();
    void getProxy();
    void getClone();
    void getActions();

    void login();
    void feedLike(bool acceptLike);
    void followByPage(QString pageId, AFAction* action);
    bool getPagesOfUid();


    bool checkProxy(QString ip, int port);
    bool getInviteLink(QString& data, QString uid);
public slots:
    void onStarted() override;
    void onMainProcess() override;

private:
    QThread m_;
};

#endif // CHROMESERVICE_H
