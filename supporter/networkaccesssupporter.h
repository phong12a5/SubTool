#ifndef NETWORKACCESSSUPPORTER_H
#define NETWORKACCESSSUPPORTER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQueue>
#include <QTimer>
#include <QList>
#include <QString>
#include <QStringList>
#include <QMutex>
#include "AppEnum.h"


struct NetworkAccessGetData {
    NetworkAccessGetData(NETWORK_ACCESS_GET_TYPE inputType = NETWORK_ACCESS_GET_TYPE::NOT_SUPPORT, QNetworkRequest inputRequest = QNetworkRequest()): type(inputType), request(inputRequest){}
    NETWORK_ACCESS_GET_TYPE type;
    QNetworkRequest request;
};
Q_DECLARE_METATYPE(NetworkAccessGetData)

struct NetworkAccessPostData {
    NetworkAccessPostData(NETWORK_ACCESS_POST_TYPE inputType = NETWORK_ACCESS_POST_TYPE::NOT_SUPPORT, QNetworkRequest inputRequest = QNetworkRequest(), QByteArray inputData = QByteArray()):
        type(inputType), request(inputRequest), data(inputData){}
    NETWORK_ACCESS_POST_TYPE type;
    QNetworkRequest request;
    QByteArray data;
};
Q_DECLARE_METATYPE(NetworkAccessPostData)

class NetworkAccessSupporter: public QObject
{
    Q_OBJECT

public:
    NetworkAccessSupporter(QObject* parent = nullptr);

    // default request
    void getRequest(QNetworkRequest request);
    void postRequest(QNetworkRequest request, QByteArray data);

protected:
    void networkAccessGetRequest(NetworkAccessGetData inputData);
    void networkAccessPostRequest(NetworkAccessPostData inputData);

    virtual void getRequestResponseHandle();
    virtual void postRequestResponseHandle();

private slots:
    void makeGetRequest(NetworkAccessGetData inputData);
    void makePostRequest(NetworkAccessPostData inputData);

    void onNetworkAccessManagerGetRequestResponse();
    void onNetworkAccessManagerPostRequestResponse();

signals:
    void sigMakeGetRequest(NetworkAccessGetData inputData);
    void sigMakePostRequest(NetworkAccessPostData inputData);

    // default request
    void onGetRequest(QNetworkReply *reply);
    void onPostRequest(QNetworkReply *reply);


protected:
    QNetworkAccessManager m_networkAccessManagerGetRequest;
    QNetworkReply* m_networkGetReply;
    QQueue<NetworkAccessGetData> m_netWorkAccessGetRequestQueue;

    QNetworkAccessManager m_networkAccessManagerPostRequest;
    QNetworkReply* m_networkPostReply;
    QQueue<NetworkAccessPostData> m_netWorkAccessPostRequestQueue;

    QMutex mutex;
};

#endif // NETWORKACCESSSUPPORTER_H
