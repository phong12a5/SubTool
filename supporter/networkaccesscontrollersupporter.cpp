#include "networkaccesscontrollersupporter.h"
#include "log.h"
#include "DefineString.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

NetworkAccessControllerSupporter::NetworkAccessControllerSupporter(QObject *parent): NetworkAccessSupporter(parent)
{
    m_timerRequestTinsoft.setSingleShot(true);
    m_timerRequestTinsoft.setInterval(150000);

    connect(&m_timerRequestTinsoft, &QTimer::timeout, this, &NetworkAccessControllerSupporter::requestGetProxyFromProxyKeyTinSoft);

//    connect(&m_networkAccessManagerGetRequest, &QNetworkAccessManager::finished, this, &NetworkAccessControllerSupporter::onNetworkAccessManagerGetRequestResponse, Qt::QueuedConnection);
}

void NetworkAccessControllerSupporter::getRequestResponseHandle()
{
    NetworkAccessGetData headRequest = m_netWorkAccessGetRequestQueue.head();
    NetworkAccessGetData nextRequest;
    if(m_netWorkAccessGetRequestQueue.size() > 1){
        nextRequest = m_netWorkAccessGetRequestQueue.at(1);
    }
    LOGD << "Head data type = " << (int)headRequest.type << ", url = " << headRequest.request.url();
    bool isGetFromProxyNextRequest = false;

    switch (headRequest.type) {
    case NETWORK_ACCESS_GET_TYPE::GET_REQUEST_DEFAULT:
        emit onGetRequest(m_networkGetReply);
        break;
    case NETWORK_ACCESS_GET_TYPE::CHECK_USER_KEY_TINSOFT:
        emit onCheckUserKeyApiTinSoft(m_networkGetReply);
        break;
    case NETWORK_ACCESS_GET_TYPE::CHECK_PROXY_KEY_TINSOFT:
        emit onCheckProxyKeyApiTinSoft(m_networkGetReply);
        break;
    case NETWORK_ACCESS_GET_TYPE::GET_PROXY_FROM_USER_KEY_TINSOFT:
        handleGetFromUserKeyTinSoft(QString(m_networkGetReply->readAll()));
        break;
    case NETWORK_ACCESS_GET_TYPE::GET_PROXY_FROM_PROXY_KEY_TINSOFT:
        readProxyTinsoftFromReply(m_networkGetReply->readAll());
        if(nextRequest.type != NETWORK_ACCESS_GET_TYPE::NOT_SUPPORT){
            if(nextRequest.type == NETWORK_ACCESS_GET_TYPE::GET_PROXY_FROM_PROXY_KEY_TINSOFT){
                isGetFromProxyNextRequest = true;
                LOGD << "isGetFromProxyNextRequest true";
            }
        }
        if(!isGetFromProxyNextRequest&&m_isCallForStartApp) {
            LOGD << "emit onGetProxyFromUserKeyApiTinSoft";
            emit onGetProxyFromUserKeyApiTinSoft();
            m_isCallForStartApp = false;
        }
        break;
    case NETWORK_ACCESS_GET_TYPE::NOT_SUPPORT:
        break;
    default:
        break;
    }
}

void NetworkAccessControllerSupporter::postRequestResponseHandle()
{
    NetworkAccessPostData headRequest = m_netWorkAccessPostRequestQueue.head();
//    NetworkAccessPostData nextRequest;
//    if(m_netWorkAccessPostRequestQueue.size() > 1){
//        nextRequest = m_netWorkAccessPostRequestQueue.at(1);
//    }
    LOGD << "Head data type = " << (int)headRequest.type << ", url = " << headRequest.request.url();

    switch (headRequest.type) {
    case NETWORK_ACCESS_POST_TYPE::POST_REQUEST_DEFAULT:
        emit onPostRequest(m_networkPostReply);
        break;
    case NETWORK_ACCESS_POST_TYPE::CHECK_USER_KEY_TM:
        emit onCheckUserKeyApiTm(m_networkPostReply);
        break;
    case NETWORK_ACCESS_POST_TYPE::GET_PROXY_TM:
        if(m_networkPostReply->error() == QNetworkReply::NoError){
            readProxyTmFromReply(m_networkPostReply->readAll());
            emit onGetProxyFromProxyKeyApiTm(true, "");
        }
        else{
            QString err = m_networkPostReply->errorString();
            LOGD << err;
            emit onGetProxyFromProxyKeyApiTm(false, err);
        }

    case NETWORK_ACCESS_POST_TYPE::NOT_SUPPORT:
        break;
    default:
        break;
    }
}

void NetworkAccessControllerSupporter::handleGetFromUserKeyTinSoft(QString data)
{
    LOGD << "handleGetFromUserKeyTinSoft json: " << data;
    QJsonDocument dataUser = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject getDataUser = dataUser.object();
    QJsonValue dataJson = getDataUser.value(QString("data"));
    int leng = dataJson.toArray().size();
    if(leng > 0) {
        m_listProxyKeyApiTinsoft.clear();
        for (int i = 0; i < leng; i++) {
            QJsonValue checkKeySuccess = getDataUser.value(QString("data"))[i].operator[]("success");
            QJsonValue key = getDataUser.value(QString("data"))[i].operator[]("key");
            LOGD << "key = " << key;
            if(checkKeySuccess.toBool() == true) {
                m_listProxyKeyApiTinsoft.append(key.toString());
            }
        }
        LOGD << "find number key availabe = " << m_listProxyKeyApiTinsoft.size();
    }
    requestGetProxyFromProxyKeyTinSoft();
}

void NetworkAccessControllerSupporter::requestGetProxyFromProxyKeyTinSoft()
{
    LOGD;
    m_timerRequestTinsoft.start();
    if(m_listProxyKeyApiTinsoft.isEmpty()){
        if(m_isCallForStartApp) {
            LOGD << "emit onGetProxyFromUserKeyApiTinSoft";
            emit onGetProxyFromUserKeyApiTinSoft();
            m_isCallForStartApp = false;
        }
        return;
    }
    LOGD << "listProxyKey is not empty => clear old proxy and get new proxy";
    mutex.lock();
    m_listProxyTinsoft.clear();
    mutex.unlock();
    foreach(QString key, m_listProxyKeyApiTinsoft)
    {
        networkAccessGetRequest(NetworkAccessGetData(NETWORK_ACCESS_GET_TYPE::GET_PROXY_FROM_PROXY_KEY_TINSOFT,
                                                     QNetworkRequest(QUrl(GET_PROXY_API + key + "&location=0"))));
    }
}

void NetworkAccessControllerSupporter::requestGetProxyFromKeyApiTm()
{
    LOGD;
    const QUrl url(QStringLiteral("https://tmproxy.com/api/proxy/get-current-proxy"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject dataJsonPath;
    dataJsonPath.insert("api_key", m_keyApiTm);
    QJsonDocument dataJsonDoc(dataJsonPath);
    std::string dataJsonPathToString(dataJsonDoc.toJson(QJsonDocument::Compact));

    QByteArray data(dataJsonPathToString.c_str());

    mutex.lock();
    m_listProxyTm.clear();
    mutex.unlock();
    networkAccessPostRequest(NetworkAccessPostData(NETWORK_ACCESS_POST_TYPE::GET_PROXY_TM, request, data));

}

QString NetworkAccessControllerSupporter::readProxyTinsoftFromReply(QString reply)
{
    LOGD << reply;
    QString proxy;
    QJsonDocument dataProxy = QJsonDocument::fromJson(reply.toUtf8());
    QJsonObject getDataProxy = dataProxy.object();
    QJsonValue dataJson = getDataProxy.value(QString("success"));
    QJsonValue getProxy = getDataProxy.value(QString("proxy"));
    LOGD << "dataJson: " << dataJson.toBool() << "| getProxy: " << getProxy;
    if(dataJson.toBool() == true) {
        proxy = getProxy.toString();
    }
    LOGD << "proxy: " << proxy;

    if(!proxy.isEmpty()) {
        LOGD << "append proxy: " << proxy;
        m_listProxyTinsoft.append(proxy);
    }
    return proxy;
}

void NetworkAccessControllerSupporter::readProxyTmFromReply(QString reply)
{
    LOGD << reply;
    QJsonDocument dataProxyObj = QJsonDocument::fromJson(reply.toUtf8());
    QJsonObject dataProxy = dataProxyObj.object();
    QString proxyHttps = dataProxy.value(QString("https")).toString();
    QString proxySockS = dataProxy.value(QString("socks5")).toString();
    if(!(proxyHttps.isEmpty() && proxySockS.isEmpty())){
        m_listProxyTm.append(TmProxy(proxyHttps, proxySockS));
    }
}

void NetworkAccessControllerSupporter::checkUserKeyApiTinSoft(QString key)
{
    LOGD;
    networkAccessGetRequest(NetworkAccessGetData(NETWORK_ACCESS_GET_TYPE::CHECK_USER_KEY_TINSOFT, QNetworkRequest(QUrl(CHECK_KEY_USER_API + key))));
}

void NetworkAccessControllerSupporter::checkProxyKeyApiTinsoft(QString key)
{
    LOGD;
    networkAccessGetRequest(NetworkAccessGetData(NETWORK_ACCESS_GET_TYPE::CHECK_PROXY_KEY_TINSOFT, QNetworkRequest(QUrl(GET_PROXY_API + key + "&location=0"))));
}

void NetworkAccessControllerSupporter::getProxyFromUserKeyApiTinsoft(QString key)
{
    LOGD;
    m_isCallForStartApp = true;
    networkAccessGetRequest(NetworkAccessGetData(NETWORK_ACCESS_GET_TYPE::GET_PROXY_FROM_USER_KEY_TINSOFT, QNetworkRequest(QUrl(CHECK_KEY_USER_API + key))));
}

QStringList NetworkAccessControllerSupporter::getListProxyTinsoft()
{
    QStringList listProxy;
    mutex.lock();
    listProxy = m_listProxyTinsoft;
    mutex.unlock();
    return listProxy;
}

QString NetworkAccessControllerSupporter::getProxyTinsoft()
{
    QString proxy;
    mutex.lock();
    int size = m_listProxyTinsoft.size();
    if(!m_listProxyTinsoft.isEmpty()) {
        LOGD << "list not empty";
        proxy = m_listProxyTinsoft.at(0);
        if(size > 1){
            m_listProxyTinsoft.move(0, (size-1));
        }
    }
    mutex.unlock();
    return proxy;
}

void NetworkAccessControllerSupporter::checkApiKeyTm(QString key)
{
    LOGD << key;
    const QUrl url(QStringLiteral(POST_REQUEST_TM_PROXY));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject dataJsonPath;
    dataJsonPath.insert("api_key", key);
    QJsonDocument dataJsonDoc(dataJsonPath);
    std::string dataJsonPathToString(dataJsonDoc.toJson(QJsonDocument::Compact));

    QByteArray data(dataJsonPathToString.c_str());

    networkAccessPostRequest(NetworkAccessPostData(NETWORK_ACCESS_POST_TYPE::CHECK_USER_KEY_TM, request, data));
}

void NetworkAccessControllerSupporter::getProxyFromProxyKeyApiTm(QString key)
{
    LOGD << "old: " << m_keyApiTm << ", new: " << key;
    if(key != m_keyApiTm) {
        m_keyApiTm = key;
    }
    requestGetProxyFromKeyApiTm();
}

QList<TmProxy> NetworkAccessControllerSupporter::getListProxyTm()
{
    QList<TmProxy> listProxy;
    mutex.lock();
    listProxy = m_listProxyTm;
    mutex.unlock();
    return listProxy;
}

QString NetworkAccessControllerSupporter::getProxyTm(PROXY_KIND kind)
{
    QString proxy;
    mutex.lock();
    int size = m_listProxyTm.size();
    if(!m_listProxyTm.isEmpty()) {
        switch (kind) {
        case PROXY_KIND::HTTP:
            proxy = m_listProxyTm.at(0).httpProxy;
            break;
        case PROXY_KIND::SOCKS:
            proxy = m_listProxyTm.at(0).sockSProxy;
            break;
        default:
            break;
        }
        if(size > 1){
            m_listProxyTm.move(0, (size-1));
        }
    }
    mutex.unlock();
    return proxy;
}
