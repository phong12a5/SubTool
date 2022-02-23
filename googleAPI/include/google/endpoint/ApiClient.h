#pragma once
#include "ApiUtil.h"
#include "ApiAuthInfo.h"
#include "ApiAppInfo.h"

class QNetworkProxy;

namespace googleQt{
    class ApiClient: public QObject,
                     public std::enable_shared_from_this<ApiClient>
    {
        Q_OBJECT
    public:
        ApiClient(ApiAppInfo* appInfo, ApiAuthInfo* authInfo);
        virtual ~ApiClient() {};
        QString getToken()const;
        QString getAppKey()const;
        QString userId()const;
        virtual void setUserId(QString email);
        QString userAgent()const{return m_userAgent;}
        void setUserAgent(QString s){m_userAgent = s;}
        int     getAccessScope()const;
        virtual bool refreshToken() = 0;        
        virtual ApiEndpoint* endpoint() = 0;
    signals:
        void downloadProgress(qint64 bytesProcessed, qint64 total);
        void uploadProgress(qint64 bytesProcessed, qint64 total);

    protected:      
        std::unique_ptr<ApiAppInfo>  m_app;
        std::unique_ptr<ApiAuthInfo> m_auth;
        QString m_userAgent;
    };
};
