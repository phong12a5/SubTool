#include "downloadmanager.h"
#include "controllermainthread.h"

extern QMap<DOWNLOAD_TYPE, QString> downloadFileID;

DownLoadManager::DownLoadManager(QObject* parent): QObject(parent)
{
    connect(this, &DownLoadManager::sigStartGDriveDownloadFile, this, &DownLoadManager::startGDriveDownloadFile, Qt::QueuedConnection);

    QString api_file = QDir::currentPath() + "/data/downloadFcare.json";
    QString auth_file = QDir::currentPath() + "/data/author.json";
    LOGD << "api_file: " << api_file;
    LOGD << "auth_file: " << auth_file;
    std::unique_ptr<googleQt::ApiAppInfo> appInfo(new googleQt::ApiAppInfo);
    appInfo->readFromFile(api_file);
    bool isContainAuth = QFile(auth_file).exists();
    std::unique_ptr<googleQt::ApiAuthInfo> authInfo(new googleQt::ApiAuthInfo(auth_file));
    if(!isContainAuth)
    {
        LOGD << "Error, please generate authen file";
    }
    else
    {
        if(!authInfo->reload())
        {
            LOGD << "Failed to load token: " << auth_file;
            return;
        }

        if(!googleQt::GoogleWebAuth::refreshToken(appInfo.get(), authInfo.get()))
        {
            LOGD << "Error, failed to store access token to file: " << auth_file;
        }
        else
        {
            LOGD << "token file refreshed: " << auth_file;
        }
    }
    m_client = googleQt::createClient(appInfo.release(), authInfo.release());
    connect(m_client.get(), &GoogleClient::downloadProgress, this, &DownLoadManager::downloadProgress);
    LOGD << "finished";
}

DownLoadManager::~DownLoadManager()
{

}

void DownLoadManager::startDownload(DOWNLOAD_TYPE type)
{
    m_downloadType = type;
    QString fileID = downloadFileID.value(m_downloadType);
    LOGD << fileID;
    try
    {
        GetFileArg arg1(fileID);
        arg1.setFields("name,size");
        auto f = m_client->gdrive()->getFiles()->get(arg1);
        m_fileSize = f->size();
        LOGD << "loading file: " << f->name() << " size: " << m_fileSize;

        QString downloadPath = QDir::currentPath() + "/downloads";
        if(!QDir(downloadPath).exists())
        {
            if(!QDir::current().mkpath(downloadPath))
            {
                out.close();
                return;
            }
        }

        QString fileName = QDir::currentPath() + "/downloads/" + f->name();
        if(QFile(fileName).exists())
        {
            LOGD << "FILE " << f->name() << " already existed > remove to download new";
            LOGD << "RESULT remove: " << QFile(fileName).remove();
        }
        out.setFileName(fileName);
        if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            LOGD << "Error opening file: " << out.fileName();
            out.close();
            return;
        }

        emit sigStartGDriveDownloadFile(fileID);

        if(DOWNLOAD_TYPE::FILE_RELEASE == m_downloadType) {
            CONTROLLERMAIN->uiUpdateProcessDialogTitle("Đang chuẩn bị tải xuống ...");
            CONTROLLERMAIN->uiUpdateProcessDialogValue(0);
            CONTROLLERMAIN->uiShowHideProcessDialog(true);
        }
    }
    catch (googleQt::GoogleException& e)
    {
        LOGD << "GG Exception: " << e.what();
    }
    catch (std::exception e) {
        LOGD << "Exception: " << e.what();
    }
}

void DownLoadManager::onDownloadCompleted()
{
    out.flush();
    LOGD << "=== file downloaded ===";
    LOGD << googleQt::size_human(out.size());
    out.close();

    switch (m_downloadType) {
    case DOWNLOAD_TYPE::FILE_VERSION:
        emit sigFileVersion();
        break;
    case DOWNLOAD_TYPE::FILE_RELEASE:
        CONTROLLERMAIN->uiShowHideProcessDialog(false);
        CONTROLLERMAIN->uiUpdateProcessDialogTitle("");
        CONTROLLERMAIN->uiUpdateProcessDialogValue(0);
        emit sigFileRelease();
        break;
    default:
        break;
    }

    m_downloadType = DOWNLOAD_TYPE::NONE;
    LOGD << "finished";
}

void DownLoadManager::startGDriveDownloadFile(QString fileID)
{
    try {
        LOGD;
        downloadTimer.start();
        googleQt::gdrive::DownloadFileArg arg(fileID);

        m_client->gdrive()->getFiles()->downloadFile_AsyncCB(arg, &out,
                                                             std::bind(&DownLoadManager::onDownloadCompleted, this), [&](std::unique_ptr<GoogleException> e) {
            LOGD << e->what();
            out.close();
            return;
        });
    }
    catch (googleQt::GoogleException& e)
    {
        LOGD << "GG Exception: " << e.what();
    }
    catch (std::exception e) {
        LOGD << "Exception: " << e.what();
    }
}

void DownLoadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    LOGD << "bytesTotal: " << bytesTotal << " bytesReceived: " << bytesReceived;
    if(m_fileSize <= 0)
        return;

    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTimer.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }
    LOGD << "process: " << QString::fromLatin1("%1 %2").arg(speed, 3, 'f', 1).arg(unit) << " | " << static_cast<float>(bytesTotal/(1024*1000)) << "Mb";

    if(DOWNLOAD_TYPE::FILE_RELEASE == m_downloadType) {
        CONTROLLERMAIN->uiUpdateProcessDialogTitle("Tốc độ tải: " + QString::fromLatin1("%1 %2").arg(speed, 3, 'f', 1).arg(unit));
        CONTROLLERMAIN->uiUpdateProcessDialogValue(bytesReceived*100/bytesTotal);
    }
}
