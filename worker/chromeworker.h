#ifndef CHROMEWORKER_H
#define CHROMEWORKER_H

#include <QObject>
#include "baseworker.h"
#include <QProcess>
#include <time.h>
#include <QJsonDocument>
#include <QJsonObject>
//#include "fity.one-api/fcare/include/WebAPI.hpp"
#include "autoFarmerAPI/include/WebAPI.hpp"
#include "sqliteworker.h"
#include "networkaccesssupporter.h"
#include "PhoneNumberAPI.h"
#include "captchaapi.h"
#include "followpageapi.h"

#define LINKFACEBOOK "https://m.facebook.com"

class ChromeWorker : public BaseWorker
{
    Q_OBJECT
public:
    explicit ChromeWorker(QObject *parent = nullptr);
    ~ChromeWorker();

    enum ACTION : int {
        ACTION_LOGIN = 0,
        ACTION_SCROLL_FEED,
        ACCTION_SCROLL_FEED_LIKE,
        ACCTION_NEWFEED_BUTTON,
        ACTION_VIDEO_BUTTON,
        ACCTION_FRIEND_BUTTON,
        ACCTION_CONFIRM_FRIEND,
        ACCTION_MESSAGE_BUTTON,
        ACCTION_JOIN_GROUP,
        ACCTION_FOLLOW_PROFILE,
        ACCTION_LIKE_POST_GROUP,
        ACCTION_LIKE_POST_FRIEND,
        ACCTION_UPLOAD_PROFILE,
        ACCTION_UPLOAD_COVER,
        ACCTION_ADDFRIEN_SUGGESTION,
        ACCTION_ADDFRIEND_BY_UID,
        ACCTION_COMEND,
        ACCTION_CHANGE_PASSWORD,
        ACCTION_SHARE_NOW,
        ACTION_MAX
    };

    enum BY : int {
        BY_NAME = 0,
        BY_CLASS,
        BY_XPATH,
        BY_LINK_TEXT,
        BY_ID
    };

    void forceCloseChrome();

public slots:
    void onStartFDriver(QString UID) override;
    void slotGetId2Captcha(QNetworkReply *reply);
    void slotGetTocken2Captcha(QNetworkReply *reply);
private:
    void unlockCaptcha();
    bool checkNoInternet();
    void uncheckpointByPhone();
    void getPhoneNumber();
    void getOPTfromPhone();
    void updateAvartaToUncheckPoint();
    void doAcctionResult(QString acction);
    void getRequest2Captcha(QString idCaptcha);
    bool waitUncheckPointSuccess();
    QMap<QString, int> getAcction(QString listAction);
    QMap<QString, int> initMapAcction();
    void updateStatusCloneToServer(QString idClone,QString keyStatus, QString status, QString action);
    bool checkCheckPoint();
    bool checkPoinLogin();
    void updateCheckPointToServer();
    void updateLanguageToServer();
    bool Navigate(QString url, QString uid, QString pass, std::string haifa, QString cookie, int row);
    void navigateAndFollowProfile(QString UID);
    void navigateAndLikeGroup(QString url);
    void settingLanguage();
    void settingLanguageVietNam();
    void nextButton();
    void loginAgain();
    bool rejectUncheckPoint();
    std::string getOPTFromMailDomain(QString mailDomain);
    QString getEmail();
    QString getNameFolder();
    void followPageByAPI();

    Element FindElementBy(By by);
    void SendKeys(QString key);
    void checkUncheckPointSusscess();
   // void Submit(Element& element);

    QString GetScreenShot();
    void friendRequestButton(bool check);
    void messagesButton(bool check);
    void videoButton(bool check, int num);
    void notificationButton(bool check);
    void moreButton(bool check);
    void newFeedButton(bool check);
    void actionScrollDownNewFeed(bool check, int num);
    void scrollDown(int num);
    void actionScrollDownFeedLike(bool check, int likeNumber);
    void ActionConfirmFriend(bool check, int num);
    void actionScrollDown(int num);
    void actionScrollUp(int num);
    void addFrendSuggetion(bool check, int num);
    void addFriendByUID(bool check, QString UID);
    void checkNewClone(bool check);
    void shareNowPost(bool check, QString urlPost);
    void commentPost(bool check, QString text);
    void likePage(QString UID);
    void likePost(QString UID);
    void watchStory();
    void changePassword(bool check, QString newPassword);
    void getInfoDataClone();

    void uploadProfile(bool check);
    void uploadCover(bool check);
    void requestReviewButton();

    Session gotoGroup(QString url);
    void randomLikePostInGroup(bool check, QString url);

    Session gotoFriend(QString url);
    void randomLikePostFriend(bool check, QString url);

    void actionRunInEachClone();
    void actionAutofarmer();
    void inputTextManual(std::string element, QString textInput, BY type);

    int randomvalue(int maxRan) {
        srand(time(NULL));
        int res = rand() % maxRan + 1;
        int ranSlep = rand() % 200 + 100;
        Sleep(ranSlep);
        return res;
    }

    int randomSleep(int min, int max) {
        int randomValue = qrand() % max + min;
        return randomValue;
    }

    QString pathUploadProfile(QString url);

    void finishRunning();
    bool getCloneToServer();

    bool closeSession(int tryNumber = 3);

private slots:
    void cookies(QString cookie);
signals:
    void testConnect();
    void runCloneFinished(QString cloneId, int serviceID);
    void updateStatus(QString UID, QString status);
    void reloadTable();
private:
    bool m_exisUID;
    QString m_linkImage;
    bool m_isCheckPoint = false;
    QMap<QString, int> m_mapAction;
    QMap<QString, int> m_mapActionWithEnum;
    CloneInfo *m_cloneRun;
//    NetworkAccessSupporter *m_networkAccessSup = nullptr;
    QString m_idCaptCha = "";
    QString m_phoneNumber = "";
    PhoneNumberAPI phoneAPI;
    captChaAPI m_captChaAPI;
    FollowPageAPI m_followPageAPI;
    bool m_optAvailabe = false;
    bool m_PhoneNumberAvailable = false;
    bool m_wait24H = false;
    bool m_stopGetPhone = false;
};

#endif // CHROMEWORKER_H
