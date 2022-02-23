#ifndef FOLLOWPAGEAPI_H
#define FOLLOWPAGEAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <synchapi.h>
#include <QCoreApplication>
#include <sqliteworker.h>
#include "log.h"
#include "curl/curl.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

class FollowPageAPI
{
public:
    FollowPageAPI();
    QString getIdPage(QString cookie, QString userAgent, QString fb_dtsg);
    bool followPage(QString cookie, QString userAgent, QString fb_dtsg, QString subscribee_id, QString idPage);
    QString getUIDSub();
    QString getPageID(QString UIDClone);
private:

};

#endif // FOLLOWPAGEAPI_H
