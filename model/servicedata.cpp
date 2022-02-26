#include "servicedata.h"
#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QFileInfoList>
#include <QFileInfo>
#include <QList>
#include "log.h"
#include <QSettings>
#include <WebAPI.hpp>

ServiceData::ServiceData(BaseService::SERVICE_TYPE type, int profileId, QObject *parent) :
    QObject(parent),
    m_type(type),
    m_profileId(profileId),
    m_cloneInfo(nullptr)
{
    QSettings settings;
    switch (m_type) {
    case BaseService::SERVICE_TYPE::TYPE_CHROME_SERVICE:
        m_type_str = "chrome";
        break;
    case BaseService::SERVICE_TYPE::TYPE_FIREFOX_SERVICE:
        m_type_str = "firefox";
        break;
    default:
        m_type_str = "default";
        break;
    }

    m_profileFolderPath = QDir::currentPath() + "/" + m_type_str + "/";
    m_profilePath = m_profileFolderPath + "profiles/" + QString::number(profileId);
    loadCloneInfo();
}

ServiceData::~ServiceData()
{
    if(m_cloneInfo) {
        delete m_cloneInfo;
        m_cloneInfo = nullptr;
    }
}

CloneInfo *ServiceData::cloneInfo()
{
    return m_cloneInfo;
}

void ServiceData::setCloneInfo(CloneInfo *cloneInfo)
{
    if(m_cloneInfo) {
        delete m_cloneInfo;
    }
    if(m_cloneInfo != cloneInfo) {
        m_cloneInfo = cloneInfo;

        // random userAgent

        connect(m_cloneInfo, &CloneInfo::cloneInfoChanged, this, &ServiceData::onCloneInfoChanged );
        onCloneInfoChanged();
    }
}

QString ServiceData::pathUploadProfile(QString url)
{
    QDir dir(url);
    QStringList finters;
    finters << "*.jpg" << "*.jpeg" << "*.jpe" << "*.jif" << "*.jfif" << "*.jfi" << "*.webp" <<
               "*.gif" << "*.png" << "*.apng" << "*.bmp" << "*.dib" << "*.tiff" << "*.tif" <<
               "*.svg" << "*.svgz" << "*.ico" << "*.xbm" ;
    dir.setNameFilters(finters);
    QFileInfoList files = dir.entryInfoList();
    QStringList lisImage;
    foreach (QFileInfo fileInfo, files) {
        QString path=fileInfo.absoluteFilePath();
        lisImage.append(path);
        QFile file(path);
        if(!file.exists()){
            LOGD << "This file is not exist";
        }else{
            LOGD << "This file was found " << file.fileName();
        }
    }
    LOGD << "files.length(): " << files.length();
    m_linkImage = lisImage.at(random(files.length() - 1));
    return m_linkImage;
}

void ServiceData::loadCloneInfo()
{
    QSettings settings;
    QString cloneInfoPath = QString(CLONE_INFO_FILED).arg(m_type_str).arg(m_profileId);
    QJsonObject cloneInfo = settings.value(cloneInfoPath).toJsonObject();
    if(cloneInfo.isEmpty()) {
        setCloneInfo(nullptr);
        LOGD << "NULL";
    } else {
        setCloneInfo(new CloneInfo(cloneInfo));
        LOGD << m_cloneInfo->toString();
    }
}

void ServiceData::onCloneInfoChanged(QString action)
{
    LOGD;
    if(action != "" && m_cloneInfo != nullptr) {
        WebAPI::getInstance()->updateClone(nullptr,
                                           action.toUtf8().data(),
                                           m_cloneInfo->appname().toUtf8().data(),
                                           m_cloneInfo->toString().toUtf8().data());
    }

    QSettings settings;
    if(m_cloneInfo && m_cloneInfo->aliveStatus() == CLONE_ALIVE_STATUS_STORE) {
        settings.setValue(cloneInfokey(), m_cloneInfo->toJson());
    } else {
        settings.setValue(cloneInfokey(), QJsonObject());
    }
}
