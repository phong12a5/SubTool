#include "captchaapi.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <synchapi.h>
#include <QCoreApplication>

captChaAPI::captChaAPI()
{

}

captChaAPI::~captChaAPI()
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

QString captChaAPI::getId2Captcha(QString dataSideKey)
{
    QString url = "http://2captcha.com/in.php?key=a21390271e1975e7b1fd607b52b0f85e&method=userrecaptcha&googlekey=" + dataSideKey + "&pageurl=https://www.google.com/recaptcha/api2/demo";
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
}
