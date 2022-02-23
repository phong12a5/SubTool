#ifndef COMMONINFORCLONE_H
#define COMMONINFORCLONE_H
#include <QObject>
#include "sqliteworker.h"

#define LOG_SPLIT_STRING "|||"

enum class APP_NAME: int {
  TYPE_FACEBOOK = 0,
  TYPE_INSTAGRAM,
  TYPE_YOUTUBE
};
typedef APP_NAME SET_APP_NAME;

enum class LANGUAGE: int {
    NONE,
    ENGLISH,
    VIET_NAM
};

enum class STATUS_UNCHECKPOINT: int {
    NOMAL,
    LIVE,
    WAITING,
    SUCCESS,
    UNSUCCESS
};


class CommonInforClone : public QObject
{
    Q_OBJECT
public:
    ~CommonInforClone(){}
    explicit CommonInforClone(QString cloneId, QString UID = "", QString passWord = "", QString haiFA = "", QString token = "", QString cookies = "",
                     QString status = "", QString note = "", SET_APP_NAME appName = SET_APP_NAME::TYPE_FACEBOOK, bool isCheckPoint = false,
                     QStringList listLog = QStringList(), QString userAgent = "", LANGUAGE language = LANGUAGE::NONE, QString email = "",
                     QString passMail = "", QString birthDay = "", QString gender = "", QString nameAcc = "", QString infoJsonClone = "", STATUS_UNCHECKPOINT sttUncheckPoint = STATUS_UNCHECKPOINT::NOMAL);


    void setCloneId(QString cloneId);
    QString getCloneId();

    void setUID(QString uid);
    QString getUID();

    void setPassWord(QString passWord);
    QString getPassword();

    void set2Fa(QString haiFa);
    QString get2Fa();

    void setToken(QString tocken);
    QString getToken();

    void setCookies(QString cookies);
    QString getCookies();

    void setStatus(QString status);
    QString getStatus();

    void setNote(QString note);
    QString getNote();

    void setAppName(SET_APP_NAME appName);
    SET_APP_NAME getAppName();

    void setCheckPoint(bool checkPoint);
    bool getCheckPoint();

    void setDoAction(QString lisDoAction);
    QString getDoAction();

    void setLog(QStringList log);
    QStringList getLog();
    void setLogString(QString log);
    QString getLogString();
    QString getLastLogWithoutDate();
    void addLog(QString log);
    QString getLastLog();

    QString getUserAgent();
    void setUserAgent(QString userAgent);

    void setLanguage(LANGUAGE language);
    LANGUAGE getLanguage();

    void setSttUncheckPoint(STATUS_UNCHECKPOINT stt);
    STATUS_UNCHECKPOINT getSttUncheckPoint();

    void setProxy(QString proxy);
    QString getProxy();

    void setMail(QString email);
    QString getMail();
    void setPassMail(QString passMail);
    QString getPassMail();
    void setBirthday(QString birthday);
    QString getBirthday();
    void setGender(QString gender);
    QString getGender();
    void setNameAcc(QString name);
    QString getNameAcc();
    void setInfoJsonClone(QString jsonInfo);
    QString getInfoJsonClone();

    void checkLogToClear();

signals:
    void updateClone(QString cloneId, DATABASE_CATEGORY category);
    void checkpointChanged();

    void logChanged(QStringList log);

private:
    void checkLogToClear_bk(int days);

private:
    QString m_cloneId;
    QString m_UID;
    QString m_passWord;
    QString m_2FA;
    QString m_token;
    QString m_cookies;
    QString m_status;
    QString m_note;
    SET_APP_NAME m_appName;
    bool m_isCheckPoint;
    QStringList m_log;
    QString m_listDoAction;
    QString m_userAgent;
    LANGUAGE m_language;
    QString m_proxy;
    QString m_email;
    QString m_passMail;
    QString m_birthday;
    QString m_gender;
    QString m_nameAcc;
    QString m_infoJsonClone;

    QMutex mutex;
    STATUS_UNCHECKPOINT m_sttCheckPoint;
};

#endif // COMMONINFORCLONE_H
