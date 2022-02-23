#ifndef DEFINITION_H
#define DEFINITION_H

#include <QString>

struct CheckPointConfig
{
    QString apiToken;
    QString apiPhone;
    QString apiEmail;
};

struct BaseConfig
{
    bool autoUpdateInfo;
    bool displayImgae;
    bool autoGetCoockie;
    bool autoInitProfile;

    bool logOutAfterInteract;
    bool saveBrowserAfterLogin;
    bool checkLiveUID;
    bool interactAfterSuccess;

    QString profileLink;
};

struct Thread
{
    int numberThread;
    int timeWaittingMin;
    int timeWaittingMax;
    int numberTurn;

};

struct InteractNewFeed
{
    int numberPostFeedMin;
    int numberPostFeedMax;
    int timerRunFeedMin;
    int timerRunFeedMax;
};

struct InteractWatch
{
    int numberPostWatchMin;
    int numberPostWatchMax;
    int timerRunWatchMin;
    int timerRunWatchMax;
};

struct IPConfig
{
   bool checkIpBeforeRun;
   int delayIpAfterChanged;
   int numberTurnToChangeIp;
   bool changeIP;
   bool useProxy;
   bool changeIpHMA;
};

struct DcomSetting
{
    bool nommalSettingDcom;
    bool hilinkSettingDcom;
    QString nameDcom;
    QString linkUrl;
};

struct TinSoftProxySetting
{
    bool useAPIUser;
    bool useAPIProxy;
    QString apiUser;
    QString apiProxy;
    QString location;
    int numberThread;
};

struct TmProxySetting
{
    QString apiKey;
    int numberThread;
    bool httpProxy;
    bool socketProxy;
};

struct XProxyShoplikeSetting
{
    QString apiKey;
    int numberThread;
};

struct Proxyv6Setting
{
    QString apiKey;
    bool typeHttp;
    bool typeSock5s;
    QString proxy;
    int numberThread;
};

struct XProxySetting
{
    bool httpXproxy;
    bool socksXproxy;
    QString proxy;
    int numberThread;
};

#endif // DEFINITION_H
