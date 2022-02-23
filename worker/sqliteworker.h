#ifndef SQLITEWORKER_H
#define SQLITEWORKER_H

#include <QObject>
#include <QtSql>
#include "qaesencryption.h"
#include "AppDefine.h"
#include "DefineString.h"

#define TABLE_NAME_ACCOUNTS "accounts"
#define TABLE_NAME_LOCAL_DATA "otherData"

class CloneInfo;

enum class DATABASE_UPDATE_CATEGORY: int {
    MIN_DATABASE_CATEGORY = 0,
    PASSWORD,
    HAIFA,
    TOKEN,
    COOKIES,
    STATUS,
    NOTE,
    APP_NAME,
    CHECKPOINT,
    LOG,
    USER_AGENT,
    LANGUAGE,
    EMAIL,
    PASS_MAIL,
    BIRTHDAY,
    GENDER,
    NAME_ACC,
    INFO_JSON_CLONE,
    STT_UNCHECKPOINT,
    MAX_DATABASE_CATEGORY
} typedef DATABASE_CATEGORY;
Q_DECLARE_METATYPE(DATABASE_CATEGORY)

// do not init value to any other enum
enum class DATABASE_LOCAL_DATA_CATEGORY: int {
    MIN_LOCAL_CATEGORY = 0,
    DATABASE_VERSION,
    TOKEN,
    PROFILE_PATH,
    NUMBER_THREAD,

    CHECK_IP_BEFORE_RUN,
    DELAY_IP_AFTER_CHANGED,
    NUMBER_TURN_TO_CHANGE_IP,
    CHANGE_IP,
    USE_PROXY,
    CHANGE_IP_HMA,

    // dcom setting
    NOMMAL_SETTING_DCOM,
    HILINK_SETTING_DCOM,
    NAME_DCOM,
    LINK_URL,

    // TinSoftProxySetting
    USE_API_USER,
    USE_API_PROXY,
    API_USER,
    API_PROXY,
    LOCATION,
    NUMBER_THREAD_TIN_SOFT,

    // XProxyShoplikeSetting
    API_KEY_XPROXY_SHOPLIKE,
    NUMBER_THREAD_XPROXY_SHOPLIKE,

    // TmProxySetting
    API_KEY_TM,
    NUMBER_THREAD_TM,
    HTTP_TMPROXY,
    SOCKET_TMPROXY,

    // Proxyv6Setting
    API_KEY_PROXYV6,
    NUMBER_THREAD_PROXYV6,
    TYPE_HTTP_PROXYV6,
    TYPE_SOCK5S_PROXYV6,
    PROXYV6,

    // XProxySetting
    XPROXY,
    NUMBER_THREAD_XPROXY,
    HTTP_XPORXY,
    SOCKS_XPROXY,

    MAX_LOCAL_CATEGORY,
} typedef LOCAL_CATEGORY;

class SQLiteWorker: public QObject
{
    Q_OBJECT
private:
    static SQLiteWorker* m_InstancePtr;

    SQLiteWorker();
    ~SQLiteWorker();

    //encode
    QByteArray encode(QVariant inputValue);
    QVariant decode(QByteArray encodeValue);
    QVariant decode(QVariant encodeValue);

    // make sure not connect to db in anywhere before call
    void checkDataBaseVersion();
    bool createAccountTable();
    void createLocalDataTable();

    bool updateListCloneFromDataBase();

public:
    static SQLiteWorker* getInstance();

    // SQLWorker thread can have only public function not relate to db
    QString getToken();
    QString getProfilePath();
    int getNumberThread();

    void checkLogToClear();

    QList<CloneInfo *> getListClone();

    bool checkCloneExist(QString cloneId);

    bool getCheckIpBeforeRun();
    int getDelayIpAfterChanged();
    int getNumberTurnToChangeIp();
    bool getChangeIp();
    bool getUseProxy();
    bool getChangeIpHma();

    bool getNommalSettingDcom();
    bool getHilinkSettingDcom();
    QString getNameDcom();
    QString getLinkUrlDcom();

    bool getUseApiUser();
    bool getUseApiProxy();
    QString getApiUser();
    QString getApiProxy();
    QString getLocation();
    int getNumberThreadTinSoft();

    QString getApiKeyTm();
    int getNumberThreadTm();
    bool getHttpTmproxy();
    bool getSocketTmproxy();

    QString getApiKeyXproxyShoplike();
    int getNumberThreadXproxyShoplike();

    QString getApiKeyProxyv6();
    bool getTypeHttpProxyv6();
    bool getTypeSock5sProxyv6();
    QString getProxyv6();
    int getNumberThreadProxyv6();

    bool getHttpXroxy();
    bool getSocksXroxy();
    QString getXproxy();
    int getNumberThreadXproxy();
signals:
    // function as signal to run in worker thread, a woker thread can have only signal functions
    void initialize();

    bool checkConnect();

    bool setToken(QString token);

    bool setProfilePath(QString profilePath);

    bool setNumberThread(int num);

    bool setListClone(QList<CloneInfo *> listClone, bool isDeleteOldList = false);
    bool addClone(CloneInfo* clone, bool isAddToList = true, bool isRefreshListClone = false);
    bool addListClone(QList<CloneInfo *> listClone, bool isAddToList = true, bool isRefreshListClone = false);
    bool deleteCloneById(QString cloneId);

    bool updateCloneById(QString cloneId, DATABASE_CATEGORY category);

    // other
    bool setCheckIpBeforeRun(bool check);
    bool setDelayIpAfterChanged(int num);
    bool setNumberTurnToChangeIp(int num);
    bool setChangeIp(bool check);
    bool setUseProxy(bool check);
    bool setChangeIpHma(bool check);

    bool setNommalSettingDcom(bool check);
    bool setHilinkSettingDcom(bool check);
    bool setNameDcom(QString nameDcom);
    bool setLinkUrl(QString linkUrl);

    bool setUseApiUser(bool check);
    bool setUseApiProxy(bool check);
    bool setApiUser(QString apiUser);
    bool setApiProxy(QString apiProxy);
    bool setLocation(QString location);
    bool setNumberThreadTinSoft(int num);

    bool setApiKeyTm(QString apiKeyTm);
    bool setNumberThreadTm(int num);
    bool setHttpTmproxy(bool check);
    bool setSocketTmproxy(bool check);

    bool setApiKeyXproxyShoplike(QString apiKeyXproxyShoplike);
    bool setNumberThreadXproxyShoplike(int num);

    bool setApiKeyProxyv6(QString apiKeyProxyv6);
    bool setTypeHttpProxyv6(bool check);
    bool setTypeSock5sProxyv6(bool check);
    bool setProxyv6(QString proxyv6);
    bool setNumberThreadProxyv6(int num);

    bool setHttpXroxy(bool check);
    bool setSocksXroxy(bool check);
    bool setXproxy(QString xproxy);
    bool setNumberThreadXproxy(int num);


private slots:

    // handle function signals
    void initializeHandle();

    bool queryDatabase(QString query);

    bool checkConnectHandle();

    bool setTokenHandle(QString token);

    bool setProfilePathHandle(QString profilePath);

    bool setNumberThreadHandle(int num);

    bool setLocalData(LOCAL_CATEGORY category, QVariant data);
    QVariant getLocalData(LOCAL_CATEGORY category);

    bool setListCloneHandle(QList<CloneInfo *> listClone, bool isDeleteOldList = false);
    bool addCloneHandle(CloneInfo* clone, bool isAddToList = true, bool isRefreshListClone = false);
    bool addListCloneHandle(QList<CloneInfo *> listClone, bool isAddToList = true, bool isRefreshListClone = false);
    bool deleteCloneByIdHandle(QString cloneId);

    bool updateCloneByIdHandle(QString cloneId, DATABASE_CATEGORY category);

//    bool updatePassword(QString uid, QString pass);

    bool setCheckIpBeforeRunHandle(bool check);
    bool setDelayIpAfterChangedHandle(int num);
    bool setNumberTurnToChangeIpHandle(int num);
    bool setChangeIpHandle(bool check);
    bool setUseProxyHandle(bool check);
    bool setChangeIpHmaHandle(bool check);

    bool setNommalSettingDcomHandle(bool check);
    bool setHilinkSettingDcomHandle(bool check);
    bool setNameDcomHandle(QString nameDcom);
    bool setLinkUrlHandle(QString linkUrl);

    bool setUseApiUserHandle(bool check);
    bool setUseApiProxyHandle(bool check);
    bool setApiUserHandle(QString apiUser);
    bool setApiProxyHandle(QString apiProxy);
    bool setLocationHandle(QString location);
    bool setNumberThreadTinSoftHandle(int num);

    bool setApiKeyTmHandle(QString apiKeyTm);
    bool setNumberThreadTmHandle(int num);
    bool setHttpTmproxyHandle(bool check);
    bool setSocketTmproxyHandle(bool check);

    bool setApiKeyXproxyShoplikeHandle(QString apiKeyXproxyShoplike);
    bool setNumberThreadXproxyShoplikeHandle(int num);

    bool setApiKeyProxyv6Handle(QString apiKeyProxyv6);
    bool setTypeHttpProxyv6Handle(bool check);
    bool setTypeSock5sProxyv6Handle(bool check);
    bool setProxyv6Handle(QString proxyv6);
    bool setNumberThreadProxyv6Handle(int num);

    bool setHttpXroxyHandle(bool check);
    bool setSocksXroxyHandle(bool check);
    bool setXproxyHandle(QString xproxy);
    bool setNumberThreadXproxyHandle(int num);
private:
    // backup function, review carefully before decide to use
    bool m_initialize = false;
    bool updateCurrentDataToDataBase();
    bool updateDataBase();
    bool updateCloneByIdAllData(QString cloneId);

private:
    QMutex mutex;
    QThread m_workerThread;
    QString m_databaseName;
    QString m_tableAccounts;
    QString m_tableData;
    QSqlDatabase m_mydb;
    QList<CloneInfo *> m_listClone;
    QMap<LOCAL_CATEGORY, QVariant> m_localData;
    QByteArray m_encrypt_key;
    QByteArray m_encrypt_iv;
    QAESEncryption* m_encryption = nullptr;
};

#endif // SQLITEWORKER_H
