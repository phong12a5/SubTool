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

    std::vector<std::string> args;
    args.push_back( "--user-data-dir=" + serviceData()->profilePath().toStdString());
    args.push_back("--ignore-certificate-errors");
//    args.push_back("--proxy-server= " + serviceData()->getProxy().toStdString());

    if(serviceData()->cloneInfo()->userAgent().isEmpty()) {
        serviceData()->cloneInfo()->setUserAgent(getRandomUserAgent());
    }
    args.push_back("--disable-notifications");
    chromeOptions.Set<std::vector<std::string>>("args",args);

    std::vector<std::string> switches;
    switches.push_back("enable-automation");
    switches.push_back("load-extension");
    chromeOptions.Set<std::vector<std::string>>("excludeSwitches",switches);

    picojson::value sourceJson = static_cast<picojson::value>(JsonObject().\
                                                              Set("intl.accept_languages", "en,en_US").\
                                                                Set("profile.password_manager_enabled", false).\
                                                                Set("credentials_enable_service", false));
    chromeOptions.Set<picojson::value>("prefs",sourceJson);
    chrome.SetChromeOptions(chromeOptions);

    driver = new FDriver(chrome);
    driver->Execute("Object.defineProperty(navigator, 'webdriver', {get: () => undefined})");

//    driver->Navigate("https://httpbin.org/headers");
    driver->Navigate("https://m.facebook.com");
}

void ChromeService::getProxy()
{
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

void ChromeService::onStarted()
{
    LOGD;
    setServiceData(new ServiceData(BaseService::TYPE_CHROME_SERVICE, m_profileId));
    startMainProcess();
}

void ChromeService::onMainProcess()
{
    LOGD;
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
            if(url.contains("what's-new")) {
                driver->CloseCurrentWindow();
            }
            if(ElementExist(ByXPath("//*[contains(@data-sigil, 'm_login_email')]")) ||
                    ElementExist(ById("approvals_code"))) {
                login();
            } else if(ElementExist(ByXPath("//*[contains(@value, 'save_device')]")) &&
                      ElementExist(ByXPath("//*[contains(@value, 'dont_save')]"))) {
                click(ByName("submit[Continue]"));
            } else if(url.contains("%2Fcheckpoint%2F") ||
                      url.contains("282/")) {
                LOGD << "CHECKPOINT";
                serviceData()->cloneInfo()->setAliveStatus(CLONE_ALIVE_STATUS_CHECKPOINT);
                finishLifecycle();
            } else if(ElementExist(ById("m_news_feed_stream"))) {
                LOGD << "NEW FEED SCREEN";
                serviceData()->cloneInfo()->setAliveStatus(CLONE_ALIVE_STATUS_STORE);
            }
        }
    }
}
