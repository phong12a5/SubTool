#include "networkaccesssupporter.h"
#include "log.h"

NetworkAccessSupporter::NetworkAccessSupporter(QObject *parent): QObject(parent)
{
    qRegisterMetaType<NetworkAccessGetData>();
    qRegisterMetaType<NetworkAccessPostData>();
    connect(this, &NetworkAccessSupporter::sigMakeGetRequest, this, &NetworkAccessSupporter::makeGetRequest, Qt::QueuedConnection);
    connect(this, &NetworkAccessSupporter::sigMakePostRequest, this, &NetworkAccessSupporter::makePostRequest, Qt::QueuedConnection);
}

void NetworkAccessSupporter::getRequest(QNetworkRequest request)
{
    networkAccessGetRequest(NetworkAccessGetData(NETWORK_ACCESS_GET_TYPE::GET_REQUEST_DEFAULT, request));
}

void NetworkAccessSupporter::postRequest(QNetworkRequest request, QByteArray data)
{
    networkAccessPostRequest(NetworkAccessPostData(NETWORK_ACCESS_POST_TYPE::POST_REQUEST_DEFAULT, request, data));
}

void NetworkAccessSupporter::networkAccessGetRequest(NetworkAccessGetData inputData)
{
    LOGD << "Add to network get request queue, type = " << (int)inputData.type << ", url = " << inputData.request.url();
    mutex.lock();
    if(inputData.type != NETWORK_ACCESS_GET_TYPE::NOT_SUPPORT) {
        m_netWorkAccessGetRequestQueue.enqueue(inputData);
    }
    if(m_netWorkAccessGetRequestQueue.size() == 1) {
        LOGD << "First get request in queue => Make request";
        emit sigMakeGetRequest(inputData);
    }
    mutex.unlock();
}

void NetworkAccessSupporter::networkAccessPostRequest(NetworkAccessPostData inputData)
{
    LOGD << "Add to network post request queue, type = " << (int)inputData.type << ", request url = " << inputData.request.url() << ", data = " << inputData.data;
    mutex.lock();
    if(inputData.type != NETWORK_ACCESS_POST_TYPE::NOT_SUPPORT) {
        m_netWorkAccessPostRequestQueue.enqueue(inputData);
    }
    if(m_netWorkAccessPostRequestQueue.size() == 1) {
        LOGD << "First post request in queue => Make request";
        emit sigMakePostRequest(inputData);
    }
    mutex.unlock();
}

void NetworkAccessSupporter::getRequestResponseHandle()
{
    NetworkAccessGetData headRequest = m_netWorkAccessGetRequestQueue.head();
    LOGD << "Head data type = " << (int)headRequest.type << ", url = " << headRequest.request.url();

    switch (headRequest.type) {
    case NETWORK_ACCESS_GET_TYPE::GET_REQUEST_DEFAULT:
        emit onGetRequest(m_networkGetReply);
        break;
    case NETWORK_ACCESS_GET_TYPE::NOT_SUPPORT:
        break;
    default:
        break;
    }
}

void NetworkAccessSupporter::postRequestResponseHandle()
{
    NetworkAccessPostData headRequest = m_netWorkAccessPostRequestQueue.head();
    LOGD << "Head data type = " << (int)headRequest.type << ", url = " << headRequest.request.url();

    switch (headRequest.type) {
    case NETWORK_ACCESS_POST_TYPE::POST_REQUEST_DEFAULT:
        emit onPostRequest(m_networkPostReply);
        break;
    case NETWORK_ACCESS_POST_TYPE::NOT_SUPPORT:
        break;
    default:
        break;
    }
}

void NetworkAccessSupporter::makeGetRequest(NetworkAccessGetData inputData)
{
    LOGD;
    m_networkGetReply = m_networkAccessManagerGetRequest.get(inputData.request);
    connect(m_networkGetReply, &QNetworkReply::finished, this, &NetworkAccessSupporter::onNetworkAccessManagerGetRequestResponse, Qt::QueuedConnection);
}

void NetworkAccessSupporter::makePostRequest(NetworkAccessPostData inputData)
{
    LOGD;
    m_networkPostReply = m_networkAccessManagerPostRequest.post(inputData.request, inputData.data);
    connect(m_networkPostReply, &QNetworkReply::finished, this, &NetworkAccessSupporter::onNetworkAccessManagerPostRequestResponse, Qt::QueuedConnection);
}

void NetworkAccessSupporter::onNetworkAccessManagerGetRequestResponse()
{
    if(m_netWorkAccessGetRequestQueue.isEmpty()) {
        LOGD << "Get Queue is empty but receive response, this case shouldnot appear, please check code again!";
        return;
    }
    LOGD << "Get Queue size before: " << m_netWorkAccessGetRequestQueue.size();

    getRequestResponseHandle();

    m_networkGetReply->deleteLater();
    m_netWorkAccessGetRequestQueue.dequeue();
    LOGD << "Get Queue size after: " << m_netWorkAccessGetRequestQueue.size();
    if(!m_netWorkAccessGetRequestQueue.isEmpty()) {
        NetworkAccessGetData nextData = m_netWorkAccessGetRequestQueue.head();
        LOGD << "Request next get request, type = " << (int)nextData.type << ", url = " << nextData.request.url();
        emit sigMakeGetRequest(nextData);
    }
}

void NetworkAccessSupporter::onNetworkAccessManagerPostRequestResponse()
{
    if(m_netWorkAccessPostRequestQueue.isEmpty()) {
        LOGD << "Post Queue is empty but receive response, this case shouldnot appear, please check code again!";
        return;
    }
    LOGD << "Post Queue size before: " << m_netWorkAccessPostRequestQueue.size();

    postRequestResponseHandle();

    m_networkPostReply->deleteLater();
    m_netWorkAccessPostRequestQueue.dequeue();
    LOGD << "Post Queue size after: " << m_netWorkAccessPostRequestQueue.size();
    if(!m_netWorkAccessPostRequestQueue.isEmpty()) {
        NetworkAccessPostData nextData = m_netWorkAccessPostRequestQueue.head();
        LOGD << "Request next post request, type = " << (int)nextData.type << ", request url = " << nextData.request.url() << ", data = " << nextData.data;
        emit sigMakePostRequest(nextData);
    }
}
