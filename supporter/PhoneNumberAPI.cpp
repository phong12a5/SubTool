#include "PhoneNumberAPI.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <synchapi.h>
#include <QCoreApplication>
#include <sqliteworker.h>

PhoneNumberAPI::PhoneNumberAPI()
    : m_isReleased(false)
    , m_userData("")
{
}

PhoneNumberAPI::~PhoneNumberAPI()
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

QString PhoneNumberAPI::getPhoneNumber()
{
    const QString url = "https://api-textnow.otp123.com/public-api/v1/text-now/get-random?token=" + SQLiteWorker::getInstance()->getToken();
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
    if(rootObj.contains("code") && rootObj.value("code").toInt() == 200)
    {
        if(rootObj.contains("data"))
        {
            QJsonObject dataObj = rootObj.value("data").toObject();
            m_userData = QJsonDocument(dataObj).toJson(QJsonDocument::Compact);
            if(dataObj.contains("phoneNumber"))
            {
                LOGD << "Phone number: " << dataObj.value("phoneNumber").toString();
                return dataObj.value("phoneNumber").toString();
            }
        }
    }

    return "";
}

QString PhoneNumberAPI::getOTP()
{
    if(m_userData.trimmed().isEmpty())
    {
        return "";
    }

    QJsonObject userObj = QJsonDocument::fromJson(m_userData.toUtf8()).object();
    if(userObj.contains("userName"))
    {
        QString userName = userObj.value("userName").toString();
        LOGD << userName;
        const QString url = "https://api-textnow.otp123.com/public-api/v1/text-now/get-otp";
        QNetworkAccessManager manager;
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QString postData = QString("{\"username\": \"%1\"}").arg(userName);
        QNetworkReply* response = manager.post(request, postData.toUtf8());
        while (!response->isFinished()) {
            s_sleep(1, nullptr);
        }
        QString html = "";
        if(response->isFinished())
            html = response->readAll();
        response->deleteLater();

        LOGD << "message OPT:" << html;
        QJsonObject rootObj = QJsonDocument::fromJson(html.toUtf8()).object();
        if(rootObj.contains("code") && rootObj.value("code").toInt() == 200)
        {
            if(rootObj.contains("data"))
            {
                QJsonObject dataObj = rootObj.value("data").toObject();
                if(dataObj.contains("otp"))
                {
                    LOGD << "OTP: " << dataObj.value("otp").toString();
                    return dataObj.value("otp").toString();
                }
            }
        }
    }

    return "";
}
