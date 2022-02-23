#include "cloneinfo.h"
#include "log.h"
//#include "controllermainthread.h"

CloneInfo::CloneInfo(QString cloneId, QString UID, QString passWord, QString haiFA, QString token, QString cookies,  QString status,
                                   QString note, SET_APP_NAME appName, bool isCheckPoint, QStringList listLog, QString userAgent, LANGUAGE language,
                                   QString email, QString passMail, QString birthDay, QString gender, QString nameAcc, QString infoJsonClone, STATUS_UNCHECKPOINT sttUncheckPoint):
    m_cloneId(cloneId),
    m_UID(UID),
    m_passWord(passWord),
    m_2FA(haiFA),
    m_token(token),
    m_cookies(cookies),
    m_status(status),
    m_note(note),
    m_appName(appName),
    m_isCheckPoint(isCheckPoint),
    m_log(listLog),
    m_userAgent(userAgent),
    m_language(language),
    m_email(email),
    m_passMail(passMail),
    m_birthday(birthDay),
    m_gender(gender),
    m_nameAcc(nameAcc),
    m_infoJsonClone(infoJsonClone),
    m_sttCheckPoint(sttUncheckPoint)
{
//    connect(this, &CloneInfo::updateClone, SQLiteWorker::getInstance(), &SQLiteWorker::updateCloneById, Qt::QueuedConnection);
//    connect(this, &CloneInfo::checkpointChanged, CONTROLLERMAIN, &ControllerMainThread::uiUpdateStatusCloneToStatusBar, Qt::QueuedConnection);
}

//void CloneInfo::setCloneId(QString cloneId)
//{
//    mutex.lock();
//    if(m_cloneId != cloneId) {
//        m_cloneId = cloneId;
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getCloneId()
//{
//    mutex.lock();
//    QString cloneId = m_cloneId;
//    mutex.unlock();
//    return cloneId;
//}

//void CloneInfo::setUID(QString uid) {
//    mutex.lock();
//    if(m_UID != uid) {
//        m_UID = uid;
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getUID() {
//    mutex.lock();
//    QString uid = m_UID;
//    mutex.unlock();
//    return uid;
//}

//void CloneInfo::setPassWord(QString passWord) {
//    mutex.lock();
//    if(m_passWord != passWord) {
//        m_passWord = passWord;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::PASSWORD);
//        CONTROLLERMAIN->sigOnchangeReloadTable(m_cloneId);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getPassword() {
//    mutex.lock();
//    QString password;
//    password = m_passWord;
//    mutex.unlock();
//    return password;
//}

//void CloneInfo::set2Fa(QString haiFa) {
//    mutex.lock();
//    if(m_2FA != haiFa) {
//        m_2FA = haiFa;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::HAIFA);
//        CONTROLLERMAIN->sigOnchangeReloadTable(m_cloneId);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::get2Fa() {
//    mutex.lock();
//    QString haiFA = m_2FA;
//    mutex.unlock();
//    return haiFA;
//}

//void CloneInfo::setToken(QString tocken) {
//    mutex.lock();
//    if(m_token != tocken) {
//        m_token = tocken;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::TOKEN);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getToken() {
//    mutex.lock();
//    QString token = m_token;
//    mutex.unlock();
//    return token;
//}

//void CloneInfo::setCookies(QString cookies) {
//    mutex.lock();
//    if(m_cookies != cookies) {
//        m_cookies = cookies;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::COOKIES);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getCookies() {
//    mutex.lock();
//    QString cookies = m_cookies;
//    mutex.unlock();
//    return cookies;
//}

//void CloneInfo::setStatus(QString status) {
//    mutex.lock();
//    if(m_status != status) {
//        m_status = status;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::STATUS);
//        CONTROLLERMAIN->sigOnchangeReloadTable(m_cloneId);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getStatus() {
//    mutex.lock();
//    QString status = m_status;
//    mutex.unlock();
//    return status;
//}

//void CloneInfo::setNote(QString note) {
//    mutex.lock();
//    if(m_note != note) {
//        m_note = note;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::NOTE);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getNote() {
//    mutex.lock();
//    QString note = m_note;
//    mutex.unlock();
//    return note;
//}

//void CloneInfo::setAppName(SET_APP_NAME appName) {
//    mutex.lock();
//    if(m_appName != appName) {
//        m_appName = appName;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::APP_NAME);
//    }
//    mutex.unlock();
//}

//SET_APP_NAME CloneInfo::getAppName() {
//    mutex.lock();
//    SET_APP_NAME appName = m_appName;
//    mutex.unlock();
//    return appName;
//}

//void CloneInfo::setCheckPoint(bool checkPoint) {
//    mutex.lock();
//    if(m_isCheckPoint != checkPoint) {
//        m_isCheckPoint = checkPoint;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::CHECKPOINT);
//        emit checkpointChanged();
//    }
//    mutex.unlock();
//}

//bool CloneInfo::getCheckPoint() {
//    mutex.lock();
//    bool isCheckPoint = m_isCheckPoint;
//    mutex.unlock();
//    return isCheckPoint;
//}

//void CloneInfo::setDoAction(QString lisDoAction)
//{
//    mutex.lock();
//    if(m_listDoAction != lisDoAction) {
//        m_listDoAction = lisDoAction;
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getDoAction()
//{
//    mutex.lock();
//    QString listDoAction = m_listDoAction;
//    mutex.unlock();
//    return listDoAction;
//}

//void CloneInfo::setLog(QStringList log)
//{
//    mutex.lock();
//    if(m_log != log) {
//        m_log = log;
//        emit logChanged(m_log);
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::LOG);
//        CONTROLLERMAIN->sigOnchangeReloadTable(m_cloneId);
//    }
//    mutex.unlock();
//}

//QStringList CloneInfo::getLog()
//{
//    mutex.lock();
//    QStringList log = m_log;
//    mutex.unlock();
//    return log;
//}

//void CloneInfo::setLogString(QString log)
//{
//    setLog(log.split(LOG_SPLIT_STRING, Qt::SkipEmptyParts));
//}

//QString CloneInfo::getLogString()
//{
//    QString log;
//    if(!getLog().isEmpty()){
//        log = getLog().join(LOG_SPLIT_STRING);
//    }
//    return log;
//}

//QString CloneInfo::getLastLogWithoutDate()
//{
//    QString logData;
//    QStringList log = getLastLog().split(":");
//    if(log.size() > 3) {
//        logData = log[3];
//    }
//    return logData;
//}

//void CloneInfo::addLog(QString log)
//{
//    mutex.lock();
//    QDateTime date;
//    m_log << (date.currentDateTime().toString() + ": " + log);
//    emit logChanged(m_log);
//    SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::LOG);
//    CONTROLLERMAIN->sigOnchangeReloadTable(m_cloneId);
//    mutex.unlock();
//}

//QString CloneInfo::getLastLog()
//{
//    QString log;
//    QStringList listLog = getLog();
//    if(!listLog.isEmpty()){
//        log = listLog.last();
//    }
//    return log;
//}

//QString CloneInfo::getUserAgent()
//{
//    mutex.lock();
//    QString userAgent = m_userAgent;
//    mutex.unlock();
//    return userAgent;
//}

//void CloneInfo::setUserAgent(QString userAgent)
//{
//    mutex.lock();
//    if(m_userAgent != userAgent) {
//        m_userAgent = userAgent;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::USER_AGENT);
//    }
//    mutex.unlock();
//}

//void CloneInfo::setLanguage(LANGUAGE language)
//{
//    mutex.lock();
//    if(m_language != language) {
//        m_language = language;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::LANGUAGE);
//    }
//    mutex.unlock();
//}

//LANGUAGE CloneInfo::getLanguage()
//{
//    mutex.lock();
//    LANGUAGE language = m_language;
//    mutex.unlock();
//    return language;
//}

//void CloneInfo::setSttUncheckPoint(STATUS_UNCHECKPOINT stt)
//{
//    mutex.lock();
//    if(m_sttCheckPoint != stt) {
//        m_sttCheckPoint = stt;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::STT_UNCHECKPOINT);
//    }
//    mutex.unlock();
//}

//STATUS_UNCHECKPOINT CloneInfo::getSttUncheckPoint()
//{
//    mutex.lock();
//    STATUS_UNCHECKPOINT stt = m_sttCheckPoint;
//    mutex.unlock();
//    return stt;
//}

//void CloneInfo::setProxy(QString proxy)
//{
//    mutex.lock();
//    if(m_proxy != proxy) {
//        m_proxy = proxy;
//        CONTROLLERMAIN->sigOnchangeReloadTable(m_cloneId);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getProxy()
//{
//    mutex.lock();
//    QString proxy = m_proxy;
//    mutex.unlock();
//    return proxy;
//}

//void CloneInfo::setMail(QString email)
//{
//    mutex.lock();
//    if(m_email != email) {
//        m_email = email;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::EMAIL);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getMail()
//{
//    mutex.lock();
//    QString email = m_email;
//    mutex.unlock();
//    return email;
//}

//void CloneInfo::setPassMail(QString passMail)
//{
//    mutex.lock();
//    if(m_passMail != passMail) {
//        m_passMail = passMail;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::PASS_MAIL);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getPassMail()
//{
//    mutex.lock();
//    QString pasMail = m_passMail;
//    mutex.unlock();
//    return pasMail;
//}

//void CloneInfo::setBirthday(QString birthday)
//{
//    mutex.lock();
//    if(m_birthday != birthday) {
//        m_birthday = birthday;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::BIRTHDAY);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getBirthday()
//{
//    mutex.lock();
//    QString birthday = m_birthday;
//    mutex.unlock();
//    return birthday;
//}

//void CloneInfo::setGender(QString gender)
//{
//    mutex.lock();
//    if(m_gender != gender) {
//        m_gender = gender;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::GENDER);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getGender()
//{
//    mutex.lock();
//    QString gender = m_gender;
//    mutex.unlock();
//    return gender;
//}

//void CloneInfo::setNameAcc(QString name)
//{
//    mutex.lock();
//    if(m_nameAcc != name) {
//        m_nameAcc = name;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::NAME_ACC);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getNameAcc()
//{
//    mutex.lock();
//    QString nameAcc = m_nameAcc;
//    mutex.unlock();
//    return nameAcc;
//}

//void CloneInfo::setInfoJsonClone(QString jsonInfo)
//{
//    mutex.lock();
//    if(m_infoJsonClone != jsonInfo) {
//        m_infoJsonClone = jsonInfo;
//        SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::INFO_JSON_CLONE);
//    }
//    mutex.unlock();
//}

//QString CloneInfo::getInfoJsonClone()
//{
//    mutex.lock();
//    QString jsonInfoClone = m_infoJsonClone;
//    mutex.unlock();
//    return jsonInfoClone;
//}

//void CloneInfo::checkLogToClear_bk(int days)
//{
//    mutex.lock();
//    QList<int> removeIndex;
//    for(int i = m_log.size()-1; i>=0; i--) {
//        QString date = m_log[i];
//        date.truncate(m_log[i].lastIndexOf(":"));
//        QDateTime logTime = QDateTime::fromString(date);
//        if(logTime < QDateTime::currentDateTime().addDays(-days)) {
//            removeIndex.append(i);
//        }
//    }
//    foreach(int index, removeIndex) {
//        m_log.removeAt(index);
//    }
//    SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::LOG);
//    mutex.unlock();
//}

//void CloneInfo::checkLogToClear()
//{
//    mutex.lock();
//    if(!m_log.isEmpty()) {
//        QString date = m_log.last();
//        date.truncate(date.lastIndexOf(":"));
//        QDateTime logTime = QDateTime::fromString(date);
//        if(logTime.isValid() && logTime.date() < QDate::currentDate()) {
//            LOGD << "check";
//            m_log.clear();
//            LOGD << m_log;
//            SQLiteWorker::getInstance()->updateCloneById(m_cloneId, DATABASE_CATEGORY::LOG);
//        }
//    }
//    mutex.unlock();
//}
