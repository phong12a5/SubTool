#include "appmodel.h"
#include <QCoreApplication>
#include "log.h"
#include "AppDefine.h"

AppModel* AppModel::sInstance = nullptr;

AppModel *AppModel::instance()
{
    if(sInstance == nullptr) {
        sInstance = new AppModel();
    }
    return sInstance;
}

AppModel::AppModel() :
    m_appStarted(false)
{
    QSettings settings;
    if(settings.contains(MAX_THREAD_FIELD))
        m_maxThread = settings.value(MAX_THREAD_FIELD).toInt();
    else
        m_maxThread = 2;

    if(settings.contains(TOKEN_FIELD))
        m_token = settings.value(TOKEN_FIELD).toString();
    else
        m_token = "";
}


bool AppModel::appStarted() { return m_appStarted; }
void AppModel::setAppStarted(bool state) {
    if(m_appStarted != state) {
        m_appStarted = state;
        emit appStartedChanged();
    }
}

int AppModel::maxThread() { return m_maxThread; }
void AppModel::setMaxThread(int max) {
    if(max != m_maxThread) {
        m_maxThread = max;
        QSettings settings;
        settings.setValue(MAX_THREAD_FIELD, max);
        emit maxThreadChanged();
    }
}

QString AppModel::token()
{
    return m_token;
}

void AppModel::setToken(QString newToken)
{
    if(newToken != m_token) {
        m_token = newToken;
        QSettings settings;
        settings.setValue(TOKEN_FIELD, newToken);
        emit tokenChanged();
    }
}
