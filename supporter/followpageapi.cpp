#include "followpageapi.h"
#include <QHttpMultiPart>

FollowPageAPI::FollowPageAPI()
{

}


inline bool s_sleep(int secs, bool* isStop)
{
    QTime dieTime = QTime::currentTime().addSecs(secs);
    while( QTime::currentTime() < dieTime)
    {
        if(isStop == nullptr || *isStop == false)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        else
        {
            qDebug() << "Break looping because thread is stop";
            return false;
        }
    }
    return true;
}

QString FollowPageAPI::getIdPage(QString cookie, QString userAgent, QString fb_dtsg)
{
    QUrlQuery postData;
    const QString url = "https://www.facebook.com/api/graphql";
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("authority","www.facebook.com");
    request.setRawHeader("user-agent" , userAgent.toLocal8Bit());
    request.setRawHeader("x-fb-friendly-name", "PageCometLaunchpointPagesListQuery");
    request.setRawHeader("origin", "https://www.facebook.com");
    request.setRawHeader("sec-fetch-site","none");
    request.setRawHeader("sec-fetch-mode","cors");
    request.setRawHeader("sec-fetch-dest","empty");
    request.setRawHeader("cookie" , cookie.toLocal8Bit());

    postData.addQueryItem("fb_dtsg", fb_dtsg.toLocal8Bit());
    postData.addQueryItem("fb_api_caller_class", "RelayModern");
    postData.addQueryItem("fb_api_req_friendly_name", "PageCometLaunchpointPagesListQuery");
    postData.addQueryItem("variables", "{\"scale\":1.5}");
    postData.addQueryItem("doc_id", "4700711393323643");

    QNetworkReply* response = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    while (!response->isFinished()) {
        s_sleep(1, nullptr);
    }
    QString html = "";
    if(response->isFinished())
        html = response->readAll();
    response->deleteLater();

    LOGD << "message getIdPage: " << html.toStdString().c_str();
    LOGD << "---------end----------";
    return html;
}

bool FollowPageAPI::followPage(QString cookie, QString userAgent, QString fb_dtsg, QString subscribee_id, QString idPage)
{
    QUrlQuery postData;
    const QString url = "https://www.facebook.com/api/graphql";
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("authority","www.facebook.com");
    request.setRawHeader("user-agent" , userAgent.toLocal8Bit());
    request.setRawHeader("origin", "https://www.facebook.com");
    request.setRawHeader("sec-fetch-site","none");
    request.setRawHeader("sec-fetch-mode","cors");
    request.setRawHeader("sec-fetch-dest","empty");
    request.setRawHeader("cookie" , cookie.toLocal8Bit());

    postData.addQueryItem("fb_dtsg", fb_dtsg.toLocal8Bit());
    postData.addQueryItem("fb_api_analytics_tags", "RelayModern");
    postData.addQueryItem("doc_id", "4451435638222552");
    postData.addQueryItem("fb_api_caller_class", "RelayModern");
    QString dataInput = QString("{\"input\":{\"subscribe_location\":\"PROFILE\",\"subscribee_id\":%1,\"actor_id\":%2,\"client_mutation_id\":\"16\"},\"scale\":1.5}")
                               .arg(subscribee_id)
                               .arg(idPage);
    LOGD << "dataInput: " << dataInput;
    postData.addQueryItem("variables", dataInput.toStdString().c_str());


    QNetworkReply* response = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    while (!response->isFinished()) {
        s_sleep(1, nullptr);
    }
    QString html = "";
    if(response->isFinished())
        html = response->readAll();
    response->deleteLater();

    LOGD << "message followPage: " << html;
    return true;
}

QString FollowPageAPI::getUIDSub()
{
    const QString url = "https://api9.autofarmer.net/public-api/v1/page-sub/get-random";
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *response = manager.get(request);
    while (!response->isFinished()) {
        if(!s_sleep(1, nullptr)) return "";
    }
    QString html = "";
    if(response->isFinished())
        html = response->readAll();
    response->deleteLater();

    LOGD << html;
    html = html.remove("\n");
    QJsonObject rootObj = QJsonDocument::fromJson(html.toUtf8()).object();
    QString UID = "";
    if(rootObj.contains("code") && rootObj.value("code").toInt() == 200)
    {
        if(rootObj.contains("data"))
        {
            QJsonObject dataObj = rootObj.value("data").toObject();
            if(dataObj.contains("uid"))
            {
                LOGD << "uid: " << dataObj.value("uid").toString();
                UID = dataObj.value("uid").toString();
            }
        }
    }
    return UID;
}

QString FollowPageAPI::getPageID(QString UIDClone)
{
    const QString url = QString("https://api9.autofarmer.net/public-api/v1/clone-pages/get-random?token=%1&uid=%2").arg(SQLiteWorker::getInstance()->getToken()).arg(UIDClone);
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *response = manager.get(request);
    while (!response->isFinished()) {
        if(!s_sleep(1, nullptr)) return "";
    }
    QString html = "";
    if(response->isFinished())
        html = response->readAll();
    response->deleteLater();

    LOGD << html;
    html = html.remove("\n");
    QJsonObject rootObj = QJsonDocument::fromJson(html.toUtf8()).object();
    QString pageID = "";
    if(rootObj.contains("code") && rootObj.value("code").toInt() == 200)
    {
        if(rootObj.contains("data"))
        {
            QJsonObject dataObj = rootObj.value("data").toObject();
            if(dataObj.contains("id"))
            {
                LOGD << "id: " << dataObj.value("id").toString();
                pageID = dataObj.value("id").toString();
            }
        }
    }
    return pageID;
}
