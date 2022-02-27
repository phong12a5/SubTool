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
    LOGD;
    if(m_cloneInfo) {
        delete m_cloneInfo;
        m_cloneInfo = nullptr;
    }
    LOGD << "done";
}

CloneInfo *ServiceData::cloneInfo()
{
    return m_cloneInfo;
}

void ServiceData::setCloneInfo(CloneInfo *cloneInfo)
{
    if(m_cloneInfo) {
        delete m_cloneInfo;
        m_cloneInfo = nullptr;
    }

    if(m_cloneInfo != cloneInfo) {
        m_cloneInfo = cloneInfo;

        // random userAgent
        if(m_cloneInfo) {
            connect(m_cloneInfo, &CloneInfo::cloneInfoChanged, this, &ServiceData::onCloneInfoChanged );
        }
    }
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
    LOGD << action;
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
        if(m_cloneInfo && m_cloneInfo->aliveStatus() == CLONE_ALIVE_STATUS_CHECKPOINT) {
            delete m_cloneInfo;
            m_cloneInfo = nullptr;
        }
    }
    LOGD << "done";
}
