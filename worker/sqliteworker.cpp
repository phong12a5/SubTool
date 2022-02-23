#include "sqliteworker.h"
#include "log.h"
#include "CommonInforClone.h"
#include <QMutex>
#include "controllermainthread.h"
#include "utils.h"

SQLiteWorker* SQLiteWorker::m_InstancePtr = nullptr;

SQLiteWorker::SQLiteWorker()
{
    // signal slot of function signal, must be QueuedConnection to run in this thread
    connect(this, &SQLiteWorker::initialize, this, &SQLiteWorker::initializeHandle, Qt::DirectConnection);
    connect(this, &SQLiteWorker::checkConnect, this, &SQLiteWorker::checkConnectHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setToken, this, &SQLiteWorker::setTokenHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setProfilePath, this, &SQLiteWorker::setProfilePathHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setNumberThread, this, &SQLiteWorker::setNumberThreadHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setListClone, this, &SQLiteWorker::setListCloneHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::addClone, this, &SQLiteWorker::addCloneHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::addListClone, this, &SQLiteWorker::addListCloneHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::deleteCloneById, this, &SQLiteWorker::deleteCloneByIdHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::updateCloneById, this, &SQLiteWorker::updateCloneByIdHandle, Qt::QueuedConnection);

    //other
    connect(this, &SQLiteWorker::setCheckIpBeforeRun, this, &SQLiteWorker::setCheckIpBeforeRunHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setDelayIpAfterChanged, this, &SQLiteWorker::setDelayIpAfterChangedHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setNumberTurnToChangeIp, this, &SQLiteWorker::setNumberTurnToChangeIpHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setChangeIp, this, &SQLiteWorker::setChangeIpHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setUseProxy, this, &SQLiteWorker::setUseProxyHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setChangeIpHma, this, &SQLiteWorker::setChangeIpHmaHandle, Qt::QueuedConnection);

    connect(this, &SQLiteWorker::setNommalSettingDcom, this, &SQLiteWorker::setNommalSettingDcomHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setHilinkSettingDcom, this, &SQLiteWorker::setHilinkSettingDcomHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setNameDcom, this, &SQLiteWorker::setNameDcomHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setLinkUrl, this, &SQLiteWorker::setLinkUrlHandle, Qt::QueuedConnection);

    connect(this, &SQLiteWorker::setUseApiUser, this, &SQLiteWorker::setUseApiUserHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setUseApiProxy, this, &SQLiteWorker::setUseApiProxyHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setApiUser, this, &SQLiteWorker::setApiUserHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setApiProxy, this, &SQLiteWorker::setApiProxyHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setLocation, this, &SQLiteWorker::setLocationHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setNumberThreadTinSoft, this, &SQLiteWorker::setNumberThreadTinSoftHandle, Qt::QueuedConnection);

    connect(this, &SQLiteWorker::setApiKeyTm, this, &SQLiteWorker::setApiKeyTmHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setNumberThreadTm, this, &SQLiteWorker::setNumberThreadTmHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setHttpTmproxy, this, &SQLiteWorker::setHttpTmproxyHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setSocketTmproxy, this, &SQLiteWorker::setSocketTmproxyHandle, Qt::QueuedConnection);

    connect(this, &SQLiteWorker::setApiKeyXproxyShoplike, this, &SQLiteWorker::setApiKeyXproxyShoplikeHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setNumberThreadXproxyShoplike, this, &SQLiteWorker::setNumberThreadXproxyShoplikeHandle, Qt::QueuedConnection);

    connect(this, &SQLiteWorker::setApiKeyProxyv6, this, &SQLiteWorker::setApiKeyProxyv6Handle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setTypeHttpProxyv6, this, &SQLiteWorker::setTypeHttpProxyv6Handle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setTypeSock5sProxyv6, this, &SQLiteWorker::setTypeSock5sProxyv6Handle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setProxyv6, this, &SQLiteWorker::setProxyv6Handle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setNumberThreadProxyv6, this, &SQLiteWorker::setNumberThreadProxyv6Handle, Qt::QueuedConnection);

    connect(this, &SQLiteWorker::setHttpXroxy, this, &SQLiteWorker::setHttpXroxyHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setSocksXroxy, this, &SQLiteWorker::setSocksXroxyHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setXproxy, this, &SQLiteWorker::setXproxyHandle, Qt::QueuedConnection);
    connect(this, &SQLiteWorker::setNumberThreadXproxy, this, &SQLiteWorker::setNumberThreadXproxyHandle, Qt::QueuedConnection);
    m_databaseName = DATABASE_NAME_DEFAULT;
    m_tableAccounts = TABLE_NAME_ACCOUNTS;
    m_tableData = TABLE_NAME_LOCAL_DATA;
    m_mydb = QSqlDatabase::addDatabase("QSQLITE");
    m_mydb.setDatabaseName(m_databaseName);

    m_encryption = new QAESEncryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    m_encrypt_key = QString(DEFAULT_KEY).toLocal8Bit();
    m_encrypt_iv = QString(DEFAULT_IV).toLocal8Bit();
}

QByteArray SQLiteWorker::encode(QVariant inputValue)
{
//    return inputValue.toByteArray();
    return m_encryption->encode(inputValue.toByteArray(), m_encrypt_key, m_encrypt_iv);
}

QVariant SQLiteWorker::decode(QByteArray encodeValue)
{
//   return QVariant(encodeValue);
    QByteArray decodeValue = m_encryption->decode(encodeValue, m_encrypt_key, m_encrypt_iv);
    return QVariant(m_encryption->removePadding(decodeValue));
}

QVariant SQLiteWorker::decode(QVariant encodeValue)
{
//    return encodeValue;
    QByteArray decodeValue = m_encryption->decode(encodeValue.toByteArray(), m_encrypt_key, m_encrypt_iv);
    return QVariant(m_encryption->removePadding(decodeValue));
}

SQLiteWorker *SQLiteWorker::getInstance()
{
    if(nullptr == m_InstancePtr) {
        m_InstancePtr = new SQLiteWorker();
    }
    return m_InstancePtr;
}

SQLiteWorker::~SQLiteWorker()
{
    if(!m_listClone.isEmpty()) {
        qDeleteAll(m_listClone);
        m_listClone.clear();
    }
    if(nullptr != m_encryption) {
        delete m_encryption;
        m_encryption = nullptr;
    }
}

void SQLiteWorker::initializeHandle()
{
    LOGD;
    if(m_initialize){
        return;
    }

    createLocalDataTable();
    checkDataBaseVersion();

    // Read local data
    for(int i = (int)LOCAL_CATEGORY::MIN_LOCAL_CATEGORY + 1; i < (int)LOCAL_CATEGORY::MAX_LOCAL_CATEGORY; i++) {
        LOCAL_CATEGORY cat = static_cast<LOCAL_CATEGORY>(i);
        m_localData.insert(cat, getLocalData(static_cast<LOCAL_CATEGORY>(i)));
    }

    QString token = getToken();
    m_tableAccounts = TABLE_NAME_ACCOUNTS + token;
    createAccountTable();

    updateListCloneFromDataBase();
}

bool SQLiteWorker::createAccountTable()
{
    bool accounts = queryDatabase("CREATE TABLE IF NOT EXISTS " + m_tableAccounts + " (cloneId BLOB not null primary key, UID BLOB, passWord BLOB, "
                                    "haiFA BLOB, token BLOB, cookies BLOB, status BLOB, note BLOB, appName BLOB, checkPoint BLOB, log BLOB, "
                                    "userAgent BLOB, language BLOB, email BLOB, passMail BLOB, birthDay BLOB, gender BLOB, nameAcc BLOB, infoJsonClone BLOB, sttUncheckPoint BLOB);");
    return accounts;
}

void SQLiteWorker::createLocalDataTable()
{
    // create table local data
    queryDatabase("CREATE TABLE IF NOT EXISTS " + m_tableData + " (category BLOB not null primary key, data BLOB);");

    // init local data
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + DATABASE_VERSION_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + TOKEN_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + PROFILE_PATH_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + NUMBER_THREAD_STRING + "', '')");

    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + CHECK_IP_BEFORE_RUN_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + DELAY_IP_AFTER_CHANGED_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + NUMBER_TURN_TO_CHANGE_IP_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + CHANGE_IP_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + USE_PROXY_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + CHANGE_IP_HMA_STRING + "', '')");

    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + NOMMAL_SETTING_DCOM_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + HILINK_SETTING_DCOM_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + NAME_DCOM_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + LINK_URL_STRING + "', '')");

    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + USE_API_USER_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + USE_API_PROXY_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + API_USER_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + API_PROXY_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + LOCATION_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + NUMBER_THREAD_TIN_SOFT_STRING + "', '')");

    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + API_KEY_TM_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + NUMBER_THREAD_TM_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + HTTP_TMPROXY_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + SOCKET_TMPROXY_STRING + "', '')");

    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + API_KEY_XPROXY_SHOPLIKE_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + SOCKET_TMPROXY_STRING + "', '')");

    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + API_KEY_PROXYV6_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + TYPE_HTTP_PROXYV6_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + TYPE_SOCK5S_PROXYV6_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + PROXYV6_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + NUMBER_THREAD_PROXYV6_STRING + "', '')");

    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + HTTP_XPORXY_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + SOCKS_XPROXY_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + XPROXY_STRING + "', '')");
    queryDatabase("INSERT OR IGNORE INTO " + m_tableData + " (category, data) VALUES ('" + NUMBER_THREAD_XPROXY_STRING + "', '')");
}

void SQLiteWorker::checkLogToClear()
{
    foreach (CommonInforClone* clone, m_listClone) {
        clone->checkLogToClear();
    }
}

bool SQLiteWorker::queryDatabase(QString query)
{
    LOGD << query;
    if(m_databaseName=="") {
        LOGD << "Database name is empty";
        return false;
    }

    if(!m_mydb.open()){
        LOGD << "Cannot open database!";
        return false;
    } else {
        QSqlQuery qry(m_mydb);
        qry.prepare(query);
        if(!qry.exec() ) {
            LOGD << "Query error: " << qry.lastError();
            m_mydb.close();
            return false;
        } else {
            m_mydb.close();
            LOGD << "Query success";
            return true;
        }
    }
}

bool SQLiteWorker::checkConnectHandle()
{
    if(m_databaseName=="") {
        LOGD << "Table name is empty";
        return false;
    }

    if(!m_mydb.open()) {
        LOGD << "Cannot open database!";
        return false;
    } else {
        m_mydb.close();
        LOGD << "Open database success";
        return true;
    }
}

bool SQLiteWorker::setTokenHandle(QString token)
{
    LOGD << token;
    if(token.isEmpty()){
        return false;
    }
    QString backupToken = getToken();

    if(backupToken == token){
        LOGD << "Same token";
        return false;
    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::TOKEN, token)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::TOKEN] = token;
    m_tableAccounts = TABLE_NAME_ACCOUNTS + token;
    // Try to create table of token if not exist
    createAccountTable();

    // update list clone again
    updateListCloneFromDataBase();
    return true;
}

QString SQLiteWorker::getToken()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::TOKEN, QString()).toString();
}

bool SQLiteWorker::setProfilePathHandle(QString profilePath)
{
    LOGD << profilePath;
    if(profilePath.isEmpty()){
        return false;
    }
    QString backupPath = getProfilePath();

    if(backupPath == profilePath){
        LOGD << "Same profile path";
        return false;
    }

    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::PROFILE_PATH, profilePath)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::PROFILE_PATH] = profilePath;
    return true;
}

QString SQLiteWorker::getProfilePath()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::PROFILE_PATH, QString()).toString();
}

bool SQLiteWorker::setNumberThreadHandle(int num)
{
    LOGD << num;
    if(num <= 0){
        return false;
    }
    int backupNum = getNumberThread();

    if(backupNum == num){
        LOGD << "Same number thread";
        return false;
    }

    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::NUMBER_THREAD, num)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NUMBER_THREAD] = num;
    return true;
}

int SQLiteWorker::getNumberThread()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NUMBER_THREAD, QString()).toInt();
}

bool SQLiteWorker::setCheckIpBeforeRunHandle(bool check)
{
    LOGD << "CheckIpBeforeRunHandle " << check;
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::CHECK_IP_BEFORE_RUN, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::CHECK_IP_BEFORE_RUN] = check;
    return true;
}

bool SQLiteWorker::getCheckIpBeforeRun()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::CHECK_IP_BEFORE_RUN, QString()).toBool();
}

bool SQLiteWorker::setDelayIpAfterChangedHandle(int num)
{
    LOGD << num;
    if(num <= 0){
        return false;
    }

    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::DELAY_IP_AFTER_CHANGED, num)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::DELAY_IP_AFTER_CHANGED] = num;
    return true;
}

int SQLiteWorker::getDelayIpAfterChanged()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::DELAY_IP_AFTER_CHANGED, QString()).toInt();
}

bool SQLiteWorker::setNumberTurnToChangeIpHandle(int num)
{
    LOGD << num;
    if(num <= 0){
        return false;
    }

    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::NUMBER_TURN_TO_CHANGE_IP, num)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NUMBER_TURN_TO_CHANGE_IP] = num;
    return true;
}

int SQLiteWorker::getNumberTurnToChangeIp()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NUMBER_TURN_TO_CHANGE_IP, QString()).toInt();
}

bool SQLiteWorker::setChangeIpHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::CHANGE_IP, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::CHANGE_IP] = check;
    return true;
}

bool SQLiteWorker::getChangeIp()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::CHANGE_IP, QString()).toBool();
}

bool SQLiteWorker::setUseProxyHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::USE_PROXY, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::USE_PROXY] = check;
    return true;
}

bool SQLiteWorker::getUseProxy()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::USE_PROXY, QString()).toBool();
}

bool SQLiteWorker::setChangeIpHmaHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::CHANGE_IP_HMA, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::CHANGE_IP_HMA] = check;
    return true;
}

bool SQLiteWorker::getChangeIpHma()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::CHANGE_IP_HMA, QString()).toBool();
}

bool SQLiteWorker::setNommalSettingDcomHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::NOMMAL_SETTING_DCOM, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NOMMAL_SETTING_DCOM] = check;
    return true;
}

bool SQLiteWorker::getNommalSettingDcom()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NOMMAL_SETTING_DCOM, QString()).toBool();
}

bool SQLiteWorker::setHilinkSettingDcomHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::NOMMAL_SETTING_DCOM, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NOMMAL_SETTING_DCOM] = check;
    return true;
}

bool SQLiteWorker::getHilinkSettingDcom()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NOMMAL_SETTING_DCOM, QString()).toBool();
}

bool SQLiteWorker::setNameDcomHandle(QString nameDcom)
{
//    if(nameDcom.isEmpty()){
//        return false;
//    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::NAME_DCOM, nameDcom)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NAME_DCOM] = nameDcom;
    return true;
}

QString SQLiteWorker::getNameDcom()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NAME_DCOM, QString()).toString();
}

bool SQLiteWorker::setLinkUrlHandle(QString linkUrl)
{
//    if(linkUrl.isEmpty()){
//        return false;
//    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::LINK_URL, linkUrl)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::LINK_URL] = linkUrl;
    return true;
}

QString SQLiteWorker::getLinkUrlDcom()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::LINK_URL, QString()).toString();
}

bool SQLiteWorker::setUseApiUserHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::USE_API_USER, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::USE_API_USER] = check;
    return true;
}

bool SQLiteWorker::getUseApiUser()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::USE_API_USER, QString()).toBool();
}

bool SQLiteWorker::setUseApiProxyHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::USE_API_PROXY, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::USE_API_PROXY] = check;
    return true;
}

bool SQLiteWorker::getUseApiProxy()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::USE_API_PROXY, QString()).toBool();
}

bool SQLiteWorker::setApiUserHandle(QString apiUser)
{
//    if(apiUser.isEmpty()){
//        return false;
//    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::API_USER, apiUser)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::API_USER] = apiUser;
    return true;
}

QString SQLiteWorker::getApiUser()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::API_USER, QString()).toString();
}

bool SQLiteWorker::setApiProxyHandle(QString apiProxy)
{
//    if(apiProxy.isEmpty()){
//        return false;
//    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::API_PROXY, apiProxy)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::API_PROXY] = apiProxy;
    return true;
}

QString SQLiteWorker::getApiProxy()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::API_PROXY, QString()).toString();
}

bool SQLiteWorker::setLocationHandle(QString location)
{

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::LOCATION, location)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::LOCATION] = location;
    return true;
}

QString SQLiteWorker::getLocation()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::LOCATION, QString()).toString();
}

bool SQLiteWorker::setNumberThreadTinSoftHandle(int num)
{
    LOGD << num;
    if(num <= 0){
        return false;
    }

    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::NUMBER_THREAD_TIN_SOFT, num)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NUMBER_THREAD_TIN_SOFT] = num;
    return true;
}

int SQLiteWorker::getNumberThreadTinSoft()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NUMBER_THREAD_TIN_SOFT, QString()).toInt();
}

bool SQLiteWorker::setApiKeyTmHandle(QString apiKeyTm)
{
//    if(apiKeyTm.isEmpty()){
//        return false;
//    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::API_KEY_TM, apiKeyTm)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::API_KEY_TM] = apiKeyTm;
    return true;
}

QString SQLiteWorker::getApiKeyTm()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::API_KEY_TM, QString()).toString();
}

bool SQLiteWorker::setNumberThreadTmHandle(int num)
{
    LOGD << num;
    if(num <= 0){
        return false;
    }

    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::NUMBER_THREAD_TM, num)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NUMBER_THREAD_TM] = num;
    return true;
}

int SQLiteWorker::getNumberThreadTm()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NUMBER_THREAD_TM, QString()).toInt();
}

bool SQLiteWorker::setHttpTmproxyHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::HTTP_TMPROXY, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::HTTP_TMPROXY] = check;
    return true;
}

bool SQLiteWorker::getHttpTmproxy()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::HTTP_TMPROXY, QString()).toBool();
}

bool SQLiteWorker::setSocketTmproxyHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::SOCKET_TMPROXY, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::SOCKET_TMPROXY] = check;
    return true;
}

bool SQLiteWorker::getSocketTmproxy()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::SOCKET_TMPROXY, QString()).toBool();
}

bool SQLiteWorker::setApiKeyXproxyShoplikeHandle(QString apiKeyXproxyShoplike)
{
//    if(apiKeyXproxyShoplike.isEmpty()){
//        return false;
//    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::API_KEY_XPROXY_SHOPLIKE, apiKeyXproxyShoplike)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::API_KEY_XPROXY_SHOPLIKE] = apiKeyXproxyShoplike;
    return true;
}

QString SQLiteWorker::getApiKeyXproxyShoplike()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::API_KEY_XPROXY_SHOPLIKE, QString()).toString();
}

bool SQLiteWorker::setNumberThreadXproxyShoplikeHandle(int num)
{
    LOGD << num;
    if(num <= 0){
        return false;
    }

    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::NUMBER_THREAD_XPROXY_SHOPLIKE, num)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NUMBER_THREAD_XPROXY_SHOPLIKE] = num;
    return true;
}

int SQLiteWorker::getNumberThreadXproxyShoplike()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NUMBER_THREAD_XPROXY_SHOPLIKE, QString()).toInt();
}

bool SQLiteWorker::setApiKeyProxyv6Handle(QString apiKeyProxyv6)
{
//    if(apiKeyProxyv6.isEmpty()){
//        return false;
//    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::API_KEY_PROXYV6, apiKeyProxyv6)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::API_KEY_PROXYV6] = apiKeyProxyv6;
    return true;
}

QString SQLiteWorker::getApiKeyProxyv6()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::API_KEY_PROXYV6, QString()).toString();
}

bool SQLiteWorker::setTypeHttpProxyv6Handle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::TYPE_HTTP_PROXYV6, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::TYPE_HTTP_PROXYV6] = check;
    return true;
}

bool SQLiteWorker::getTypeHttpProxyv6()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::TYPE_HTTP_PROXYV6, QString()).toBool();
}

bool SQLiteWorker::setTypeSock5sProxyv6Handle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::TYPE_SOCK5S_PROXYV6, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::TYPE_SOCK5S_PROXYV6] = check;
    return true;
}

bool SQLiteWorker::getTypeSock5sProxyv6()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::TYPE_SOCK5S_PROXYV6, QString()).toBool();
}

bool SQLiteWorker::setProxyv6Handle(QString proxyv6)
{
//    if(proxyv6.isEmpty()){
//        return false;
//    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::PROXYV6, proxyv6)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::PROXYV6] = proxyv6;
    return true;
}

QString SQLiteWorker::getProxyv6()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::PROXYV6, QString()).toString();
}

bool SQLiteWorker::setNumberThreadProxyv6Handle(int num)
{
    LOGD << num;
    if(num <= 0){
        return false;
    }

    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::NUMBER_THREAD_PROXYV6, num)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NUMBER_THREAD_PROXYV6] = num;
    return true;
}

int SQLiteWorker::getNumberThreadProxyv6()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NUMBER_THREAD_PROXYV6, QString()).toInt();
}

bool SQLiteWorker::setHttpXroxyHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::HTTP_XPORXY, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::HTTP_XPORXY] = check;
    return true;
}

bool SQLiteWorker::getHttpXroxy()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::HTTP_XPORXY, QString()).toBool();
}

bool SQLiteWorker::setSocksXroxyHandle(bool check)
{
    if(check == NULL)
    {
        return false;
    }
    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::SOCKS_XPROXY, check)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::SOCKS_XPROXY] = check;
    return true;
}

bool SQLiteWorker::getSocksXroxy()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::SOCKS_XPROXY, QString()).toBool();
}

bool SQLiteWorker::setXproxyHandle(QString xproxy)
{
//    if(xproxy.isEmpty()){
//        return false;
//    }

    // set new token to db
    if(!setLocalData(LOCAL_CATEGORY::XPROXY, xproxy)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::XPROXY] = xproxy;
    return true;
}

QString SQLiteWorker::getXproxy()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::XPROXY, QString()).toString();
}

bool SQLiteWorker::setNumberThreadXproxyHandle(int num)
{
    LOGD << num;
    if(num <= 0){
        return false;
    }

    // set new profilePath to db
    if(!setLocalData(LOCAL_CATEGORY::NUMBER_THREAD_XPROXY, num)){
        LOGD << "Fail to set to database!";
        return false;
    }

    m_localData[LOCAL_CATEGORY::NUMBER_THREAD_XPROXY] = num;
    return true;
}

int SQLiteWorker::getNumberThreadXproxy()
{
    QMutexLocker locker(&mutex);
    return m_localData.value(LOCAL_CATEGORY::NUMBER_THREAD_XPROXY, QString()).toInt();
}

bool SQLiteWorker::setLocalData(LOCAL_CATEGORY category, QVariant data)
{
    LOGD << (int)category << ": " << data;
    // encode data
    QByteArray encodeData = encode(data);

    QString categoryUpdate;
    switch (category) {
    case LOCAL_CATEGORY::DATABASE_VERSION:
        categoryUpdate = DATABASE_VERSION_STRING;
        break;
    case LOCAL_CATEGORY::TOKEN:
        categoryUpdate = TOKEN_STRING;
        break;
    case LOCAL_CATEGORY::PROFILE_PATH:
        categoryUpdate = PROFILE_PATH_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD:
        categoryUpdate = NUMBER_THREAD_STRING;
        break;

    // ipconfig
    case LOCAL_CATEGORY::CHECK_IP_BEFORE_RUN:
        categoryUpdate = CHECK_IP_BEFORE_RUN_STRING;
        break;
    case LOCAL_CATEGORY::DELAY_IP_AFTER_CHANGED:
        categoryUpdate = DELAY_IP_AFTER_CHANGED_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_TURN_TO_CHANGE_IP:
        categoryUpdate = NUMBER_TURN_TO_CHANGE_IP_STRING;
        break;
    case LOCAL_CATEGORY::CHANGE_IP:
        categoryUpdate = CHANGE_IP_STRING;
        break;
    case LOCAL_CATEGORY::USE_PROXY:
        categoryUpdate = USE_PROXY_STRING;
        break;
    case LOCAL_CATEGORY::CHANGE_IP_HMA:
        categoryUpdate = CHANGE_IP_HMA_STRING;
        break;

    // dcom setting
    case LOCAL_CATEGORY::NOMMAL_SETTING_DCOM:
        categoryUpdate = NOMMAL_SETTING_DCOM_STRING;
        break;
    case LOCAL_CATEGORY::HILINK_SETTING_DCOM:
        categoryUpdate = HILINK_SETTING_DCOM_STRING;
        break;
    case LOCAL_CATEGORY::NAME_DCOM:
        categoryUpdate = NAME_DCOM_STRING;
        break;
    case LOCAL_CATEGORY::LINK_URL:
        categoryUpdate = LINK_URL_STRING;
        break;

    case LOCAL_CATEGORY::USE_API_USER:
        categoryUpdate = USE_API_USER_STRING;
        break;
    case LOCAL_CATEGORY::USE_API_PROXY:
        categoryUpdate = USE_API_PROXY_STRING;
        break;
    case LOCAL_CATEGORY::API_USER:
        categoryUpdate = API_USER_STRING;
        break;
    case LOCAL_CATEGORY::API_PROXY:
        categoryUpdate = API_PROXY_STRING;
        break;
    case LOCAL_CATEGORY::LOCATION:
        categoryUpdate = LOCATION_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD_TIN_SOFT:
        categoryUpdate = NUMBER_THREAD_TIN_SOFT_STRING;
        break;

    case LOCAL_CATEGORY::API_KEY_TM:
        categoryUpdate = API_KEY_TM_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD_TM:
        categoryUpdate = NUMBER_THREAD_TM_STRING;
        break;
    case LOCAL_CATEGORY::HTTP_TMPROXY:
        categoryUpdate = HTTP_TMPROXY_STRING;
        break;
    case LOCAL_CATEGORY::SOCKET_TMPROXY:
        categoryUpdate = SOCKET_TMPROXY_STRING;
        break;

    case LOCAL_CATEGORY::API_KEY_XPROXY_SHOPLIKE:
        categoryUpdate = API_KEY_XPROXY_SHOPLIKE_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD_XPROXY_SHOPLIKE:
        categoryUpdate = NUMBER_THREAD_XPROXY_SHOPLIKE_STRING;
        break;

    case LOCAL_CATEGORY::API_KEY_PROXYV6:
        categoryUpdate = API_KEY_PROXYV6_STRING;
        break;
    case LOCAL_CATEGORY::TYPE_HTTP_PROXYV6:
        categoryUpdate = TYPE_HTTP_PROXYV6_STRING;
        break;
    case LOCAL_CATEGORY::TYPE_SOCK5S_PROXYV6:
        categoryUpdate = TYPE_SOCK5S_PROXYV6_STRING;
        break;
    case LOCAL_CATEGORY::PROXYV6:
        categoryUpdate = PROXYV6_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD_PROXYV6:
        categoryUpdate = NUMBER_THREAD_PROXYV6_STRING;
        break;

    case LOCAL_CATEGORY::HTTP_XPORXY:
        categoryUpdate = HTTP_XPORXY_STRING;
        break;
    case LOCAL_CATEGORY::SOCKS_XPROXY:
        categoryUpdate = SOCKS_XPROXY_STRING;
        break;
    case LOCAL_CATEGORY::XPROXY:
        categoryUpdate = XPROXY_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD_XPROXY:
        categoryUpdate = NUMBER_THREAD_XPROXY_STRING;
        break;
    default:
        break;
    }

    if(categoryUpdate.isEmpty()){
        LOGD << "Category is not handle!";
        return false;
    }
    if(m_mydb.open()) {
        QSqlQuery qry(m_mydb);
        qry.prepare("UPDATE " + m_tableData + " SET data = :data WHERE category = :category;");
        qry.bindValue(":data", encodeData);
        qry.bindValue(":category", categoryUpdate);

        if(!qry.exec() ) {
            LOGD << "Query error: " << qry.lastError();
            m_mydb.close();
            return false;
        }

        LOGD << "Update " << categoryUpdate << " success!";
        m_mydb.close();
        return true;
    } else {
        LOGD << "Cannot open database!";
        return false;
    }
}

QVariant SQLiteWorker::getLocalData(LOCAL_CATEGORY category)
{
    QString data;
    QString categoryUpdate;
    switch (category) {
    case LOCAL_CATEGORY::DATABASE_VERSION:
        categoryUpdate = DATABASE_VERSION_STRING;
        break;
    case LOCAL_CATEGORY::TOKEN:
        categoryUpdate = TOKEN_STRING;
        break;
    case LOCAL_CATEGORY::PROFILE_PATH:
        categoryUpdate = PROFILE_PATH_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD:
        categoryUpdate = NUMBER_THREAD_STRING;
        break;

    case LOCAL_CATEGORY::CHECK_IP_BEFORE_RUN:
        categoryUpdate = CHECK_IP_BEFORE_RUN_STRING;
        break;
    case LOCAL_CATEGORY::DELAY_IP_AFTER_CHANGED:
        categoryUpdate = DELAY_IP_AFTER_CHANGED_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_TURN_TO_CHANGE_IP:
        categoryUpdate = DELAY_IP_AFTER_CHANGED_STRING;
        break;
    case LOCAL_CATEGORY::CHANGE_IP:
        categoryUpdate = CHANGE_IP_STRING;
        break;
    case LOCAL_CATEGORY::USE_PROXY:
        categoryUpdate = USE_PROXY_STRING;
        break;
    case LOCAL_CATEGORY::CHANGE_IP_HMA:
        categoryUpdate = CHANGE_IP_HMA_STRING;
        break;

    case LOCAL_CATEGORY::NOMMAL_SETTING_DCOM:
        categoryUpdate = NOMMAL_SETTING_DCOM_STRING;
        break;
    case LOCAL_CATEGORY::HILINK_SETTING_DCOM:
        categoryUpdate = HILINK_SETTING_DCOM_STRING;
        break;
    case LOCAL_CATEGORY::NAME_DCOM:
        categoryUpdate = NAME_DCOM_STRING;
        break;
    case LOCAL_CATEGORY::LINK_URL:
        categoryUpdate = LINK_URL_STRING;
        break;

    case LOCAL_CATEGORY::USE_API_USER:
        categoryUpdate = USE_API_USER_STRING;
        break;
    case LOCAL_CATEGORY::USE_API_PROXY:
        categoryUpdate = USE_API_PROXY_STRING;
        break;
    case LOCAL_CATEGORY::API_USER:
        categoryUpdate = API_USER_STRING;
        break;
    case LOCAL_CATEGORY::API_PROXY:
        categoryUpdate = API_PROXY_STRING;
        break;
    case LOCAL_CATEGORY::LOCATION:
        categoryUpdate = LOCATION_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD_TIN_SOFT:
        categoryUpdate = NUMBER_THREAD_TIN_SOFT_STRING;
        break;

    case LOCAL_CATEGORY::API_KEY_TM:
        categoryUpdate = API_KEY_TM_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD_TM:
        categoryUpdate = NUMBER_THREAD_TM_STRING;
        break;
    case LOCAL_CATEGORY::HTTP_TMPROXY:
        categoryUpdate = HTTP_TMPROXY_STRING;
        break;
    case LOCAL_CATEGORY::SOCKET_TMPROXY:
        categoryUpdate = SOCKET_TMPROXY_STRING;
        break;

    case LOCAL_CATEGORY::API_KEY_XPROXY_SHOPLIKE:
        categoryUpdate = API_KEY_XPROXY_SHOPLIKE_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD_XPROXY_SHOPLIKE:
        categoryUpdate = NUMBER_THREAD_XPROXY_SHOPLIKE_STRING;
        break;

    case LOCAL_CATEGORY::API_KEY_PROXYV6:
        categoryUpdate = API_KEY_PROXYV6_STRING;
        break;
    case LOCAL_CATEGORY::TYPE_HTTP_PROXYV6:
        categoryUpdate = TYPE_HTTP_PROXYV6_STRING;
        break;
    case LOCAL_CATEGORY::TYPE_SOCK5S_PROXYV6:
        categoryUpdate = TYPE_SOCK5S_PROXYV6_STRING;
        break;
    case LOCAL_CATEGORY::PROXYV6:
        categoryUpdate = PROXYV6_STRING;
        break;

    case LOCAL_CATEGORY::HTTP_XPORXY:
        categoryUpdate = HTTP_XPORXY_STRING;
        break;
    case LOCAL_CATEGORY::SOCKS_XPROXY:
        categoryUpdate = SOCKS_XPROXY_STRING;
        break;
    case LOCAL_CATEGORY::XPROXY:
        categoryUpdate = XPROXY_STRING;
        break;
    case LOCAL_CATEGORY::NUMBER_THREAD_XPROXY:
        categoryUpdate = NUMBER_THREAD_XPROXY_STRING;
        break;

    default:
        break;
    }

    if(categoryUpdate.isEmpty()){
        LOGD << (int)category << ": Category is not handle!";
        return data;
    }

    if(m_mydb.open()){
        QSqlQuery qry(m_mydb);
        qry.prepare("SELECT * FROM " + m_tableData + " WHERE category = :category;");
        qry.bindValue(":category", categoryUpdate);

        if(!qry.exec() ) {
            LOGD << (int)category << ": Query error: " << qry.lastError();
            m_mydb.close();
            return data;
        }

        while (qry.next()) {
            QVariant encodeData;
            encodeData = qry.value("data");
            // decode data
            data = decode(encodeData).toString();
        }
        m_mydb.close();
         LOGD << (int)category;
        return data;
    } else {
        LOGD << (int)category << ": Cannot open database!";
        return data;
    }
}

QList<CommonInforClone *> SQLiteWorker::getListClone()
{
    QMutexLocker locker(&mutex);
    return m_listClone;
}

bool SQLiteWorker::setListCloneHandle(QList<CommonInforClone *> listClone, bool isDeleteOldList)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    if(m_mydb.open()){
        QSqlQuery qry(m_mydb);

        // delete all old data
        qry.prepare("DELETE FROM " + m_tableAccounts);

        if(!qry.exec() ) {
            LOGD << "Query error: " << qry.lastError();
            m_mydb.close();
            return false;
        }

        if(isDeleteOldList && !m_listClone.isEmpty())
        {
            qDeleteAll(m_listClone);
            m_listClone.clear();
        }
        m_mydb.close();
    } else {
        LOGD << "Cannot open database!";
        return false;
    }

    // set new data
    addListCloneHandle(listClone);

    LOGD << "Update success!";
    return true;
}

bool SQLiteWorker::addCloneHandle(CommonInforClone* clone, bool isAddToList, bool isRefreshListClone)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    if(m_mydb.open()){
        QSqlQuery qry;
        qry.prepare("INSERT INTO " + m_tableAccounts + " (cloneId, UID, passWord, haiFA, token, cookies, status, note, appName, "
                        "checkPoint, log, userAgent, language, email, passMail, birthDay, gender, nameAcc, infoJsonClone, sttUncheckPoint) VALUES (:cloneId, :UID, "
                        ":passWord, :haiFA, :token, :cookies, :status, :note, :appName, :checkPoint, :log, :userAgent, :language, "
                        ":email, :passMail, :birthDay, :gender, :nameAcc, :infoJsonClone, :sttUncheckPoint);");

        qry.bindValue(":cloneId", encode(clone->getCloneId()));
        qry.bindValue(":UID", encode(clone->getUID()));
        qry.bindValue(":passWord", encode(clone->getPassword()));
        qry.bindValue(":haiFA", encode(clone->get2Fa()));
        qry.bindValue(":token", encode(clone->getToken()));
        qry.bindValue(":cookies", encode(clone->getCookies()));
        qry.bindValue(":status", encode(clone->getStatus()));
        qry.bindValue(":note", encode(clone->getNote()));
        qry.bindValue(":appName", encode(static_cast<int>(clone->getAppName())));
        qry.bindValue(":checkPoint", encode(clone->getCheckPoint()));
        qry.bindValue(":log", encode(clone->getLogString()));
        qry.bindValue(":userAgent", encode(clone->getUserAgent()));
        qry.bindValue(":language", encode(static_cast<int>(clone->getLanguage())));
        qry.bindValue(":email", encode(clone->getMail()));
        qry.bindValue(":passMail", encode(clone->getPassMail()));
        qry.bindValue(":birthDay", encode(clone->getBirthday()));
        qry.bindValue(":gender", encode(clone->getGender()));
        qry.bindValue(":nameAcc", encode(clone->getNameAcc()));
        qry.bindValue(":infoJsonClone", encode(clone->getInfoJsonClone()));
        qry.bindValue(":sttUncheckPoint", encode(static_cast<int>(clone->getSttUncheckPoint())));

        LOGD << "kcheck: " << qry.boundValues();
        if( !qry.exec() ) {
            LOGD << "Clone " << clone->getCloneId() << " insert error: " << qry.lastError();
            m_mydb.close();
            return false;
        } else {
            LOGD << "Clone " << clone->getCloneId() << " inserted!";
            if(isRefreshListClone){
                updateListCloneFromDataBase();
            } else if(isAddToList) {
                m_listClone.append(clone);
            }
            m_mydb.close();
            CONTROLLERMAIN->uiUpdateStatusCloneToStatusBar();
            CONTROLLERMAIN->sigOnchangeReloadTable(clone->getCloneId());
            return true;
        }
    }
    LOGD << "Cannot open database to add clone cloneId = " << clone->getCloneId() << " !";
    return false;
}

bool SQLiteWorker::addListCloneHandle(QList<CommonInforClone *> listClone, bool isAddToList, bool isRefreshListClone)
{
    bool isAddedAll = true;
    for(CommonInforClone* clone: listClone) {
        if(!addCloneHandle(clone, isAddToList, isRefreshListClone)){
            isAddedAll = false;
            delete clone;
        }
    }
    LOGD << "All clone in list added: " << isAddedAll;
    return isAddedAll;
}

bool SQLiteWorker::updateListCloneFromDataBase()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    if(m_mydb.open()){
        QSqlQuery qry(m_mydb);
        qry.prepare("select * from " + m_tableAccounts);

        if(!qry.exec() ) {
            LOGD << "Query error: " << qry.lastError();
            m_mydb.close();
            return false;
        }

        if(!m_listClone.isEmpty())
        {
            qDeleteAll(m_listClone);
            m_listClone.clear();
        }

        while (qry.next()) {
            m_listClone.append(new CommonInforClone(decode(qry.value("cloneId")).toString(), decode(qry.value("UID")).toString(),
                                                    decode(qry.value("passWord")).toString(), decode(qry.value("haiFA")).toString(),
                                                    decode(qry.value("token")).toString(), decode(qry.value("cookies")).toString(),
                                                    decode(qry.value("status")).toString(), decode(qry.value("note")).toString(),
                                                    static_cast<SET_APP_NAME>(decode(qry.value("appName")).toInt()),
                                                    decode(qry.value("checkPoint")).toBool(), decode(qry.value("log")).toString().split(LOG_SPLIT_STRING, Qt::SkipEmptyParts),
                                                    decode(qry.value("userAgent")).toString(), static_cast<LANGUAGE>(decode(qry.value("language")).toInt()),
                                                    decode(qry.value("email")).toString(), decode(qry.value("passMail")).toString(),
                                                    decode(qry.value("birthDay")).toString(), decode(qry.value("gender")).toString(),
                                                    decode(qry.value("nameAcc")).toString(), decode(qry.value("infoJsonClone")).toString(),
                                                    static_cast<STATUS_UNCHECKPOINT>(decode(qry.value("sttUncheckPoint")).toInt())
                                                    ));
        }
        m_mydb.close();
        LOGD << "Update success!";
        return true;
    } else {
        LOGD << "Cannot open database!";
        return false;
    }
}

bool SQLiteWorker::updateCurrentDataToDataBase()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    if(m_mydb.open()) {
        foreach (CommonInforClone* clone, m_listClone) {
            QSqlQuery qry(m_mydb);
            qry.prepare("UPDATE " + m_tableAccounts + " SET passWord = :passWord, haiFA = :haiFA, status = :status, log = :log WHERE cloneId = :cloneId;");
            qry.bindValue(":cloneId", encode(clone->getCloneId()));
            qry.bindValue(":passWord", encode(clone->getPassword()));
            qry.bindValue(":haiFA", encode(clone->get2Fa()));
            qry.bindValue(":status", encode(clone->getStatus()));
            qry.bindValue(":log", encode(clone->getLogString()));

            if(!qry.exec() ) {
                LOGD << "Query error: " << qry.lastError();
                m_mydb.close();
                return false;
            }
        }
        LOGD << "Update success!";
        m_mydb.close();
        return true;
    } else {
        LOGD << "Cannot open database!";
        return false;
    }
}

bool SQLiteWorker::updateDataBase()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    if(m_mydb.open()) {
        QSqlQuery qry(m_mydb);

        // delete all old data in database
        qry.prepare("DELETE FROM " + m_tableAccounts);

        if(!qry.exec() ) {
            LOGD << "Query error: " << qry.lastError();
            m_mydb.close();
            return false;
        }
        m_mydb.close();
        bool isAddedAll = true;
        for(CommonInforClone* clone: m_listClone) {
            if(!addCloneHandle(clone, false)){
                isAddedAll = false;
            }
        }
        LOGD << "All clone in list updated: " << isAddedAll;
        return isAddedAll;
    } else {
        LOGD << "Cannot open database!";
        return false;
    }
}

bool SQLiteWorker::updateCloneByIdAllData(QString cloneId)
{
    LOGD;
    foreach (CommonInforClone* clone, m_listClone) {
        if(cloneId == clone->getCloneId()) {
            if(m_mydb.open()) {
                QSqlQuery qry(m_mydb);
                qry.prepare("UPDATE " + m_tableAccounts + " SET passWord = :passWord, haiFA = :haiFA, status = :status, log = :log WHERE cloneId = :cloneId;");
                qry.bindValue(":cloneId", encode(cloneId));
                qry.bindValue(":passWord", encode(clone->getPassword()));
                qry.bindValue(":haiFA", encode(clone->get2Fa()));
                qry.bindValue(":status", encode(clone->getStatus()));
                qry.bindValue(":log", encode(clone->getLogString()));

                if(!qry.exec() ) {
                    LOGD << "Query error: " << qry.lastError();
                    m_mydb.close();
                    return false;
                }

                LOGD << "Update cloneId " << cloneId << " success!";
                m_mydb.close();
                return true;
            } else {
                LOGD << "Cannot open database!";
                return false;
            }
        }
    }
    LOGD << "cloneId " << cloneId << " doesnot exist in listClone!";
    m_mydb.close();
    return false;
}

bool SQLiteWorker::updateCloneByIdHandle(QString cloneId, DATABASE_CATEGORY category)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    LOGD << "cloneId = " << cloneId << ", category = " << (int)category;
    foreach (CommonInforClone* clone, m_listClone) {
        if(cloneId == clone->getCloneId()) {
            QString categoryUpdate = "";
            QVariant categoryData;
            switch (category) {
            case DATABASE_CATEGORY::PASSWORD:
                categoryUpdate = "passWord";
                categoryData = clone->getPassword();
                break;
            case DATABASE_CATEGORY::HAIFA:
                categoryUpdate = "haiFA";
                categoryData = clone->get2Fa();
                break;
            case DATABASE_CATEGORY::TOKEN:
                categoryUpdate = "token";
                categoryData = clone->getToken();
                break;
            case DATABASE_CATEGORY::COOKIES:
                categoryUpdate = "cookies";
                categoryData = clone->getCookies();
                break;
            case DATABASE_CATEGORY::STATUS:
                categoryUpdate = "status";
                categoryData = clone->getStatus();
                break;
            case DATABASE_CATEGORY::NOTE:
                categoryUpdate = "status";
                categoryData = clone->getNote();
                break;
            case DATABASE_CATEGORY::APP_NAME:
                categoryUpdate = "appName";
                categoryData = static_cast<int>(clone->getAppName());
                break;
            case DATABASE_CATEGORY::CHECKPOINT:
                categoryUpdate = "checkPoint";
                categoryData = clone->getCheckPoint();
                break;
            case DATABASE_CATEGORY::LOG:
                categoryUpdate = "log";
                categoryData = clone->getLogString();
                break;
            case DATABASE_CATEGORY::USER_AGENT:
                categoryUpdate = "userAgent";
                categoryData = clone->getUserAgent();
                break;
            case DATABASE_CATEGORY::LANGUAGE:
                categoryUpdate = "language";
                categoryData = static_cast<int>(clone->getLanguage());
                break;
            case DATABASE_CATEGORY::EMAIL:
                categoryUpdate = "email";
                categoryData = clone->getMail();
                break;
            case DATABASE_CATEGORY::PASS_MAIL:
                categoryUpdate = "passMail";
                categoryData = clone->getMail();
                break;
            case DATABASE_CATEGORY::BIRTHDAY:
                categoryUpdate = "birthDay";
                categoryData = clone->getBirthday();
                break;
            case DATABASE_CATEGORY::GENDER:
                categoryUpdate = "gender";
                categoryData = clone->getGender();
                break;
            case DATABASE_CATEGORY::NAME_ACC:
                categoryUpdate = "nameAcc";
                categoryData = clone->getNameAcc();
                break;
            case DATABASE_CATEGORY::INFO_JSON_CLONE:
                categoryUpdate = "infoJsonClone";
                categoryData = clone->getInfoJsonClone();
                break;
            case DATABASE_CATEGORY::STT_UNCHECKPOINT:
                categoryUpdate = "sttUncheckPoint";
                categoryData = static_cast<int>(clone->getSttUncheckPoint());
                break;
            default:
                break;
            }

//            LOGD << "data: " << categoryData;

            // encode data
            QByteArray categoryDataEncode = encode(categoryData);

            if(categoryUpdate == ""){
                LOGD << "Category is not handle!";
                return false;
            }
            if(m_mydb.open()) {
                QSqlQuery qry(m_mydb);
                qry.prepare("UPDATE " + m_tableAccounts + " SET " + categoryUpdate + " = :data WHERE cloneId = :cloneId;");
                qry.bindValue(":cloneId", encode(cloneId));
                qry.bindValue(":data", categoryDataEncode);

                if(!qry.exec() ) {
                    LOGD << "Query error: " << qry.lastError();
                    m_mydb.close();
                    return false;
                }

                LOGD << "Update cloneId " << cloneId << " success!";
                m_mydb.close();
                return true;
            } else {
                LOGD << "Cannot open database!";
                return false;
            }
        }
    }
    LOGD << "cloneId " << cloneId << " doesnot exist in listClone!";
    m_mydb.close();
    return false;
}

bool SQLiteWorker::deleteCloneByIdHandle(QString cloneId)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    if(m_mydb.open()) {
        QSqlQuery qry(m_mydb);

        // delete all old data in database
        qry.prepare("DELETE FROM " + m_tableAccounts + " WHERE cloneId = :cloneId;");
        qry.bindValue(":cloneId", encode(cloneId));

        if(!qry.exec() ) {
            LOGD << "Query delete error: " << qry.lastError();
            m_mydb.close();
            return false;
        }
        LOGD << "Delete clone " << cloneId << " from DB success!";
        bool isInList = false;
        for(int i=0; i < m_listClone.size(); i++) {
            if(m_listClone[i]->getCloneId() == cloneId){
                isInList = true;
                m_listClone.removeAt(i);
                LOGD << "Delete clone " << cloneId << " from local list success!";
                CONTROLLERMAIN->uiUpdateStatusCloneToStatusBar();
                CONTROLLERMAIN->sigOnchangeReloadTable(cloneId);
                break;
            }
        }
        if(!isInList){
            LOGD << "Clone " << cloneId << " not in local list";
        }

        CONTROLLERMAIN->reloadTable();
        return true;
    } else {
        LOGD << "Cannot open database!";
        return false;
    }
}

//bool SQLiteWorker::updatePassword(QString uid, QString pass)
//{
//    if(m_mydb.open()) {
//        foreach (CommonInforClone* clone, m_listClone) {
//            if(uid == clone->getUID()) {
//                QSqlQuery qry(m_mydb);
//                qry.prepare("UPDATE " + m_tableName + " SET passWord = :passWord  WHERE UID = :UID;");
//                qry.bindValue(":UID", uid);
//                qry.bindValue(":passWord", pass);

//                if(!qry.exec() ) {
//                    LOGD << "Query error: " << qry.lastError();
//                    m_mydb.close();
//                    return false;
//                }
//                clone->setPassWord(pass);
//                LOGD << "Update UID " << uid << " success!";
//                m_mydb.close();
//                return true;
//            }
//        }
//        LOGD << "UID " << uid << " doesnot exist in listClone!";
//        m_mydb.close();
//        return false;
//    } else {
//        LOGD << "Cannot open database!";
//        return false;
//    }
//}

bool SQLiteWorker::checkCloneExist(QString cloneId)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    bool isExist = false;
    foreach (CommonInforClone* clone, m_listClone) {
        if(cloneId == clone->getCloneId()) {
            isExist = true;
            break;
        }
    }
    LOGD << "cloneId " << cloneId << " exist: " << isExist;
    return isExist;
}

void SQLiteWorker::checkDataBaseVersion()
{
    QString oldDbVer = getLocalData(LOCAL_CATEGORY::DATABASE_VERSION).toString();
    if(oldDbVer.isEmpty()) oldDbVer = "0.00.00";
    QString newDbVer = DATABASE_VER;
    if(checkVersionFormat(oldDbVer) && checkVersionFormat(newDbVer)) {
        if(compareVersion(oldDbVer, newDbVer) < 0) {
            LOGD << "Old database version => update database";
            QStringList listTables;
            if(m_mydb.open()) {
                listTables = m_mydb.tables();
                m_mydb.close();
            } else {
                LOGD << "Cannot open database!";
                return;
            }

            bool isOk = false;
            foreach(QString table, listTables){
                // handle table account
                if(table.contains(TABLE_NAME_ACCOUNTS)){
                    LOGD << "Save table: " << table;

                    // backup data
                    m_tableAccounts = table;
                    isOk = updateListCloneFromDataBase();

                    // delete old table
                    if(!isOk) break;
                    isOk = queryDatabase("DROP TABLE if EXISTS " + m_tableAccounts);

                    // create new table
                    if(!isOk) break;
                    isOk = createAccountTable();

                    // restore data
                    if(!isOk) break;
                    isOk = addListCloneHandle(m_listClone, false);
                }
            }
            LOGD << "Save data: " << isOk;
            if(isOk) {
                if(setLocalData(LOCAL_CATEGORY::DATABASE_VERSION, DATABASE_VER)){
                    LOGD << "Update sucess to Database version: " << DATABASE_VER;
                    m_localData[LOCAL_CATEGORY::DATABASE_VERSION] = DATABASE_VER;
                    return;
                }
            }
        } else {
            LOGD << "Already Newest database version";
        }
    } else {
        LOGD << "Wrong format!";
    }
}
