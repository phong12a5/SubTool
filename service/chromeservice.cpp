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

ChromeService::ChromeService(QObject *parent) :
    BaseService(SERVICE_TYPE::TYPE_CHROME_SERVICE, parent)
{
//    LOGD << "--------- ChromeService ---------";
//    m_worker->setSerivceData(m_service_data);
    connectSignalSlots();
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
//    args.push_back("--user-agent=" + serviceData()->cloneInfo()->userAgent().toStdString());
    args.push_back("--user-agent=Mozilla/5.0 (Linux; Android 8.1.0; SM-G610F Build/M1AJQ) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/87.0.4280.66 Mobile Safari/537.36");
    args.push_back("--lang=en");
    args.push_back("--disable-blink-features=AutomationControlled");
    args.push_back("--no-sandbox");
    args.push_back("user-agent=#{linux_useragent}");
    args.push_back("--disable-web-security");
    args.push_back("--disable-xss-auditor");
//    args.push_back("excludeSwitches", ["enable-automation", "load-extension"])
    //--lang=es
    args.push_back("--disable-notifications");
    chromeOptions.Set<std::vector<std::string>>("args",args);
    chrome.SetChromeOptions(chromeOptions);

    driver = new FDriver(chrome);
//    picojson::value sourceJson = static_cast<picojson::value>(JsonObject()
//                                                              .Set("source", "\
//                                                                   Object.defineProperty(navigator, 'webdriver', {\
//                                                                     get: () => undefined\
//                                                                   }),\
//                                                                   Object.defineProperty(navigator, 'languages', {\
//                                                                     get: () => ['en-US', 'en']\
//                                                                   }),\
//                                                                   Object.defineProperty(navigator, 'platform', {\
//                                                                     get: () => \"Android\"\
//                                                                   })"));
//    driver->Execute("Page.addScriptToEvaluateOnNewDocument", JsArgs() << sourceJson);

    driver->Navigate("http://m.facebook.com");
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
            if(ElementExist(ByXPath("//*[contains(@data-sigil, 'm_login_email')]")) ||
                    ElementExist(ById("approvals_code"))) {
                login();
            } else if(ElementExist(ByXPath("*[contains(text(), 'Nhớ trình duyệt')]")) &&
                      ElementExist(ByXPath("*[contains(text(), 'Lưu trình duyệt')]"))) {
                click(ByName("submit[Continue]"));
            }
        }

    }
}
