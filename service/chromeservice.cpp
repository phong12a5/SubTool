#include "chromeservice.h"
#include "model/servicedata.h"
#include <QTimer>
#include <WebAPI.hpp>
#include <CkHttp.h>
#include <CkJsonObject.h>
#include <QJsonArray>
#include <QFile>
#include <QProcess>
#include <utils.h>
#include <regex>
#include <CkHttp.h>
#include <CkHttpRequest.h>
#include <CkHttpResponse.h>
#include <QRandomGenerator>
#include <appmodel.h>
#include <QDir>

QString getRandomUserAgent()
{
    QString userAgen;
    QFile file;
    file.setFileName(":/model/userAgent.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    userAgen = file.readAll();
    file.close();

    QJsonDocument jsonDocUserAgen = QJsonDocument::fromJson(userAgen.toUtf8());
    QJsonObject jsonUserAgent = jsonDocUserAgen.object();
    QJsonValue infoUserAgent = jsonUserAgent.value(QString("useragent"));
    QJsonArray listUserAgen = infoUserAgent.toArray();
    int leng = infoUserAgent.toArray().size() - 1;
    srand(time(NULL));
    int res = rand() % leng;
    return listUserAgen.at(res).toString();
}

ChromeService::ChromeService(int profileId, QObject *parent) :
    BaseService(SERVICE_TYPE::TYPE_CHROME_SERVICE,profileId, parent)
{
    LOGD << "--------- ChromeService: " << profileId << " ---------";
}

ChromeService::~ChromeService()
{
//    LOGD;
}

void ChromeService::connectSignalSlots()
{
//    LOGD;
}

ServiceData *ChromeService::model()
{
    return m_service_data;
}

void ChromeService::initChromeDriver()
{
    Chrome chrome;
    JsonObject chromeOptions;
    chrome.SetPlatform(platform::Mac);

    std::vector<std::string> args;
    args.push_back("--user-data-dir=" + serviceData()->profilePath().toStdString());
    args.push_back("--ignore-certificate-errors");
    args.push_back("--proxy-server=http://" + serviceData()->getProxy().toStdString());
    args.push_back("--disable-features=ChromeWhatsNewUI");
#if 0
    if(serviceData()->cloneInfo()->userAgent().isEmpty()) {
        serviceData()->cloneInfo()->setUserAgent(getRandomUserAgent());
    }
#endif

    args.push_back("--disable-notifications");
    args.push_back("--window-position=1500,0");
    chromeOptions.Set<std::vector<std::string>>("args",args);

    std::vector<std::string> switches;
    switches.push_back("enable-automation");
    switches.push_back("load-extension");
    chromeOptions.Set<std::vector<std::string>>("excludeSwitches",switches);



    JsonObject sourceJson = JsonObject();
    sourceJson.Set("intl.accept_languages", "en,en_US");
    sourceJson.Set("profile.password_manager_enabled", false);
    sourceJson.Set("credentials_enable_service", false);
    chromeOptions.Set<picojson::value>("prefs", static_cast<picojson::value>(sourceJson));

#if 0
    JsonObject mobileEmulation = JsonObject();
    mobileEmulation.Set("deviceName", "iPhone 8 Plus");
    chromeOptions.Set<picojson::value>("mobileEmulation",static_cast<picojson::value>(mobileEmulation));
#endif

    chrome.SetChromeOptions(chromeOptions);

    driver = new FDriver(chrome);

    int width = 375;
    int height = 812;

    static int max_width = 0, max_height = 0;
    if(max_width == 0 || max_height == 0) {
        Window window = driver->GetCurrentWindow().Maximize();
        max_width = window.GetSize().width;
        max_height = window.GetSize().height;
    }
    Point point = Point(QRandomGenerator::global()->bounded(max_width - width), 0);;
    driver->GetCurrentWindow().SetPosition(point);
    Size size; size.width = width; size.height = height;
    driver->GetCurrentWindow().SetSize(size);
    driver->Execute("Object.defineProperty(navigator, 'webdriver', {get: () => undefined})");

    driver->Navigate("https://m.facebook.com");
}

void ChromeService::getProxy()
{
#if 0
    CkHttp http;

    // Set the Login and Password properties for authentication.
    http.put_Login("admin");
    http.put_Password("w2Yt4b2B8xWnhoa");

    // To use HTTP Basic authentication..
    http.put_BasicAuth(true);

    const char *result = http.quickGetStr("https://proxy.autofarmer.net/public-api/v1/get-vpn");
    LOGD << "result: " << result;
    if (http.get_LastMethodSuccess() != true) {
        std::cout << http.lastErrorText() << "\r\n";
        return;
    }

    if(result) {
        CkJsonObject jsonResult;
        if(jsonResult.Load(result) && jsonResult.HasMember("data")) {
            CkJsonObject* data = jsonResult.ObjectOf("data");
            LOGD << (data && data->HasMember("vpn"));
            if(data && data->HasMember("vpn")) {
                QString proxy = "140.82.45.238:8899";//data->stringOf("vpn");
                QStringList params = proxy.trimmed().split(":");
                if(params.length() == 2) {
                    QString ip = params[0];
                    if(ping(ip)) {
                        serviceData()->setProxy(proxy);
                        return;
                    }
                }
            }
        }
    }
#else
    QString ip = "45.77.128.27";
    int port = QRandomGenerator::global()->bounded(10000, 11000);
    QString cgbProxy = ip + ":" + QString::number(port);
    if(checkProxy(ip, port)) {
        serviceData()->setProxy(cgbProxy);
    }
#endif
}

void ChromeService::getClone()
{
    std::string result = WebAPI::getInstance()->getClone(nullptr, "facebook");
    QJsonObject cloneInfo = QJsonDocument::fromJson(result.c_str()).object().value("cloneInfo").toObject();
    if(!cloneInfo.isEmpty()) {
        serviceData()->setCloneInfo(new CloneInfo(cloneInfo));
        if(serviceData()->cloneInfo()) {
            serviceData()->cloneInfo()->setAliveStatus("stored");
        }
    }
}

void ChromeService::login()
{
    try {
        QString uid = serviceData()->cloneInfo()->uid();
        QString password = serviceData()->cloneInfo()->password();

        Element emailElement = driver->FindElement(ByXPath("//*[contains(@data-sigil, 'm_login_email')]"));
        if(uid != QString(emailElement.GetAttribute("value").c_str())) {
             inputText(serviceData()->cloneInfo()->uid(),ByXPath("//*[contains(@data-sigil, 'm_login_email')]"));
             delay(random(500, 1000));
        }

        Element passwordElement = driver->FindElement(ByXPath("//*[contains(@data-sigil, 'm_login_email')]"));
        if(password != QString(passwordElement.GetAttribute("value").c_str())) {
            inputText(serviceData()->cloneInfo()->password(),ByXPath("//*[contains(@data-sigil, 'password-plain-text-toggle-input')]"));
            delay(random(500, 1000));
        }

        if(ElementExist(ByXPath("//*[contains(@data-sigil, 'touchable login_button_block m_login_button')]"))) {
            click(ByXPath("//*[contains(@data-sigil, 'touchable login_button_block m_login_button')]"));
            delay(5000);
        }

        if(ElementExist(ById("approvals_code"))) {
            QString secretkey = serviceData()->cloneInfo()->secretkey();
            if(secretkey.isEmpty()) {
                return;
            } else {
                delay(random(1000, 2000));
                inputText(WebAPI::getInstance()->tOTP(secretkey.toUtf8().data()).c_str()\
                      ,ById("approvals_code"));
                delay(random(500,1000));
                click(ById("checkpointSubmitButton-actual-button"));
            }
        }
    } catch(...) {
        LOGD << "m_login_email not found";
    }
}

void ChromeService::followByPage()
{
    LOGD;
    if(serviceData()->cloneInfo()->property("fb_dtsg").toString().isEmpty()) {
        std::string source = driver->Get("https://m.facebook.com/composer/ocelot/async_loader/?publisher=feed&hc_location=ufi").GetSource();
        std::string fb_dtsg;
        std::string regx = R"(name=\\\"fb_dtsg\\\" value=\\\"([\s\S]*?)\\\")";
        std::smatch matches;
        if (std::regex_search(source, matches, std::regex(regx)))
        {
            fb_dtsg = matches[1];
            serviceData()->cloneInfo()->setProperty("fb_dtsg", QVariant(fb_dtsg.c_str()));
        }
        driver->Back();
    } else {
        QString targetUid = "104665758145747";
        QString fb_dtsg = serviceData()->cloneInfo()->property("fb_dtsg").toString();
        QString pageId = "106451348651403";
        std::vector<Cookie> cookies = driver->GetCookies();
        std::string cookiesStr;
        foreach(Cookie cookie , cookies) {
            std::string cookieStr;
            cookieStr += cookie.name + "=" + cookie.value + ";";
            cookiesStr += cookieStr;
        }
        LOGD << "cookieStr: " << cookiesStr.c_str();


        CkHttp http;
        CkHttpRequest req;
        req.put_HttpVerb("POST");
        req.put_Path("/api/graphql");
        req.put_ContentType("multipart/form-data;charset=utf-8");

        req.AddHeader("authority", "www.facebook.com");
        req.AddHeader("sec-ch-ua", "\" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"");
        req.AddHeader("accept", "application/json, text/plain, */*");
        req.AddHeader("sec-ch-ua-mobile", "?0");
        req.AddHeader("user-agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36");
        req.AddHeader("sec-ch-ua-platform", "\"Windows\"");
        req.AddHeader("origin", "https://www.facebook.com");
        req.AddHeader("sec-fetch-site", "none");
        req.AddHeader("sec-fetch-mode", "navigate");
        req.AddHeader("sec-fetch-dest", "document");
        req.AddHeader("accept-language", "en-US,en;q=0.9,vi;q=0.8");
        req.AddHeader("cookie", cookiesStr.c_str());

        req.AddParam("fb_api_analytics_tags", "[\"qpl_active_flow_ids=30605361,431626192\"]");
        req.AddParam("doc_id", "4451435638222552");
        req.AddParam("fb_api_req_friendly_name", "CometUserFollowMutation");
        req.AddParam("fb_dtsg", fb_dtsg.toUtf8().data());

        QString variable = QString("{\"input\":{\"subscribe_location\":\"PROFILE\",\"subscribee_id\":\"%1\",\"actor_id\":\"%2\",\"client_mutation_id\":\"16\"},\"scale\":1.5}").arg(targetUid).arg(pageId);
        req.AddParam("variables", variable.toUtf8().data());

        CkHttpResponse *resp = http.PostUrlEncoded("https://www.facebook.com/api/graphql",req);
        if (http.get_LastMethodSuccess() == false) {
            LOGD << "error: " << http.lastErrorText();
            return;
        } else {
             QJsonObject respObj = QJsonDocument::fromJson(resp->bodyStr()).object();
             LOGD << "body: " << resp->bodyStr();
             if(!respObj.isEmpty() && !respObj.contains("error") && !respObj.value("actor_subscribe").toObject().isEmpty()) {
                 LOGD << "Follow success";
             } else {
                 LOGD << "Follow succeded";
                 delay(10000);
             }
        }
    }
}

bool ChromeService::checkProxy(QString ip, int port)
{
    CkHttp http;
    http.put_ProxyDomain(ip.toUtf8().data());
    http.put_ProxyPort(port);
    const char * html = http.quickGetStr("https://www.google.com.vn/");
    if(html) {
        return html;
    } else {
        LOGD << "proxy died";
        return false;
    }
}

bool ChromeService::getInviteLink(QString& data, QString uid)
{
    CkHttp http;
    QString url = QString("https://api.pagesub.me/public-api/v1/clone/get-invite?uid=%1").arg(uid);
    const char * html = http.quickGetStr(url.toUtf8().data());
    if(html) {
        return html;
    } else {
        LOGD << "proxy died";
        return false;
    }
}

void ChromeService::onStarted()
{
    LOGD;
    setServiceData(new ServiceData(BaseService::TYPE_CHROME_SERVICE, m_profileId));
    if(serviceData()->cloneInfo() == nullptr) {
        QDir dir(serviceData()->profilePath());
        dir.removeRecursively();
    }
    startMainProcess();
}

void ChromeService::onMainProcess()
{
    LOGD;
    try {
        if(serviceData()->getProxy().isEmpty()) {
            // get proxy first
            getProxy();
        } else if(serviceData()->cloneInfo() == nullptr) {
            getClone();
        } else {
            if(driver == Q_NULLPTR) {
                initChromeDriver();
            } else {
                QString url = driver->GetUrl().c_str();
                LOGD << "url: " << url;
                if(url == "chrome://whats-new") {
                    driver->CloseCurrentWindow();
                }

                if(ElementExist(ByXPath("//*[contains(@data-sigil, 'm_login_email')]")) ||
                        ElementExist(ById("approvals_code"))) {
                    login();
                } else if(ElementExist(ByXPath("//*[contains(@value, 'save_device')]")) &&
                          ElementExist(ByXPath("//*[contains(@value, 'dont_save')]"))) {
                    click(ByName("submit[Continue]"));
                } else if(ElementExist(ByXPath("//*[contains(@href, '/a/nux/wizard/nav.php?step=homescreen_shortcut&skip')]"))) {
                    click(ByXPath("//*[contains(@href, '/a/nux/wizard/nav.php?step=homescreen_shortcut&skip')]"));
                } else if(url.contains("%2Fcheckpoint%2F") ||
                          url.contains("282/")) {
                    LOGD << "CHECKPOINT";
                    serviceData()->cloneInfo()->setAliveStatus(CLONE_ALIVE_STATUS_CHECKPOINT);
                    driver->DeleteCookies();
                    finish();
                } else if(ElementExist(ById("m_news_feed_stream"))) {
                    LOGD << "NEW FEED SCREEN";
                    serviceData()->cloneInfo()->setAliveStatus(CLONE_ALIVE_STATUS_STORE);
                    finish();
    //                followByPage();
                }
            }
        }
    } catch(...) {

    }
}
