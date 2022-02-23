#include "chromeworker.h"
#include <QProcess>
#include <QThread>
#include <QDir>
#include "servicedata.h"
#include "DefineString.h"
#include "servicemanager.h"
#include "QMutex"
#include <QDir>
#include "followpageapi.h"

ChromeWorker::ChromeWorker(QObject *parent)
{
    LOGD << "--------- ChromeWorker ---------";
    initMapAcction();
    m_exisUID = false;
//    QObject::connect(this, &ChromeWorker::runCloneFinished, ServiceManager::instance, &ServiceManager::onWorkerFinished, Qt::QueuedConnection);
}

ChromeWorker::~ChromeWorker()
{
    if(nullptr != driver) {
        delete driver;
    }
    LOGD << "ChromeWorker::~ChromeWorker";
}

inline bool u_sleep(int msecs)
{
    QTime dieTime = QTime::currentTime().addMSecs(msecs);
    while( QTime::currentTime() < dieTime )
    {

    }
    return true;
}

QMap<QString, int> ChromeWorker::getAcction(QString listAction)
{
    QJsonDocument listJsonAction = QJsonDocument::fromJson(listAction.toUtf8());
    QJsonObject getDataAction = listJsonAction.object();
    QJsonValue dataJson = getDataAction.value(QString("data"));
    int leng = dataJson.toArray().size();
    for (int i = 0; i < leng; i++) {
        QJsonValue action = getDataAction.value(QString("data"))[i].operator[]("action");
        QJsonValue count = getDataAction.value(QString("data"))[i].operator[]("count");
        LOGD << "action: " << action.toString() << " | " << "count: " << count.toInt();
        m_mapAction.insert(action.toString(), count.toInt());
    }
    LOGD << m_mapAction.size();
    return m_mapAction;
}

QMap<QString, int> ChromeWorker::initMapAcction()
{
    m_mapActionWithEnum["Feed"] = ACTION_SCROLL_FEED;
    m_mapActionWithEnum["FeedLike"] = ACCTION_SCROLL_FEED_LIKE;
    m_mapActionWithEnum["Watch"] = ACTION_VIDEO_BUTTON;
    m_mapActionWithEnum["1"] = ACCTION_FRIEND_BUTTON;
    m_mapActionWithEnum["ConfirmFriend"]   = ACCTION_CONFIRM_FRIEND;
    m_mapActionWithEnum["2"] = ACCTION_MESSAGE_BUTTON;
    m_mapActionWithEnum["PageLike"] = ACCTION_JOIN_GROUP;
    m_mapActionWithEnum["3"] = ACCTION_FOLLOW_PROFILE;
    m_mapActionWithEnum["PageLike"] = ACCTION_LIKE_POST_GROUP;
    m_mapActionWithEnum["4"] = ACCTION_LIKE_POST_FRIEND;
    m_mapActionWithEnum["5"] = ACCTION_UPLOAD_PROFILE;
    m_mapActionWithEnum["6"] = ACCTION_UPLOAD_COVER;
    m_mapActionWithEnum["AddFriendSuggest"] = ACCTION_ADDFRIEN_SUGGESTION;
    m_mapActionWithEnum["AddFriendUID"] = ACCTION_ADDFRIEND_BY_UID;
    m_mapActionWithEnum["7"] = ACCTION_COMEND;
    m_mapActionWithEnum["8"] = ACCTION_CHANGE_PASSWORD;
    m_mapActionWithEnum["ShareNow"] = ACCTION_SHARE_NOW;
    return m_mapActionWithEnum;
}

bool ChromeWorker::checkCheckPoint()
{
    QString isCheckPoint = "";
    std::string checkPoint = "";
    try {
        checkPoint = driver->FindElement(ByXPath("//*[contains(@data-sigil, 'touchable')]")).GetText();
        isCheckPoint = driver->GetUrl().c_str();
        LOGD << "checkCheckPoint text: " << checkPoint.c_str();
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }
    if (checkPoint == REQUEST_REVIEW || checkPoint == REQUEST_REVIEW_2 || isCheckPoint.contains("checkpoint")) {
        m_isCheckPoint = true;
        m_infoClone->setCheckPoint(true);
        m_infoClone->addLog("Check Point");
        updateCheckPointToServer();
        return true;
    } else {
        return false;
    }
}

bool ChromeWorker::checkPoinLogin()
{
    try {
        std::string checkPoint = driver->FindElement(ByXPath("//*[contains(@data-sigil, 'touchable')]")).GetText();
        std::string login = driver->FindElement(ByName("login")).GetText();
        LOGD << "checkPoinLogin text: " << checkPoint.c_str();
        if (login == LOG_IN || checkPoint == CONTINUE || checkPoint == DOWN_LOAD_YOUR_INFOR) {
            m_isCheckPoint = true;
            m_infoClone->setCheckPoint(true);
            m_infoClone->addLog("Check Point");
            updateCheckPointToServer();
            return true;
        } else {
            return false;
        }
    } catch (const std::exception& ex) {
        return false;
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        return false;
        LOGD << "cant not finl element";
    } catch (...) {
        return false;
        LOGD << "cant not finl element";
    }
}

void ChromeWorker::updateCheckPointToServer()
{
    updateStatusCloneToServer(m_infoClone->getCloneId(), "alive_status" ,"checkpoint", "UpdateAliveStatus");
    QString typeCheckPoint = "";
    try {
        typeCheckPoint = driver->GetUrl().c_str();
    } catch (const std::exception& ex) {
        LOGD << "typeCheckPoint not found";
    } catch (const std::string& ex) {
        LOGD << "typeCheckPoint not found";
    } catch (...) {
        LOGD << "typeCheckPoint not found";
    }

    if(typeCheckPoint.contains("282/")) {
        m_infoClone->addLog("checkpoint 282");
        updateStatusCloneToServer(m_infoClone->getCloneId(), "checkpoint_type" ,"checkpoint_282", "UpdateAliveStatus");
        requestReviewButton();
        try {
            std::string getTextConfirm = driver->FindElement(ByClass("_3-8n")).GetText();
            QString textConfirm = getTextConfirm.c_str();
            LOGD << textConfirm;
            if(textConfirm.contains("Help us confirm it's you")) {
                return;
            }
        } catch (const std::exception& ex) {
            LOGD << "recaptcha-anchor-label not found";
        } catch (const std::string& ex) {
            LOGD << "recaptcha-anchor-label not found";
        } catch (...) {
            LOGD << "recaptcha-anchor-label not found";
        }

        if(m_infoClone->getLanguage() == LANGUAGE::ENGLISH) {
            m_infoClone->addLog("setting language Viet Nam");
            settingLanguageVietNam();
        }
        uncheckpointByPhone();

    } else if(typeCheckPoint.contains("956")){
        m_infoClone->addLog("checkpoint 956");
        updateStatusCloneToServer(m_infoClone->getCloneId(), "checkpoint_type" ,"checkpoint_956", "UpdateAliveStatus");
    }

}

void ChromeWorker::updateLanguageToServer()
{
    updateStatusCloneToServer(m_infoClone->getCloneId(), "setting_lang" ,"English", "UpdateSettingLang");
}

//Mở profile
void ChromeWorker::onStartFDriver(QString UID)
{
    LOGD << "onStartFDriver: " << QThread::currentThreadId();
    try {
        if(m_service_data != nullptr && m_infoClone != nullptr) {
//            if(nullptr == m_networkAccessSup) {
//                m_networkAccessSup = new NetworkAccessSupporter();
//            }
            QString proxy = m_service_data->getProxyChromWorker();
            if(proxy.isEmpty()){
                m_infoClone->setProxy("Sử dụng proxy mặc định");
            } else {
                m_infoClone->setProxy(proxy);
            }

            Chrome chrome;
            JsonObject chromeOptions;

            std::vector<std::string> args;
            if(m_service_data->getLinkProfile().toStdString() == "") {
                args.push_back( "--user-data-dir=" + QDir::currentPath().toStdString() + "/profiles/" + m_infoClone->getUID().toStdString());
            } else {
                args.push_back( "--user-data-dir=" + m_service_data->getLinkProfile().toStdString() + "/profiles/" + m_infoClone->getUID().toStdString());
            }
            args.push_back("--ignore-certificate-errors");

            switch(m_service_data->getProxyKind()){
            case PROXY_KIND::HTTP:
                args.push_back("--proxy-server= " + proxy.toStdString());
                LOGD << "-------------proxy:  " << proxy;
                break;
            case PROXY_KIND::SOCKS:
                args.push_back("--proxy-server= socks5://" + proxy.toStdString());
                LOGD << "-------------proxy:  " << proxy;
                break;
            }

//            args.push_back("--user-agent=" + m_infoClone->getUserAgent().toStdString());
            args.push_back("--disable-notifications");


            chromeOptions.Set<std::vector<std::string>>("args",args);
            chrome.SetChromeOptions(chromeOptions);
            fdriver::Size size;
            size.width = 300;
            size.height = 500;
            driver = new FDriver(chrome);
            driver->GetCurrentWindow().SetPosition(Point(m_service_data->getX(), m_service_data->getY()));
            driver->GetCurrentWindow().SetSize(size);

//            driver->Navigate("https://www.google.com/recaptcha/api2/demo");
//            Element element323 = driver->FindElement(ById("recaptcha-demo"));
//            LOGD << "find element323 sususes";
//            QString dataSideKey = element323.GetAttribute("data-sitekey").c_str();
//            LOGD << "GetAttribute(data-sitekey): " << dataSideKey;
//            QString url = "http://2captcha.com/in.php?key=a21390271e1975e7b1fd607b52b0f85e&method=userrecaptcha&googlekey=" + dataSideKey + "&pageurl=https://www.google.com/recaptcha/api2/demo";
//            QNetworkRequest request(QUrl(url.toStdString().c_str()));
//            connect(m_networkAccessSup, &NetworkAccessSupporter::onGetRequest, this, &ChromeWorker::slotGetId2Captcha);
//            m_networkAccessSup->getRequest(request);

//            driver->Navigate("https://whoer.net/");
//            u_sleep(15000);

//            unlockCaptcha();
//            u_sleep(15000);
//            actionRunInEachClone();
            actionAutofarmer();
//            followPageByAPI();
            u_sleep(15000);


        } else {
            LOGD << "onStartFDriver exception";
        }
    } catch (const std::exception& ex) {
        LOGD << "onStartFDriver exception";
    } catch (const std::string& ex) {
        LOGD << "onStartFDriver exception";
    } catch (...) {
        LOGD << "onStartFDriver exception";
    }
    finishRunning();
}


void ChromeWorker::unlockCaptcha()
{
    driver->Navigate("https://www.google.com/recaptcha/api2/demo");
    Element element323 = driver->FindElement(ById("recaptcha-demo"));
    LOGD << "find element323 sususes";
    QString dataSideKey = element323.GetAttribute("data-sitekey").c_str();
    LOGD << "GetAttribute(data-sitekey): " << dataSideKey;
//    getId2Captcha()
    m_captChaAPI.getId2Captcha(dataSideKey);

}

void ChromeWorker::slotGetId2Captcha(QNetworkReply *reply)
{
//    disconnect(m_networkAccessSup, nullptr, nullptr, nullptr);
//    QString replyInfo = reply->readAll();
//    LOGD << "slotGetId2Captcha: " << replyInfo;
//    QStringList listReply = replyInfo.split("|");
//    LOGD << "slotGetId2Captcha:  " << listReply.at(1);
//    m_idCaptCha = listReply.at(1);
//    QString url = "http://2captcha.com/res.php?key=a21390271e1975e7b1fd607b52b0f85e&action=get&id=" + listReply.at(1);
//    QNetworkRequest request(QUrl(url.toStdString().c_str()));
//    connect(m_networkAccessSup, &NetworkAccessSupporter::onGetRequest, this, &ChromeWorker::slotGetTocken2Captcha);
//    m_networkAccessSup->getRequest(request);

}

void ChromeWorker::slotGetTocken2Captcha(QNetworkReply *reply)
{
//    disconnect(m_networkAccessSup, nullptr, nullptr, nullptr);
//    QString replyTocken = reply->readAll();
//    LOGD << "slotGetTocken2Captcha: " << replyTocken;
//    if(replyTocken == "CAPCHA_NOT_READY" || replyTocken == "ERROR_WRONG_CAPTCHA_ID") {
//        getRequest2Captcha(m_idCaptCha);
//    } else if (replyTocken.contains("OK|")) {
//        LOGD << "okkkk";
//        QStringList dataReplytocken = replyTocken.split("|");
//        QString wirte_tokon_js = "document.getElementById('g-recaptcha-response').innerHTML='" + dataReplytocken.at(1) + "';";
//        driver->Execute(wirte_tokon_js.toStdString());
//        u_sleep(3000);
//        driver->Execute("document.getElementById('recaptcha-demo-form').submit();");
//        LOGD << "finish";
//        u_sleep(10000);
//    }

}


bool ChromeWorker::checkNoInternet()
{
    std::string message = "";
    QString messageNoInternet = "";
    try {
        message = driver->FindElement(ById("main-message")).GetText();
        messageNoInternet = QString::fromStdString(message);
    } catch (const std::exception& ex) {
        LOGD << "checkNoInternet exception";
    } catch (const std::string& ex) {
        LOGD << "checkNoInternet exception";
    } catch (...) {
        LOGD << "checkNoInternet exception";
    }
    if(messageNoInternet.contains("This site can’t be reached")) {
        return true;
    } else {
        return false;
    }
}

void ChromeWorker::uncheckpointByPhone()
{
    try {
//        Navigate("https://m.facebook.com", m_infoClone->getUID(), m_infoClone->getPassword(), m_service_data->get2fa(), "", 1);
        QString nameButton = "";
        try {
            nameButton = driver->FindElement(ByName("action_proceed")).GetText().c_str();
        } catch (const std::exception& ex) {
            LOGD << "nameButton not found";
        } catch (const std::string& ex) {
            LOGD << "nameButton not found";
        } catch (...) {
            LOGD << "nameButton not found";
        }
        try {
            driver->FindElement(ByName("action_unset_contact_point")).Click();
            u_sleep(1000);
        } catch (const std::exception& ex) {
            LOGD << "action_unset_contact_point not found";
        } catch (const std::string& ex) {
            LOGD << "action_unset_contact_point not found";
        } catch (...) {
            LOGD << "action_unset_contact_point not found";
        }
        int numberChange = 0;
        if(nameButton == CONTINUE) {
            requestReviewButton();
            while(numberChange <= 3 && m_optAvailabe == false && m_stopGetPhone == false) {
                getPhoneNumber();
                if(m_wait24H == true) {
                    break;
                }
                if (m_PhoneNumberAvailable == true) {
                    getOPTfromPhone();
                }
                numberChange++;
                u_sleep(2000);
                driver->FindElement(ByName("action_unset_contact_point")).Click();
            }
        } else if(nameButton == REQUEST_REVIEW_2) {
            requestReviewButton();

            while(numberChange <= 3 && m_optAvailabe == false && m_stopGetPhone == false) {
                getPhoneNumber();
                if(m_wait24H == true) {
                    break;
                }
                if (m_PhoneNumberAvailable == true) {
                    getOPTfromPhone();
                }
                numberChange++;
                u_sleep(2000);
                driver->FindElement(ByName("action_unset_contact_point")).Click();
            }
        } else {
            LOGD << "start uncheckpoint";
            while(numberChange <= 3 && m_optAvailabe == false && m_stopGetPhone == false) {
                getPhoneNumber();
                if(m_wait24H == true) {
                    break;
                }
                if (m_PhoneNumberAvailable == true) {
                    getOPTfromPhone();
                }
                numberChange++;
                u_sleep(2000);
//                driver->FindElement(ByName("action_upload_image")).Click();
            }
        }
        if(m_wait24H == false) {
            updateAvartaToUncheckPoint();
        }

    } catch (const std::exception& ex) {
        LOGD << "uncheckpointByPhone exception";
    } catch (const std::string& ex) {
        LOGD << "uncheckpointByPhone exception";
    } catch (...) {
        LOGD << "uncheckpointByPhone exception";
    }
}

void ChromeWorker::getPhoneNumber()
{
    try {
        bool requestPhoneAgain = true;
        int numberRequest = 0;
        while (numberRequest < 5 && requestPhoneAgain == true) {
            numberRequest++;
            QString phoneNumber = "";
            int count = 0;
            while (count <= 10 && phoneNumber == "") {
                phoneNumber = phoneAPI.getPhoneNumber();
                u_sleep(8000);
                count++;
                if(phoneNumber == "") {
                    m_infoClone->addLog("can't get phone number");
                    m_infoClone->addLog("try again get phone number");
                }
            }
            bool haveCountryCode = false;
            if(phoneNumber != "") {
                m_infoClone->addLog("add phone number: " + phoneNumber);
                try {
                    driver->FindElement(ByName("country_code")).SendKeys("Hợp chủng quốc Hoa Kỳ (+1) ");
                    haveCountryCode = true;
                } catch (const std::exception& ex) {
                    LOGD << "country_code exception";
                } catch (const std::string& ex) {
                    LOGD << "country_code exception";
                } catch (...) {
                    LOGD << "country_code exception";
                }
                if(phoneNumber != "" && haveCountryCode == false) {
                    phoneNumber = "+1" + phoneNumber;
                } else if(phoneNumber != "" && haveCountryCode == true) {
                    LOGD << "have country code";
                }
                u_sleep(1000);
//                driver->FindElement(ByName("contact_point")).SendKeys(phoneNumber.toStdString());
                for (int i = 0; i < phoneNumber.length(); i++) {
                    u_sleep(100);
                    driver->FindElement(ByName("contact_point")).SendKeys(Shortcut() << keys::Backspace);
                }

                inputTextManual("contact_point", phoneNumber, BY::BY_NAME);
                u_sleep(2000);
                driver->FindElement(ByName("action_set_contact_point")).Click();
                u_sleep(2000);
                QString requestText = "";
                try {
                    requestText = driver->FindElement(ByClass("_9u29")).GetText().c_str();
                } catch (const std::exception& ex) {
                    LOGD << "phone number ok";
                } catch (const std::string& ex) {
                    LOGD << "phone number ok";
                } catch (...) {
                    LOGD << "phone number ok";
                }
                if(requestText.contains("Vui lòng thử số khác")) {
                    m_infoClone->addLog("the phone number has been used");
                    m_infoClone->addLog("get an other phone number");
                    m_PhoneNumberAvailable = false;
                    requestPhoneAgain = true;
                } else if(requestText.contains("phải 24 giờ nữa")) {
                    m_infoClone->addLog("must wait 24h to add phone number");
                    m_wait24H = true;
                    break;
                } else {
                    m_PhoneNumberAvailable = true;
                    requestPhoneAgain = false;
                }
            } else {
                m_infoClone->addLog("can't get phone number");
                m_infoClone->addLog("try again get phone number");
                m_PhoneNumberAvailable = false;
            }
        }

    } catch (const std::exception& ex) {
        m_stopGetPhone = true;
        m_infoClone->addLog("can't add phone number in this clone");
        LOGD << "getPhoneNumber exception";
    } catch (const std::string& ex) {
        m_stopGetPhone = true;
        m_infoClone->addLog("can't add phone number in this clone");
        LOGD << "getPhoneNumber exception";
    } catch (...) {
        m_stopGetPhone = true;
        m_infoClone->addLog("can't add phone number in this clone");
        LOGD << "getPhoneNumber exception";
    }
}

void ChromeWorker::getOPTfromPhone()
{
    try {
        bool requestOptAgain = true;
        int numberRequest = 0;
        while (numberRequest < 3 && requestOptAgain == true) {
            numberRequest++;
            QString opt = "";
            int count = 0;
            while (count <= 7 && opt == "") {
                opt = phoneAPI.getOTP();
                LOGD << "get opt: " << opt;
                u_sleep(8000);
                count++;
            }
            if(opt != "") {
                m_infoClone->addLog("opt: " + opt);
//                driver->FindElement(ByName("code")).SendKeys(opt.toStdString());
                inputTextManual("code", opt, BY::BY_NAME);
                u_sleep(2000);
                std::vector<Element> submitCode = driver->FindElements(ByName("action_submit_code"));
                if(submitCode.size() >= 2) {
                    submitCode[1].Click();
                }
                u_sleep(2000);
//                driver->FindElement(ByName("action_upload_image")).Click();
                requestOptAgain = false;
                m_optAvailabe = true;
            } else {
                driver->FindElement(ByName("action_resend_code")).Click();
                requestOptAgain = true;
                m_optAvailabe = false;
                m_infoClone->addLog("get OPT fail");
                m_infoClone->addLog("try again get opt");
                u_sleep(2000);
            }
        }

    } catch (const std::exception& ex) {
        LOGD << "getOPTfromPhone exception";
    } catch (const std::string& ex) {
        LOGD << "getOPTfromPhone exception";
    } catch (...) {
        LOGD << "getOPTfromPhone exception";
    }

}

void ChromeWorker::updateAvartaToUncheckPoint()
{
    try {
        u_sleep(2000);
        m_infoClone->addLog("upload photo");
        QString path = QDir::currentPath() + "/image_av2";
        std::string image = pathUploadProfile(path).toStdString();
        LOGD << "image: " << image.c_str();
        driver->FindElement(ByName("mobile_image_data")).SendKeys(image);
        u_sleep(1000);
//        driver->FindElement(ByName("action_unset_contact_point")).Click();
        driver->FindElement(ByName("action_upload_image")).Click();
        m_infoClone->addLog("uncheckpoint successed");
        m_infoClone->setSttUncheckPoint(STATUS_UNCHECKPOINT::WAITING);
        updateStatusCloneToServer(m_infoClone->getCloneId(), "alive_status" ,"waiting_review", "UpdateAliveStatus");
        QDir dir(QString::fromStdString(image));
        dir.removeRecursively();
        if(m_infoClone->getLanguage() != LANGUAGE::ENGLISH) {
            m_infoClone->addLog("setting language english");
            settingLanguage();
        }
    } catch (const std::exception& ex) {
        LOGD << "updateAvartaToUncheckPoint exception";
    } catch (const std::string& ex) {
        LOGD << "updateAvartaToUncheckPoint exception";
    } catch (...) {
        LOGD << "updateAvartaToUncheckPoint exception";
    }
}

void ChromeWorker::doAcctionResult(QString acction)
{
    try {
        QJsonObject rootObj = QJsonDocument::fromJson(m_infoClone->getInfoJsonClone().toUtf8()).object();
        rootObj["action"] = acction;
        QJsonDocument cloneJsonDoc(rootObj);
        std::string cloneJsonPathToString(cloneJsonDoc.toJson(QJsonDocument::Compact));
//        WebAPI::getInstance()->doResult(m_infoClone->getCloneId().toStdString().c_str(), cloneJsonPathToString.c_str());
    } catch (const std::exception& ex) {
        LOGD << "doAcctionResult exception";
    } catch (const std::string& ex) {
        LOGD << "doAcctionResult exception";
    } catch (...) {
        LOGD << "doAcctionResult exception";
    }
}

void ChromeWorker::getRequest2Captcha(QString idCaptcha)
{
//    LOGD << "----------idCaptcha: " << idCaptcha;
//    u_sleep(5000);
//    QString url = "http://2captcha.com/res.php?key=a21390271e1975e7b1fd607b52b0f85e&action=get&id=" + idCaptcha;
//    QNetworkRequest request(QUrl(url.toStdString().c_str()));
//    connect(m_networkAccessSup, &NetworkAccessSupporter::onGetRequest, this, &ChromeWorker::slotGetTocken2Captcha);
//    m_networkAccessSup->getRequest(request);
}

bool ChromeWorker::waitUncheckPointSuccess()
{
    QString textInfo = "";
    try {
        std::string getTextInfo = driver->FindElement(ByXPath("//*[contains(text(), 'We received your information')]")).GetText();
        textInfo = getTextInfo.c_str();
    } catch (const std::exception& ex) {
        LOGD << "waitUncheckPointSuccess exception";
    } catch (const std::string& ex) {
        LOGD << "waitUncheckPointSuccess exception";
    } catch (...) {
        LOGD << "waitUncheckPointSuccess exception";
    }
    if(textInfo.contains("We received your information")) {
        updateStatusCloneToServer(m_infoClone->getCloneId(), "alive_status" ,"waiting_review", "UpdateAliveStatus");
    }
    if(textInfo.contains("We received your information") && m_infoClone->getCheckPoint() == false && m_infoClone->getSttUncheckPoint() != STATUS_UNCHECKPOINT::WAITING) {
        m_infoClone->addLog("clone waiting facebook unlock");
        m_isCheckPoint = true;
        m_infoClone->setCheckPoint(true);
        m_infoClone->setSttUncheckPoint(STATUS_UNCHECKPOINT::WAITING);
        return true;
    } else {
        return false;
    }
}

void ChromeWorker::actionRunInEachClone()
{
    try {
//        driver->Navigate("https://m.facebook.com");
        cookies(m_infoClone->getCookies());
        Navigate("https://m.facebook.com", m_infoClone->getUID(), m_infoClone->getPassword(), m_service_data->get2fa(), "", 1);
//        driver->Navigate("https://m.facebook.com/me");
        u_sleep(3000);
        std::vector<Cookie> getCookies = driver->GetCookies();
        std::string cookie = "";
        for(int i = 0; i < static_cast<int>(getCookies.size()); i++) {
            LOGD << "cookie name------>: " << driver->GetCookies().at(i).name.c_str();
            LOGD << "cookie value------>: " << driver->GetCookies().at(i).value.c_str();
            cookie += driver->GetCookies().at(i).name + "=" + driver->GetCookies().at(i).value;
            cookie += ";";
        }
        LOGD << "cookie cookie: " << cookie.c_str() << "---------end----------";
        Element bodyText = driver->FindElement(ByXPath("/html"));
        bodyText.SendKeys(Shortcut() << keys::Control << "F5");

        if(checkNoInternet()) {
            return;
        }
        nextButton();
        LOGD << "------doaction----start";
//        std::string doaction = WebAPI::getInstance()->doAction(m_infoClone->getCloneId().toStdString().c_str());
        std::string doaction = WebAPI::getInstance()->doAction(nullptr, "facebook" ,m_infoClone->getCloneId().toStdString().c_str());
        LOGD << "------doaction----fnish";
        LOGD << "get doaction: " << doaction.c_str();
        QString listAction = QString::fromStdString(doaction);
        QJsonDocument listJsonAction = QJsonDocument::fromJson(listAction.toUtf8());
        QJsonObject getDataAction = listJsonAction.object();
        QJsonValue codeJson = getDataAction.value(QString("code"));
        if(m_infoClone->getLanguage() != LANGUAGE::ENGLISH) {
            settingLanguage();
        }
        if(m_infoClone->getNameAcc() == "") {
            getInfoDataClone();
        }
        if (codeJson.toInt() == 200) {
            QJsonValue dataJson = getDataAction.value(QString("data"));
            int leng = dataJson.toArray().size();
            for (int i = 0; i < leng; i++) {
                if (checkPoinLogin() == true || checkCheckPoint() == true || waitUncheckPointSuccess() == true) {
                    break;
                } else {
                    LOGD << "Clone not check point";
                }

                if(m_infoClone->getLanguage() != LANGUAGE::ENGLISH) {
                    settingLanguage();
                }

                QJsonValue action = getDataAction.value(QString("data"))[i].operator[]("action");
                QJsonValue count = getDataAction.value(QString("data"))[i].operator[]("count");
                LOGD << "action: " << action.toString() << " | " << "count: " << count.toInt();
                QString numberRun = QString::number(count.toInt());
                LOGD << "numberRun: " << numberRun;
                if (action.toString() == "Watch") {
                    m_infoClone->addLog("Watch: " + numberRun);
                    u_sleep(1000);
                    videoButton(true, count.toInt());
                } else if (action.toString() == "FeedLike") {
                    m_infoClone->addLog("FeedLike: " + numberRun);
                    u_sleep(1000);
                    actionScrollDownFeedLike(true, count.toInt());

                } else if (action.toString() == "PostStatusImage") {
                    m_infoClone->addLog("PostStatusImage");
                    LOGD << "not yet impliment";
                } else if (action.toString() == "WatchLike") {
                    m_infoClone->addLog("WatchLike");
                    LOGD << "not yet impliment";
                } else if (action.toString() == "FriendLike") {
                    m_infoClone->addLog("FriendLike");
                    LOGD << "not yet impliment";
                } else if (action.toString() == "AddFriendSuggest") {
                    m_infoClone->addLog("AddFriendSuggest: " + numberRun);
                    u_sleep(1000);
                    addFrendSuggetion(true, count.toInt());
                } else if (action.toString() == "Feed") {
                    m_infoClone->addLog("Feed: " + numberRun);
                    actionScrollDownNewFeed(true, count.toInt());
                } else if (action.toString() == "AddFriendUID") {
                    m_infoClone->addLog("AddFriendUID");
                    u_sleep(1000);
                    addFriendByUID(true, "100006710623291,100007019198224,100007131240924");
                } else if (action.toString() == "ConfirmFriend") {
                    m_infoClone->addLog("ConfirmFriend: " + numberRun);
                    u_sleep(1000);
                    ActionConfirmFriend(true, count.toInt());
                } else if (action.toString() == "PageLike") {
                    m_infoClone->addLog("PageLike");
                    u_sleep(1000);
                    randomLikePostInGroup(true, "https://m.facebook.com/autofarmer9999/?tsid=0.5154616450878537&source=result");
                } else if (action.toString() == "JoinGroup") {
                    m_infoClone->addLog("JoinGroup");
                    u_sleep(1000);
                    navigateAndLikeGroup("https://m.facebook.com/groups/200251934658625?group_view_referrer=search");
                } else if (action.toString() == "ShareNow") {
                    m_infoClone->addLog("ShareNow");
                    u_sleep(1000);
                    shareNowPost(true, "");
                } else if (action.toString() == "GroupLike") {
                    m_infoClone->addLog("GroupLike");
                    u_sleep(1000);
                    randomLikePostInGroup(true, "https://m.facebook.com/autofarmer9999/?tsid=0.5154616450878537&source=result");
                } else if (action.toString() == "PostStatus") {
                    m_infoClone->addLog("PostStatus");
                    LOGD << "not yet impliment";
                }
                doAcctionResult(action.toString());
            }
        } else {
            m_infoClone->addLog("get action fail with code = " + QString::number(codeJson.toInt()));
            LOGD << "get action fail with code = " << QString::number(codeJson.toInt());
        }
    } catch (const std::exception& ex) {
        LOGD << "actionRunInEachClone exception";
    } catch (const std::string& ex) {
        LOGD << "actionRunInEachClone exception";
    } catch (...) {
        LOGD << "actionRunInEachClone exception";
    }
}

void ChromeWorker::actionAutofarmer()
{
    try {
        cookies(m_infoClone->getCookies());
        Navigate("https://m.facebook.com", m_infoClone->getUID(), m_infoClone->getPassword(), m_service_data->get2fa(), "", 1);
        int random = rand() % 10;
        if(random % 2 == 0) {
            watchStory();
        }
        if(checkNoInternet()) {
            return;
        }
        nextButton();
        LOGD << "------doaction----start";
//        std::string doaction = WebAPI::getInstance()->doAction(m_infoClone->getCloneId().toStdString().c_str());
        std::string doaction = WebAPI::getInstance()->doAction(nullptr, "facebook" ,m_infoClone->getCloneId().toStdString().c_str());
        LOGD << "------doaction----fnish";
        LOGD << "get doaction: " << doaction.c_str();
        QString listAction = QString::fromStdString(doaction);
        QJsonDocument listJsonAction = QJsonDocument::fromJson(listAction.toUtf8());
        QJsonObject getDataAction = listJsonAction.object();
        QJsonValue codeJson = getDataAction.value(QString("code"));
        if(m_infoClone->getLanguage() != LANGUAGE::ENGLISH) {
            settingLanguage();
        }
        if(m_infoClone->getNameAcc() == "") {
            getInfoDataClone();
        }
        driver->Navigate("https://m.facebook.com");
        if (codeJson.toInt() == 200) {
            QJsonValue dataJson = getDataAction.value(QString("actions"));
            int leng = dataJson.toArray().size();
            for (int i = 0; i < leng; i++) {
                if (checkPoinLogin() == true || checkCheckPoint() == true || waitUncheckPointSuccess() == true) {
                    break;
                } else {
                    LOGD << "Clone not check point";
                }

                if(m_infoClone->getLanguage() != LANGUAGE::ENGLISH) {
                    settingLanguage();
                }

                QJsonValue action = getDataAction.value(QString("actions"))[i].operator[]("action");
                QJsonValue count = getDataAction.value(QString("actions"))[i].operator[]("count");
                QJsonValue faceID = getDataAction.value(QString("actions"))[i].operator[]("fb_id");
                QJsonValue postID = getDataAction.value(QString("actions"))[i].operator[]("post_id");
                LOGD << "action: " << action.toString() << " | " << "count: " << count.toInt();
                QString numberRun = QString::number(count.toInt());
                LOGD << "numberRun: " << numberRun;
                if (action.toString() == "Watch") {
                    m_infoClone->addLog("Watch: " + numberRun);
                    u_sleep(1000);
                    videoButton(true, count.toInt());
                } else if (action.toString() == "FeedLike") {
                    m_infoClone->addLog("FeedLike: " + numberRun);
                    u_sleep(1000);
                    actionScrollDownFeedLike(true, count.toInt());

                } else if (action.toString() == "PostStatusImage") {
                    m_infoClone->addLog("PostStatusImage");
                    LOGD << "not yet impliment";
                } else if (action.toString() == "WatchLike") {
                    m_infoClone->addLog("WatchLike");
                    LOGD << "not yet impliment";
                } else if (action.toString() == "FriendLike") {
                    m_infoClone->addLog("FriendLike");
                    LOGD << "not yet impliment";
                } else if (action.toString() == "AddFriendSuggest") {
                    m_infoClone->addLog("AddFriendSuggest: " + numberRun);
                    u_sleep(1000);
                    addFrendSuggetion(true, count.toInt());
                } else if (action.toString() == "Feed") {
                    m_infoClone->addLog("Feed: " + numberRun);
                    actionScrollDownNewFeed(true, count.toInt());
                } else if (action.toString() == "AddFriendUID") {
                    m_infoClone->addLog("AddFriendUID");
                    u_sleep(1000);
                    addFriendByUID(true, "100006710623291,100007019198224,100007131240924");
                } else if (action.toString() == "ConfirmFriend") {
                    m_infoClone->addLog("ConfirmFriend: " + numberRun);
                    u_sleep(1000);
                    ActionConfirmFriend(true, count.toInt());
                } else if (action.toString() == "PageLike") {
                    m_infoClone->addLog("PageLike");
                    u_sleep(1000);
                    randomLikePostInGroup(true, "https://m.facebook.com/autofarmer9999/?tsid=0.5154616450878537&source=result");
                } else if (action.toString() == "JoinGroup") {
                    m_infoClone->addLog("JoinGroup");
                    u_sleep(1000);
                    navigateAndLikeGroup("https://m.facebook.com/groups/200251934658625?group_view_referrer=search");
                } else if (action.toString() == "ShareNow") {
                    m_infoClone->addLog("ShareNow");
                    u_sleep(1000);
                    shareNowPost(true, "");
                } else if (action.toString() == "GroupLike") {
                    m_infoClone->addLog("GroupLike");
                    u_sleep(1000);
                    randomLikePostInGroup(true, "https://m.facebook.com/autofarmer9999/?tsid=0.5154616450878537&source=result");
                } else if (action.toString() == "PostStatus") {
                    m_infoClone->addLog("PostStatus");
                    LOGD << "not yet impliment";
                } else if (action.toString() == "facebook_likepage_high" || action.toString() == "facebook_likepage_low"
                             || action.toString() == "likepage_warranty_high" ||  action.toString() == "facebook_likepage_medium") {
                    m_infoClone->addLog(action.toString());
                    u_sleep(1000);
                    likePage(faceID.toString());
                }  else if (action.toString() == "facebook_follow" || action.toString() == "facebook_follow_high"
                            || action.toString() == "facebook_follow_low") {
                    m_infoClone->addLog(action.toString());
                    u_sleep(1000);
                    navigateAndFollowProfile(faceID.toString());
                    u_sleep(30000);
                }  else if (action.toString() == "facebook_reaction_low" || action.toString() == "facebook_reaction_high"
                            || action.toString() == "facebook_bufflike_low" || action.toString() == "facebook_bufflike_high") {
                    m_infoClone->addLog(action.toString());
                    u_sleep(1000);
                    likePost(postID.toString());
                    u_sleep(30000);
                }
                doAcctionResult(action.toString());
            }
        } else {
            m_infoClone->addLog("get action fail with code = " + QString::number(codeJson.toInt()));
            LOGD << "get action fail with code = " << QString::number(codeJson.toInt());
        }
    } catch (const std::exception& ex) {
        LOGD << "actionRunInEachClone exception";
    } catch (const std::string& ex) {
        LOGD << "actionRunInEachClone exception";
    } catch (...) {
        LOGD << "actionRunInEachClone exception";
    }
}

void ChromeWorker::inputTextManual(string element, QString textInput, BY type)
{
    try {
        auto parts = textInput.split(QString());
        Element FindElement;
        if(type == BY::BY_XPATH) {
            FindElement = driver->FindElement(ByXPath(element));
        } else if (type == BY::BY_NAME) {
            FindElement = driver->FindElement(ByName(element));
        } else if (type == BY::BY_ID) {
            FindElement = driver->FindElement(ById(element));
        } else if (type == BY::BY_CLASS) {
            FindElement = driver->FindElement(ByClass(element));
        } else if (type == BY::BY_LINK_TEXT) {
            FindElement = driver->FindElement(ByLinkText(element));
        }
        for (int i = 1; i < parts.length(); i++) {
            u_sleep(randomSleep(200, 400));
            FindElement.SendKeys(parts[i].toStdString());
        }

    } catch (const std::exception& ex) {
        LOGD << "inputTextManual exception";
    } catch (const std::string& ex) {
        LOGD << "inputTextManual exception";
    } catch (...) {
        LOGD << "inputTextManual exception";
    }
}

// đăng nhâp profile
bool ChromeWorker::Navigate(QString url, QString uid, QString pass, std::string haifa, QString cookie, int row)
{
    m_infoClone->addLog("Navigate facebook");
    std::string utf8_textUid = uid.toStdString();
    std::string utf8_textPass = pass.toStdString();
    std::string utf8_textCookie = cookie.toStdString();
    QString qstring2Fa = QString::fromStdString(haifa);

    driver->Navigate(url.toStdString());
    QString outerHTML = "";
    if(uid == "" && pass == "" && haifa == "") {
        cookies(cookie);
    } else {
        try {
            loginAgain();
            inputTextManual("email", uid, BY::BY_NAME);
            Element element = driver->FindElement(ByName("email"));
//            element.SendKeys(utf8_textUid);
            u_sleep(1000);
            element.SendKeys(Shortcut() << keys::Control << "n");
            inputTextManual("pass", pass, BY::BY_NAME);
            Element element1 = driver->FindElement(ByName("pass"));
//            element1.SendKeys(utf8_textPass);
            u_sleep(1000);

            Element element2 = driver->FindElement(ByName("login"));
            element2.Click();
            u_sleep(3000);

            if(haifa != "") {
                try {
                    m_infoClone->addLog("get 2FA");
                    u_sleep(5000);
                    driver->FindElement(ByName("approvals_code")).SendKeys(haifa);
//                    inputTextManual("approvals_code", qstring2Fa, BY::BY_NAME);
                    u_sleep(1000);
                    driver->FindElement(ByName("submit[Submit Code]")).Click();
                    u_sleep(1000);
                    driver->FindElement(ByName("submit[Continue]")).Click();
                    u_sleep(3000);
                    m_infoClone->addLog("get 2FA finish");
                } catch (const std::exception& ex) {
                    LOGD << "cant not finl element";
                } catch (const std::string& ex) {
                    LOGD << "cant not finl element";
                } catch (...) {
                    LOGD << "cant not finl element";
                }
            }
            u_sleep(2000);
            requestReviewButton();
            if(rejectUncheckPoint() == true) {
                return true;
            }

            Element bodyText = driver->FindElement(ByXPath("/html"));
            bodyText.SendKeys(Shortcut() << keys::Control << "F5");

            outerHTML = QString::fromUtf8(bodyText.GetAttribute("outerHTML").c_str());
            LOGD<<outerHTML.count();

            if(m_infoClone->getLanguage() != LANGUAGE::ENGLISH) {
                settingLanguage();
            }
            requestReviewButton();

        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
    return 0;
}

QString ChromeWorker::pathUploadProfile(QString url)
{
    QDir dir(url);
    QStringList finters;
    finters << "*.jpg" << "*.jpeg" << "*.jpe" << "*.jif" << "*.jfif" << "*.jfi" << "*.webp" <<
               "*.gif" << "*.png" << "*.apng" << "*.bmp" << "*.dib" << "*.tiff" << "*.tif" <<
               "*.svg" << "*.svgz" << "*.ico" << "*.xbm" ;
    dir.setNameFilters(finters);
    QFileInfoList files = dir.entryInfoList();
    QStringList lisImage;
    foreach (QFileInfo fileInfo, files) {
        QString path=fileInfo.absoluteFilePath();
        lisImage.append(path);
        QFile file(path);
        if(!file.exists()){
            LOGD << "This file is not exist";
        }else{
            LOGD << "This file was found " << file.fileName();
        }
    }
    LOGD << "files.length(): " << files.length();
    if(files.length() >= 1) {
        m_linkImage = lisImage.at(randomvalue(files.length() - 1));
    }
    return m_linkImage;
}

void ChromeWorker::finishRunning()
{
    LOGD << "finishRunning()";
    m_infoClone->setProxy(QString());
    if (driver != nullptr) {
        u_sleep(1000);
        try {
//            closeSession();
            driver->CloseCurrentWindow();
//            std::vector<Window> win = driver->GetWindows();
//            for (int i = 0; i < static_cast<int>(win.size()); i++) {
//                try {
//                    driver->SetFocusToWindow(win[i]).CloseCurrentWindow();
//                } catch (const std::exception& ex) {
//                    LOGD << "CloseCurrentWindow exeption";
//                } catch (const std::string& ex) {
//                    LOGD << "CloseCurrentWindow exeption";
//                } catch (...) {
//                    LOGD << "CloseCurrentWindow exeption";
//                }
//            }
        } catch (const std::exception& ex) {
            LOGD << "CloseCurrentWindow exeption";
        } catch (const std::string& ex) {
            LOGD << "CloseCurrentWindow exeption";
        } catch (...) {
            LOGD << "CloseCurrentWindow exeption";
        }
//        delete driver;
    }
    m_infoClone->addLog("Finished");
    emit workerFinished();
}

void ChromeWorker::cookies(QString cookie) {

    std::string c_user = "";
    std::string xs = "";
    std::string fr = "";
    std::string datr = "";
    try {
        if(cookie.contains(";")) {
            QStringList cookiear = cookie.split(";");
            for (int i= 0; i<cookiear.size() ;i++ ) {
                QStringList cookiearr = cookiear[i].split("=");
                std::vector<Cookie> existed = driver->GetCookies();
                for(int i = 0; i < static_cast<int>(existed.size()); i++) {
                    Cookie tmp = existed.at(i);
                }

                for (auto i: cookiearr) {
                    if (cookiearr[0].trimmed() == "c_user") {
                        std::string c_userarr = cookiearr[1].toStdString();
                        c_user = c_userarr;
                        Cookie cookie1("c_user", c_user, "/", "");
                        driver-> SetCookie(cookie1);
                    }
                    if (cookiearr[0].trimmed() == "xs") {

                        std::string xsarr = cookiearr[1].toStdString();
                        xs = xsarr;
                        Cookie cookie2("xs", xs, "/", "");
                        driver-> SetCookie(cookie2);
                    }
                    if (cookiearr[0].trimmed() == "fr") {

                        std::string frarr = cookiearr[1].toStdString();
                        fr = frarr;
                        Cookie cookie3("fr", fr, "/", "");
                        driver-> SetCookie(cookie3);
                    }
                    if (cookiearr[0].trimmed() == "datr") {

                        std::string datrarr = cookiearr[1].toStdString();
                        datr = datrarr;
                        Cookie cookie4("datr", datr, "/", "");
                        driver-> SetCookie(cookie4);
                    }
                }
            }
        }
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }

}


Element ChromeWorker::FindElementBy(By by)
{
    LOGD;
    try {
        return driver->FindElement(by);
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }

}

void ChromeWorker::SendKeys(QString key)
{
    try {
        driver->SendKeys(key.toStdString());
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }

}

void ChromeWorker::checkUncheckPointSusscess()
{
    QString getText = "";
    try {
        std::vector<Element> element = driver->FindElements(ByClass("_5xu4"));
        if(element.size() >= 2) {
            getText = element[1].GetText().c_str();
            LOGD << getText;
        }
    } catch (const std::exception& ex) {
        LOGD << "checkUncheckPointSusscess cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "checkUncheckPointSusscess cant not finl element";
    } catch (...) {
        LOGD << "checkUncheckPointSusscess cant not finl element";
    }
    if(getText.contains("What's on your mind?") && m_infoClone->getCheckPoint() == true) {
        m_infoClone->setSttUncheckPoint(STATUS_UNCHECKPOINT::SUCCESS);
    }
}

QString ChromeWorker::GetScreenShot()
{
    try {
        return QString(driver->GetScreenshot().data());
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }

}

//Mở profile và thực hiện cuộn màn hình đối với friend
void ChromeWorker::friendRequestButton(bool check)
{
    if(check == true) {
        try {
            driver->FindElement(ByName("Friend requests")).Click();
            u_sleep(1000);
            actionScrollDown(5);
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::messagesButton(bool check)
{
    if(check == true) {
        try {
            driver->FindElement(ByName("Messages")).Click();
            u_sleep(2000);
            driver->FindElement(ByXPath("//*[contains(@data-sigil, 'MBackNavBarClick')]")).Click();
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

//Mở profile và thực hiện cuộn màn hình đối với watch
void ChromeWorker::videoButton(bool check, int num)
{
    if(check == true) {
        try {
            u_sleep(2000);
            Element element = driver->FindElement(ById("videos_tab_jewel"));
            element.Click();
            u_sleep(4000);
            for (int i = 0; i < num; i++) {
                u_sleep(1000);
                Element bodyText = driver->FindElement(ByXPath("/html"));
                bodyText.SendKeys(Shortcut() << keys::PageDown);
            }
            newFeedButton(true);
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::notificationButton(bool check)
{
    if(check == true) {
        try {
            driver->FindElement(ByName("Notifications")).Click();
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::moreButton(bool check)
{
    if(check == true) {
        driver->Navigate("https://m.facebook.com");
        try {
            u_sleep(1000);
            driver->FindElement(ByName("More")).Click();
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::newFeedButton(bool check)
{
    if(check == true) {
        try {
            driver->FindElement(ByName("News Feed")).Click();
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::navigateAndLikeGroup(QString url)
{
    if(url != "") {
        try {
            driver->Navigate(url.toStdString());
            u_sleep(3000);
            driver->FindElement(ByClass("_1f9d")).Click();
            u_sleep(2000);
            driver->Navigate("https://m.facebook.com");
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }

    }
}

void ChromeWorker::settingLanguage()
{
    try {
        m_infoClone->addLog("Setting Language");
        u_sleep(1000);
        driver->Navigate("https://m.facebook.com/language/");
        u_sleep(1000);
        driver->FindElement(ByXPath("//*[contains(text(),'English (US)')]")).Click();
        u_sleep(1000);
        driver->Navigate("https://m.facebook.com");
        updateLanguageToServer();
        m_infoClone->setLanguage(LANGUAGE::ENGLISH);
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }
}

void ChromeWorker::settingLanguageVietNam()
{
    try {
        m_infoClone->addLog("Setting Language");
        u_sleep(1000);
        driver->Navigate("https://m.facebook.com/language/");
        u_sleep(1000);
        driver->FindElement(ByXPath("//*[contains(text(),'Tiếng Việt')]")).Click();
        u_sleep(1000);
        driver->Navigate("https://m.facebook.com");
        updateLanguageToServer();
        m_infoClone->setLanguage(LANGUAGE::VIET_NAM);
    } catch (const std::exception& ex) {
        LOGD << "cant not settingLanguage VietNam";
    } catch (const std::string& ex) {
        LOGD << "cant not settingLanguage VietNam";
    } catch (...) {
        LOGD << "cant not settingLanguage VietNam";
    }
}

void ChromeWorker::nextButton()
{
    try {
        u_sleep(3000);
        driver->FindElement(ById("nux-nav-button")).Click();
        u_sleep(1000);
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element nextButton";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element nextButton";
    } catch (...) {
        LOGD << "cant not finl element nextButton";
    }
}

void ChromeWorker::loginAgain()
{
    try {
        driver->FindElement(ByXPath("//*[contains(@data-sigil, 'login_profile_form')]")).Click();
        u_sleep(1000);
        Element element1 = driver->FindElement(ByName("pass"));
        element1.SendKeys(m_infoClone->getPassword().toStdString());
        u_sleep(1000);

        Element element2 = driver->FindElement(ByXPath("//*[contains(@data-sigil, 'touchable password_login_button')]")).Click();;
        element2.Click();
        u_sleep(3000);

        if(m_service_data->get2fa() != "") {
            try {
                m_infoClone->addLog("get 2FA");
                u_sleep(5000);
                driver->FindElement(ByName("approvals_code")).SendKeys(m_service_data->get2fa());
                u_sleep(1000);
                driver->FindElement(ByName("submit[Submit Code]")).Click();
                u_sleep(1000);
                driver->FindElement(ByName("submit[Continue]")).Click();
                u_sleep(3000);
                m_infoClone->addLog("get 2FA finish");
            } catch (const std::exception& ex) {
                LOGD << "cant not finl element";
            } catch (const std::string& ex) {
                LOGD << "cant not finl element";
            } catch (...) {
                LOGD << "cant not finl element";
            }
        }
        u_sleep(2000);

        std::vector<Element> element = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'touchable')]"));
        LOGD << "number elemet: " << element.size();
        for (int i = 0; i < static_cast<int>(element.size()); i++) {
            u_sleep(2000);
            if (element.at(i).GetAttribute("value") == "OK") {
                element.at(i).Click();
            }
        }

    } catch (const std::exception& ex) {
        LOGD << "cant not finl element to loginAgain";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element to loginAgain";
    } catch (...) {
        LOGD << "cant not finl element to loginAgain";
    }
}

bool ChromeWorker::rejectUncheckPoint()
{
    QString getText = "";
    try {
        getText = driver->FindElement(ByClass("_3-8n")).GetText().c_str();

    } catch (const std::exception& ex) {
        LOGD << "rejectUncheckPoint exception";
    } catch (const std::string& ex) {
        LOGD << "rejectUncheckPoint exception";
    } catch (...) {
        LOGD << "rejectUncheckPoint exception";
    }
    if (getText.contains("Your account has been disabled")) {
        m_infoClone->addLog("request uncheck point has been facebook reject");
        m_infoClone->setSttUncheckPoint(STATUS_UNCHECKPOINT::UNSUCCESS);
        updateStatusCloneToServer(m_infoClone->getCloneId(), "alive_status" ,"checkpoint_rejected", "UpdateAliveStatus");
        return true;
    } else {
        return false;
    }
}

void ChromeWorker::navigateAndFollowProfile(QString UID)
{
    if(UID != "") {
        try {
            driver->Navigate("https://m.facebook.com/" + UID.toStdString());
            u_sleep(3000);
            std::vector<Element> element1 = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'hq-profile-logging-action-bar-button flyout-causal')]"));
            LOGD << "size element: " << element1.size();
            if(element1.size() >= 1) {
                element1[element1.size() -1].Click();
                u_sleep(1000);
                driver->FindElement(ByXPath("//*[contains(@data-sigil, 'touchable touchable hq-profile-logging-action-bar-button')]")).Click();
            }
            u_sleep(2000);
            scrollDown(3);
            driver->Navigate("https://m.facebook.com");
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }

    }
}

//Mở profile và thực hiện cuộn màn hình đối với home
void ChromeWorker::actionScrollDownNewFeed(bool check, int num)
{
    if(check == true) {
        try {
            driver->Navigate("https://m.facebook.com");
            for(int i = 0; i < num; i++) {
                u_sleep(1000);
                Element bodyText = driver->FindElement(ByXPath("/html"));
                bodyText.SendKeys(Shortcut() << keys::PageDown);
            }
            newFeedButton(true);
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::scrollDown(int num)
{
    for(int i = 0; i < num; i++) {
        u_sleep(1000);
        Element bodyText = driver->FindElement(ByXPath("/html"));
        bodyText.SendKeys(Shortcut() << keys::PageDown);
    }
}

void ChromeWorker::actionScrollDownFeedLike(bool check, int likeNumber)
{
    if(check == true) {
        try {
            driver->Navigate("https://m.facebook.com");
            u_sleep(1000);
            std::vector<Element> element = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'touchable ufi-inline-like like-reaction-flyout')]"));
            LOGD << "number elemet: " << element.size();
            for (int i = 0; i < static_cast<int>(element.size()); i++) {
                u_sleep(2000);
                Element bodyText = driver->FindElement(ByXPath("/html"));
                bodyText.SendKeys(Shortcut() << keys::PageDown);
                if(i < likeNumber){
                    u_sleep(500);
                    Session mouse = driver->MoveToCenterOf(element.at(i));
                    if (element.at(i).GetAttribute("aria-pressed") == "false") {
                        element.at(i).Click();
                        u_sleep(2000);
                        Element bodyText = driver->FindElement(ByXPath("/html"));
                        bodyText.SendKeys(Shortcut() << keys::PageDown);
                    }
                } else {
                    break;
                }
            }
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}


//Cấu hình số lượt cuộn màn hình, thời gian giữa các lần cuộn, có di chuyển chuột
void ChromeWorker::actionScrollDown(int num)
{
    for(int i = 0; i < num; i++) {
        u_sleep(1000);
        try {
            Element bodyText = driver->FindElement(ByXPath("/html"));
            bodyText.SendKeys(Shortcut() << keys::PageDown);
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::actionScrollUp(int num)
{
    for(int i = 0; i < num; i++) {
        u_sleep(1000);
        try {
            Element bodyText = driver->FindElement(ByXPath("/html"));
            bodyText.SendKeys(Shortcut() << keys::PageUp);
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::ActionConfirmFriend(bool check, int num)
{
    if(check == true) {
        try {
            driver->Navigate("https://m.facebook.com");
            u_sleep(1000);
            driver->FindElement(ById("requests_jewel")).Click();
            u_sleep(2000);
            std::vector<Element> listConfirmFriend = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'touchable confirm-request')]"));
            for(int i = 0; i < num; i++) {
                u_sleep(1000);
                if(i < static_cast<int>(listConfirmFriend.size())) {
                    listConfirmFriend[i].Click();
                } else {
                    LOGD << "finish confirm friend";
                }
            }
            newFeedButton(true);
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::addFrendSuggetion(bool check, int num)
{
    try {
        if(check == true) {
            driver->Navigate("https://m.facebook.com");
            u_sleep(1000);
            driver->FindElement(ById("requests_jewel")).Click();
            u_sleep(2000);
            driver->FindElement(ByXPath("//*[contains(@data-sigil, 'friends-center-nav-item')]")).Click();
            actionScrollDown(1);
            u_sleep(1000);
            std::vector<Element> element2 = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'm-add-friend-secondary')]"));
            for(int i = 0; i < num; i++) {
                u_sleep(1000);
                if(i + 1 < static_cast<int>(element2.size())) {
                    element2[i + 1].Click();
                }
            }
            u_sleep(2000);
            actionScrollUp(3);
        }
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }
}

void ChromeWorker::addFriendByUID(bool check, QString UID)
{

    if(check == true) {
        try {
            QStringList listUID = UID.split(QLatin1Char(','), Qt::SkipEmptyParts);
            for(int i = 0; i < listUID.size(); i++) {
                u_sleep(2000);
                driver->Navigate("https://m.facebook.com/" + listUID[i].toStdString());
                u_sleep(1000);
                driver->FindElement(ByXPath("//*[contains(@data-sigil, 'add-friend hq-profile-logging-action-bar-button')]")).Click();
            }
            u_sleep(1000);
            driver->Navigate("https://m.facebook.com");
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }

}

void ChromeWorker::uploadProfile(bool check)
{
    try {
        if(check == true) {
            u_sleep(3000);
            driver->Navigate("https://m.facebook.com");
            u_sleep(1000);
            moreButton(true);
            u_sleep(3000);
            std::vector<Element> element = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'touchable')]"));
            element[0].Click();
            u_sleep(2000);
            std::vector<Element> editProfile = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'dialog-link')]"));
            editProfile[1].Click();
            u_sleep(2000);
            Element element2 = driver->FindElement(ById("nuxPicFileInput"));
            u_sleep(2000);
            std::string image = pathUploadProfile("C:/Users/T470S/Desktop/image profile").toStdString();
            LOGD << "image: " << image.c_str();
            element2.SendKeys(image);
            u_sleep(3000);
            driver->FindElement(ById("nuxUploadPhotoButton")).Click();
            u_sleep(3000);
            actionScrollDown(5);
            u_sleep(1000);
            driver->Navigate("https://m.facebook.com");

        }
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }
}

void ChromeWorker::uploadCover(bool check)
{
    try {
        if(check == true) {
            u_sleep(3000);
            driver->Navigate("https://m.facebook.com");
            u_sleep(1000);
            moreButton(true);
            u_sleep(3000);
            std::vector<Element> element = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'touchable')]"));
            element[0].Click();
            u_sleep(2000);
            std::vector<Element> editProfile = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'dialog-link')]"));
            editProfile[0].Click();
            u_sleep(2000);
            Element element2 = driver->FindElement(ByXPath("//*[@id='modalDialogView']/div/div[1]/div[4]/div/a"));
            element2.Click();
            u_sleep(1000);
            Element element3 = driver->FindElement(ById("nuxPicFileInput"));
            u_sleep(2000);
            std::string image = pathUploadProfile("C:/Users/T470S/Desktop/image profile").toStdString();
            element3.SendKeys(image);
            u_sleep(3000);
            driver->FindElement(ById("nuxUploadPhotoButton")).Click();
            u_sleep(3000);
            actionScrollDown(5);
            u_sleep(1000);
            driver->Navigate("https://m.facebook.com");
        }
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }
}

void ChromeWorker::requestReviewButton()
{
    try {
        driver->FindElement(ByName("action_proceed")).Click();
    } catch (const std::exception& ex) {
        LOGD << "nameButton not found";
    } catch (const std::string& ex) {
        LOGD << "nameButton not found";
    } catch (...) {
        LOGD << "nameButton not found";
    }
}

Session ChromeWorker::gotoGroup(QString url)
{
    return driver->Navigate(url.toStdString());
}

void ChromeWorker::randomLikePostInGroup(bool check, QString url)
{
    if(check == true) {
        try {
            u_sleep(3000);
            gotoGroup(url);
            actionScrollDown(3);
            u_sleep(1000);
            std::vector<Element> element = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'touchable ufi-inline-like like-reaction-flyout')]"));
            LOGD << "number elemet: " << element.size();
            for (int i = 0; i < static_cast<int>(element.size()); i++) {
                u_sleep(2000);
                Element bodyText = driver->FindElement(ByXPath("/html"));
                bodyText.SendKeys(Shortcut() << keys::PageDown);
                if(m_service_data->random(element.size()) % 2 == 0){
                    u_sleep(1000);
                    if (element.at(i).GetAttribute("aria-pressed") == "false") {
                        element.at(i).Click();
                    }
                } else {
                    u_sleep(2000);
                    Element bodyText = driver->FindElement(ByXPath("/html"));
                    bodyText.SendKeys(Shortcut() << keys::PageDown);
                }
            }
            driver->Navigate("https://m.facebook.com");
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

Session ChromeWorker::gotoFriend(QString url)
{
    try {
        return driver->Navigate(url.toStdString());
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }

}

void ChromeWorker::randomLikePostFriend(bool check, QString url)
{
    if(check == true) {
        try {
            u_sleep(3000);
            gotoGroup(url);
            actionScrollDown(3);
            u_sleep(1000);
            std::vector<Element> element = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'touchable ufi-inline-like like-reaction-flyout')]"));
            LOGD << "number elemet: " << element.size();
            for (int i = 0; i < static_cast<int>(element.size()); i++) {
                u_sleep(2000);
                Element bodyText = driver->FindElement(ByXPath("/html"));
                bodyText.SendKeys(Shortcut() << keys::PageDown);
                if(m_service_data->random(element.size()) % 2 == 0){
                    u_sleep(500);
                    if (element.at(i).GetAttribute("aria-pressed") == "false") {
                        element.at(i).Click();
                    }
                } else {
                    u_sleep(2000);
                    Element bodyText = driver->FindElement(ByXPath("/html"));
                    bodyText.SendKeys(Shortcut() << keys::PageDown);
                }
            }
            driver->Navigate("https://m.facebook.com");
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::checkNewClone(bool check)
{
    try {
        if(check == true) {
            moreButton(true);
            u_sleep(1000);
            newFeedButton(true);
            u_sleep(1000);
            newFeedButton(true);
            u_sleep(1000);
            Element elemetNext = driver->FindElement(ByXPath("//*[contains(@data-sigil, 'mChromeHeaderRight')]")).Click();
            u_sleep(1000);
        }
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }
}

void ChromeWorker::shareNowPost(bool check, QString urlPost)
{

    if(check == true) {
        try {
            u_sleep(2000);
            //            driver->Navigate("https://m.facebook.com/story.php?story_fbid=3191041907884676&id=1848124...d_mobile");
            Element element = driver->FindElement(ById("videos_tab_jewel")).Click();
            u_sleep(4000);
            driver->FindElement(ByLinkText("Share")).Click();
            u_sleep(2000);
            driver->FindElement(ById("share-one-click-button")).Click();
            u_sleep(2000);
            driver->Navigate("https://m.facebook.com");
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::commentPost(bool check, QString text)
{
    try {
        if(check == true) {
            actionScrollDown(3);
            u_sleep(1000);
            std::vector<Element> element = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'feed-ufi-focus feed-ufi-trigger ufiCommentLink mufi-composer-focus')]"));
            element[1].Click();
            u_sleep(1000);
            driver->FindElement(ByXPath("//*[contains(@data-sigil, 'textarea mufi-composer m-textarea-input')]")).SendKeys(text.toStdString().c_str());
            u_sleep(2000);
            driver->FindElement(ByXPath("//*[contains(@data-sigil, 'touchable composer-submit')]")).Click();
            u_sleep(2000);
            driver->Navigate("https://m.facebook.com");
        }
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }
}

void ChromeWorker::likePage(QString UID)
{
    try {
        driver->Navigate("https://m.facebook.com/" + UID.toStdString());
        u_sleep(2000);
        scrollDown(1);
        driver->FindElement(ByXPath("img[@src='https://static.xx.fbcdn.net/rsrc.php/v3/yK/r/sbuwNgI3bKj.png']")).Click();
        u_sleep(2000);
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element likePage";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element likePage";
    } catch (...) {
        LOGD << "cant not finl element likePage";
    }
}

void ChromeWorker::likePost(QString UID)
{
    try {
        u_sleep(3000);
        driver->Navigate("https://m.facebook.com/" + UID.toStdString());
        u_sleep(1000);
        Element element = driver->FindElement(ByXPath("//*[contains(@data-sigil, 'touchable ufi-inline-like like-reaction-flyout')]"));
        driver->MoveToCenterOf(element);
        if (element.GetAttribute("aria-pressed") == "false") {
            element.Click();
            u_sleep(2000);
        } else {
            driver->Navigate("https://m.facebook.com");
        }
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element";
    } catch (...) {
        LOGD << "cant not finl element";
    }
}

void ChromeWorker::watchStory()
{
    try {
        std::vector<Element> element = driver->FindElements(ByXPath("//*[contains(@data-sigil, 'm-stories-rectangular-item-title')]"));
        if(element.size() >= 2) {
            element[1].Click();
        }
        Sleep(1000);
        for (int i = 0; i < 10; i++) {
            driver->FindElement(ByXPath("//*[contains(@data-sigil, 'm-stories-next-button')]")).Click();
            Sleep(2000);
        }
        driver->Navigate("https://m.facebook.com");

    } catch (const std::exception& ex) {
        LOGD << "cant not finl element watchStory";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element watchStory";
    } catch (...) {
        LOGD << "cant not finl element watchStory";
    }
}

void ChromeWorker::changePassword(bool check, QString newPassword)
{
    if(check == true) {
        try {
            QString oldPassword = m_infoClone->getPassword();
            u_sleep(1000);
            driver->Navigate("https://m.facebook.com/settings/security/password/");
            Element currentElement = driver->FindElement(ByName("password_old"));
            currentElement.SendKeys(oldPassword.toStdString());
            u_sleep(1000);
            currentElement = driver->FindElement(ByName("password_new"));
            currentElement.SendKeys(newPassword.toStdString());
            u_sleep(1000);
            currentElement = driver->FindElement(ByName("password_confirm"));
            currentElement.SendKeys(newPassword.toStdString());
            u_sleep(1000);
            driver->FindElement(ByName("save")).Click();
            u_sleep(1000);
            try {
                m_infoClone->setPassWord(newPassword);
                driver->FindElement(ByXPath("//*[contains(text(), 'Stay logged in')]")).Click();
                u_sleep(1000);
                driver->FindElement(ByName("submit_action")).Click();
                u_sleep(3000);
                LOGD << "Change password success from " << oldPassword << " to " << newPassword;
            } catch (const std::exception& ex) {
                LOGD << "cant not finl element";
            } catch (const std::string& ex) {
                LOGD << "cant not finl element";
            } catch (...) {
                LOGD << "cant not finl element";
            }

            driver->Navigate("https://m.facebook.com");
        } catch (const std::exception& ex) {
            LOGD << "cant not finl element";
        } catch (const std::string& ex) {
            LOGD << "cant not finl element";
        } catch (...) {
            LOGD << "cant not finl element";
        }
    }
}

void ChromeWorker::getInfoDataClone()
{
    try {
        m_infoClone->addLog("get info clone");
        driver->Navigate("https://m.facebook.com/" + m_infoClone->getUID().toStdString() + "/about");
        QString mail = "";
        QString birthday = "";
        QString gender = "";
        QString nameAcc = "";
        u_sleep(2000);
        std::vector<Element> elementInfoClone = driver->FindElements(ByClass("_5cdv"));
        if(elementInfoClone.size() >= 3) {
            birthday = elementInfoClone[1].GetText().c_str();
            gender = elementInfoClone[2].GetText().c_str();
            nameAcc = driver->FindElement(ByXPath("//*[contains(@data-sigil, 'MBackNavBarClick')]")).GetText().c_str();
        }
        QString mainDomain = getEmail();
        m_infoClone->setBirthday(birthday);
        m_infoClone->setGender(gender);
        m_infoClone->setNameAcc(nameAcc);
        CONTROLLERMAIN->sigOnchangeReloadTable(m_infoClone->getCloneId());
        updateStatusCloneToServer(m_infoClone->getCloneId(), "birthday", birthday  , "UpdateCloneInfo");
        updateStatusCloneToServer(m_infoClone->getCloneId(), "sex"     , gender    , "UpdateCloneInfo");
        updateStatusCloneToServer(m_infoClone->getCloneId(), "name"    , nameAcc   , "UpdateCloneInfo");

        try {
            if(nameAcc != "") {
                m_infoClone->setMail(mainDomain);
                updateStatusCloneToServer(m_infoClone->getCloneId(), "email"   , mainDomain, "UpdateCloneInfo");
                u_sleep(2000);
//                driver->Navigate("https://m.facebook.com/settings/email/add");
//                u_sleep(2000);
//                m_infoClone->addLog("get add email: " + mainDomain);
//                driver->FindElement(ByName("email")).SendKeys(mainDomain.toStdString());
//                u_sleep(2000);
//                driver->FindElement(ByName("save")).Click();
                u_sleep(2000);
                CONTROLLERMAIN->sigOnchangeReloadTable(m_infoClone->getCloneId());
                //            driver->FindElement(ByXPath("//*[contains(text(), 'Confirm Email Address')]")).Click();
                //            std::string opt = getOPTFromMailDomain(mainDomain);
                //            u_sleep(2000);
                std::string opt = "";
                if(opt != "") {
                    driver->FindElement(ByName("code")).SendKeys(opt);
                    u_sleep(2000);
                    driver->FindElement(ByName("submit")).Click();
                } else {
                    m_infoClone->addLog("get opt from main domain fail");
                }
            }

            u_sleep(3000);
        } catch (const std::exception& ex) {
            m_infoClone->addLog("can't add mail domain");
            LOGD << "cant not add add main";
        } catch (const std::string& ex) {
            m_infoClone->addLog("can't add mail domain");
            LOGD << "cant not add add main";
        } catch (...) {
            m_infoClone->addLog("can't add mail domain");
            LOGD << "cant not add add main";
        }
        u_sleep(2000);
    } catch (const std::exception& ex) {
        LOGD << "cant not finl element in getInfoDataClone";
    } catch (const std::string& ex) {
        LOGD << "cant not finl element in getInfoDataClone";
    } catch (...) {
        LOGD << "cant not finl element in getInfoDataClone";
    }
}

QString ChromeWorker::getEmail()
{
    const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz");
    const int randomStringLength = 10; // assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = rand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }

    QStringList domainList = {
        "@5theway.net",
        "@likeme.click",
        "@buylikefb.com",
        "@bobolala.xyz"
    };

    QString email = randomString.append(QString::number(QRandomGenerator::global()->bounded(1000, 100000))).append(domainList.at(QRandomGenerator::global()->bounded(0, 3)));
    LOGD << "Random email: " << email;
    return email;
}

QString ChromeWorker::getNameFolder()
{
    const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz");
    const int randomStringLength = 10; // assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = rand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

void ChromeWorker::followPageByAPI()
{
    try {
        driver->Navigate("https://m.facebook.com/me");
        u_sleep(3000);
        std::vector<Cookie> getCookies = driver->GetCookies();
        std::string cookie = "";
        for(int i = 0; i < static_cast<int>(getCookies.size()); i++) {
            LOGD << "cookie name------>: " << driver->GetCookies().at(i).name.c_str();
            LOGD << "cookie value------>: " << driver->GetCookies().at(i).value.c_str();
            cookie += driver->GetCookies().at(i).name + "=" + driver->GetCookies().at(i).value;
            cookie += ";";
        }
        LOGD << "cookie cookie: " << cookie.c_str() << "---------end----------";
        m_infoClone->setCookies(cookie.c_str());

        driver->Navigate("https://m.facebook.com/composer/ocelot/async_loader/?publisher=feed&hc_location=ufi");
        Element bodyText = driver->FindElement(ByXPath("/html"));

        QString alltextBody = QString::fromStdString(bodyText.GetText());
        QString fb_dtsg = alltextBody.mid(alltextBody.indexOf("fb_dtsg"), 50);


        QStringList textSlip = fb_dtsg.split("\"");
        for (int i = 0; i < textSlip.size(); i++) {
            LOGD << "tex split: " << textSlip.at(i);
        }
        fb_dtsg = textSlip.at(2);
        fb_dtsg = fb_dtsg.remove(QChar('\\'), Qt::CaseInsensitive);
        fb_dtsg = fb_dtsg.replace( " ", "" );
        LOGD << "get fb_dtsg: " << fb_dtsg.toStdString().c_str() << "-------end-----------";
        QString subscribee_id = m_followPageAPI.getUIDSub();
        QString idPage = m_followPageAPI.getPageID(m_infoClone->getUID());
        driver->Navigate("https://m.facebook.com/" + subscribee_id.toStdString());
        m_followPageAPI.followPage(m_infoClone->getCookies(), m_infoClone->getUserAgent(), fb_dtsg, subscribee_id, idPage);
        Element body = driver->FindElement(ByXPath("/html"));
        body.SendKeys(Shortcut() << keys::Control << "F5");
        u_sleep(5000);
        scrollDown(8);

    } catch (const std::exception& ex) {
        LOGD << "followPageByAPI exception";
    } catch (const std::string& ex) {
        LOGD << "followPageByAPI exception";
    } catch (...) {
        LOGD << "followPageByAPI exception";
    }

}

string ChromeWorker::getOPTFromMailDomain(QString mailDomain)
{
    int count = 0;
    LOGD << "mailDomain: " << mailDomain;
    std::string opt = "";
    while (opt == "" && count <= 5) {
//        opt = WebAPI::getInstance()->getFacebookCodeFromCGBDomainMail(mailDomain.toStdString().c_str());
        LOGD << "OPT mail doamin: " << opt.c_str();
        u_sleep(3000);
        count++;
    }
    return opt;
}


bool ChromeWorker::getCloneToServer()
{
    CommonInforClone* newClone;
    QString UID;
    QString passWord;
    QString haiFa;
    QString idClone;
//    std::string infoClone = WebAPI::getInstance()->getClone();
    std::string infoClone;
    LOGD << "getCloneToServer: " << infoClone.c_str();
    QJsonDocument dataClone = QJsonDocument::fromJson(QString::fromStdString(infoClone).toUtf8());
    QJsonObject getDataClone = dataClone.object();

    UID = getDataClone.value(QString("data")).operator[]("uid").toString();
    passWord = getDataClone.value(QString("data")).operator[]("password").toString();
    haiFa = getDataClone.value(QString("data")).operator[]("secretkey").toString();
    idClone = getDataClone.value(QString("data")).operator[]("id").toString();
    LOGD << "infoClone: " << UID << "|" << passWord << "|" << haiFa << "|" << idClone ;

    if(getDataClone.value(QString("code")).toInt() == 200) {
        newClone = new CommonInforClone(idClone, UID , passWord, haiFa);
        SQLiteWorker::getInstance()->addClone(newClone);
        m_cloneRun = newClone;
        return true;
    } else {
        return false;
    }
}

bool ChromeWorker::closeSession(int tryNumber)
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(QString::fromStdString(driver->GetResource()->GetUrl())));
    QNetworkReply* rep = manager.deleteResource(request);
    bool success = false;
    for(int i =0; i < tryNumber; i++){
        if(success){
            break;
        }
        QObject::connect(rep, &QNetworkReply::finished, [=, &success]{
            if(rep->error() == QNetworkReply::NoError){
                LOGD<<rep->readAll();
                success = true;
            }
            else{
                QString err = rep->errorString();
                LOGD << err;
            }
        });
        QEventLoop loop;
        QObject::connect(rep, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }
    return success;
}

void ChromeWorker::forceCloseChrome()
{
//    delete driver;
    driver->CloseCurrentWindow();
}

void ChromeWorker::updateStatusCloneToServer(QString idClone,QString keyStatus, QString status, QString action)
{
    try {
        QJsonObject rootObj = QJsonDocument::fromJson(m_infoClone->getInfoJsonClone().toUtf8()).object();
        rootObj[keyStatus] = status;
        QJsonDocument cloneJsonDoc(rootObj);
        std::string cloneJsonPathToString(cloneJsonDoc.toJson(QJsonDocument::Compact));
//        m_infoClone->setInfoJsonClone(cloneJsonPathToString.c_str());
        LOGD << "cloneJsonPathToString: " << cloneJsonPathToString.c_str();
        LOGD << "------updateClone----start";
//        WebAPI::getInstance()->updateClone(action.toStdString().c_str(), cloneJsonPathToString.c_str());
        LOGD << "------finish----start";
    } catch (const std::exception& ex) {
        LOGD << "cant not updateStatusCloneToServer[API error]";
    } catch (const std::string& ex) {
        LOGD << "cant not updateStatusCloneToServer[API error]";
    } catch (...) {
        LOGD << "cant not updateStatusCloneToServer[API error]";
    }

}
