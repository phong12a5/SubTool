#ifndef NETWORKACCESSCONTROLLERSUPPORTER_H
#define NETWORKACCESSCONTROLLERSUPPORTER_H

#include <QNetworkAccessManager>
#include "networkaccesssupporter.h"


struct TmProxy {
    TmProxy(QString inputHttp = QString(), QString inputSockS = QString()): httpProxy(inputHttp), sockSProxy(inputSockS){}
    QString httpProxy;
    QString sockSProxy;
};

class NetworkAccessControllerSupporter: public NetworkAccessSupporter
{
    Q_OBJECT

private:
    void getRequestResponseHandle() override;
    void postRequestResponseHandle() override;

    // Tinsoft handle
    void handleGetFromUserKeyTinSoft(QString data);
    QString readProxyTinsoftFromReply(QString reply);

    // Tm handle
    void readProxyTmFromReply(QString reply);

public:
    NetworkAccessControllerSupporter(QObject *parent = nullptr);

    // tinsoft
    void checkUserKeyApiTinSoft(QString key);
    void checkProxyKeyApiTinsoft(QString key);
    void getProxyFromUserKeyApiTinsoft(QString key);

    QStringList getListProxyTinsoft();
    QString getProxyTinsoft();

    // tm
    void checkApiKeyTm(QString key);
    void getProxyFromProxyKeyApiTm(QString key);

    QList<TmProxy> getListProxyTm();
    QString getProxyTm(PROXY_KIND kind = PROXY_KIND::HTTP);


public slots:

private slots:
    void requestGetProxyFromProxyKeyTinSoft();
    void requestGetProxyFromKeyApiTm();

signals:
    // tinsoft
    void onCheckUserKeyApiTinSoft(QNetworkReply *reply);
    void onCheckProxyKeyApiTinSoft(QNetworkReply *reply);
    void onGetProxyFromUserKeyApiTinSoft();

    // tm
    void onCheckUserKeyApiTm(QNetworkReply *reply);
    void onGetProxyFromProxyKeyApiTm(bool success, QString err = QString());


private:
    // handle from api tinsoft
    QList<QString> m_listProxyKeyApiTinsoft;
    QList<QString> m_listProxyTinsoft;

    QTimer m_timerRequestTinsoft;

    // handle from api tm
    QString m_keyApiTm;
    QList<TmProxy> m_listProxyTm;

    bool m_isCallForStartApp = false;
};

#endif // NETWORKACCESSCONTROLLERSUPPORTER_H
