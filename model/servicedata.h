#ifndef SERVICEDATA_H
#define SERVICEDATA_H

#include <QObject>
#include "cloneinfo.h"
#include "log.h"
#include "fdriver/include/util/futil.h"
#include <windows.h>
#include "AppEnum.h"
#include "cloneinfo.h"

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

    explicit ServiceData(QObject* parent = nullptr);

    CloneInfo* getCloneInfo() { return m_cloneInfo; }

    void  setCloneInfo(CloneInfo* cloneInfo) {
        m_cloneInfo = cloneInfo;
    }

    void setLinkProfile(QString url) {
        m_linkProfile = url;
    }

    QString getLinkProfile() {
        return m_linkProfile;
    }

    void setNumberChrom(int num) {
        m_numberThread = num;
        setX(num);
        setY(num);
    }

    int numberChrom() {
        return m_numberThread;
    }

    void setX(int numberThread) {
        m_xPosstion = 470*(numberThread % 4);

    }
    int getX() {
        return m_xPosstion;
    }
    void setY(int numberThread) {
        m_yPossition = 500 * (numberThread/4);
    }
    int getY() {
        return m_yPossition;
    }

    int random(int maxRan) {
        srand(time(NULL));
        int res = rand() % maxRan + 1;
        int ranSlep = rand() % 200 + 100;
        Sleep(ranSlep);
        return res;
    }

    void setServiceID(int num) {
        m_serviceID = num;
    }
    int getServiceID() {
        LOGD << "getServiceID is delete: " << m_serviceID;
        return m_serviceID;
    }

    void setProxyChromWorker(QString proxy) {
        m_proxy = proxy;
    }
    QString getProxyChromWorker() {
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

    QString pathUploadProfile(QString url);
private:
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
};

#endif // SERVICEDATA_H
