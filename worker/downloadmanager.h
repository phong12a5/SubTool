#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <google/endpoint/ApiAppInfo.h>
#include <google/endpoint/ApiAuthInfo.h>
#include <google/endpoint/GoogleWebAuth.h>
#include <GoogleClient.h>
#include <gdrive/GdriveRequestArg.h>
#include <gdrive/GdriveRoutes.h>
#include <QObject>
#include <QDir>
#include <log.h>
#include <QtCore>
#include "AppEnum.h"
#include "AppDefine.h"

using namespace std;
using namespace googleQt;
using namespace googleQt::gdrive;

class DownLoadManager : public QObject
{
    Q_OBJECT
public:
    DownLoadManager(QObject* parent = nullptr);
    ~DownLoadManager();
    void startDownload(DOWNLOAD_TYPE type);
    void onDownloadCompleted();

signals:
    void sigStartGDriveDownloadFile(QString fileID);

    void sigFileVersion();
    void sigFileRelease();


private slots:
    void startGDriveDownloadFile(QString fileID);

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

protected:
    gclient_ptr m_client;
    quint64 m_fileSize;
    QFile out;
    QElapsedTimer downloadTimer;
    DOWNLOAD_TYPE m_downloadType = DOWNLOAD_TYPE::NONE;


};

#endif // DOWNLOADMANAGER_H
