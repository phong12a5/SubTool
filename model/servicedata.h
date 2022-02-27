#ifndef SERVICEDATA_H
#define SERVICEDATA_H

#include <QObject>
#include "cloneinfo.h"
#include "log.h"
#include "fdriver/include/util/futil.h"
#include <windows.h>
#include "AppEnum.h"
#include "cloneinfo.h"
#include <service/baseservice.h>
#include <AppDefine.h>
#include <QSize>
#include <QPoint>

class ServiceData : public QObject
{
    Q_OBJECT

public:

    enum ACTION : int {
        ACTION_LOGIN = 0,
        ACTION_SCROLL_FEED,
        ACCTION_FEED_LIKE,
        ACCTION_NEWFEED_BUTTON,
        ACCTION_FRIEND_BUTTON
    };

    explicit ServiceData(BaseService::SERVICE_TYPE type, int profileId, QObject* parent = nullptr);
    ~ServiceData();

    QString profilePath() { return m_profilePath; }

    QString cloneInfokey() { return QString(CLONE_INFO_FILED).arg(m_type_str).arg(m_profileId);}

    CloneInfo* cloneInfo();
    void  setCloneInfo(CloneInfo* cloneInfo);

    void setLinkProfile(QString url) {
        m_linkProfile = url;
    }

    QString getLinkProfile() {
        return m_linkProfile;
    }

    int numberChrom() {
        return m_numberThread;
    }

    QPoint windowPosition() {
        return m_posstion;
    }

    void setWindowPosition(QPoint pos) {
        m_posstion = pos;
    }

    QSize windowSize() {
        return m_windowSize;
    }

    void setWindowSize(QSize size) {
        m_windowSize = size;;
    }

    void setServiceID(int num) {
        m_serviceID = num;
    }
    int getServiceID() {
        return m_serviceID;
    }

    void setProxy(QString proxy) {
        LOGD << proxy;
        m_proxy = proxy;
    }
    QString getProxy() {
        return m_proxy;
    }

    void setProxyKind(const PROXY_KIND proxyKind){
        if(proxyKind != m_proxyKind){
            m_proxyKind = proxyKind;
        }
    }
    PROXY_KIND getProxyKind() const{
        return m_proxyKind;
    }
private:
    void loadCloneInfo();

private slots:
    void onCloneInfoChanged(QString action = "");

private:
    BaseService::SERVICE_TYPE m_type;
    QString m_type_str;
    int m_profileId;
    QString m_profileFolderPath;
    QString m_profilePath;
    CloneInfo* m_cloneInfo;
    std::string m_2Fa;
    int m_numberThread;
    int m_xPosstion;
    int m_yPossition;
    QString m_linkImage;
    int m_serviceID;
    QString m_proxy;
    PROXY_KIND m_proxyKind = PROXY_KIND::HTTP;
    QString m_linkProfile;
    QPoint m_posstion;
    QSize m_windowSize;
};

#endif // SERVICEDATA_H
