//
// Created by phong.dang on 9/5/2019.
//

#ifndef WEBAPI_H
#define WEBAPI_H

#include <vector>
#include <list>
#include <map>
#include <string.h>
#include <iostream>
#include "WebAPI_global.h"

class CkJsonObject;

typedef struct device_info{
    char * device_id = nullptr;
    char * app_version_name = nullptr;

    device_info &operator=(device_info& obj) {
        device_id = new char[strlen(obj.device_id) + 1];
        strcpy(this->device_id, obj.device_id);
        app_version_name = new char[strlen(obj.app_version_name) + 1];
        strcpy(this->app_version_name, obj.app_version_name);
        return *this;
    }
} DEVICE_INFO;

class WEBAPI_EXPORT WebAPI {
private:
    WebAPI();

public:
    enum E_SUPPORTED_PLATFORM: int {
        PLATFORM_UNKNOWN = 0,
        PLATFORM_F_CARE,
        PLATFORM_F_SYSTEM,
        PLATFORM_F_ANDROID,
        PLATFORM_F_ANDROID_WEBVIEW,
        PLATFORM_F_IOS,
        PLATFORM_F_IOS_WEBVIEW
    };


    /** */
    static WebAPI* getInstance();

    /**
     * @author phong.dang
     * @brief obtain lib version
     * @return std::string
     */
    static std::string version();

    /**
     * @brief WebAPI must be initialized before using
     *
     * @param token
     * @param deviceInfo -> example:
    /// code
    DEVICE_INFO deviceInfo;
    deviceInfo.device_id = (char*)"123456789";
    deviceInfo.app_version_name = (char*)"0.0.1";
    /// code
     * @return true
     * @return false
     */
    bool initWebAPIs(const char * token, DEVICE_INFO& deviceInfo);

    // Config APIs
    /**
     * @brief register this device to server, webapi is availble when this device is approved from website
     *
     * @param extraDeviceInfo: at present, this param is unused
     * @return std::string
     */
    std::string upsertDevice(const char * extraDeviceInfo = nullptr);
    /**
     * @brief update status of this device to server
     * NOTE: at present, this api is just for fun
     * @param extraDeviceInfo: at present, this param is unused
     * @return std::string
     */
    std::string updateDeviceInfo(const char * extraDeviceInfo = nullptr);
    /**
     * @brief Get the Configuration of this device from server
     *
     * @return std::string
     */
    std::string getConfig();
    /**
     * @brief Get the Clone live from server
     *
     * @return std::string
     */
    std::string getClone();
    /**
     * @brief Get the full Clone Info
     *
     * @param cloneJsonPath: example: cloneJsonPath: {
            "alive_status": "stored",
            "appname": "facebook",
            "id": "614ca178658a2ec8c04e67ae",
            "uid": "1000485734857394",
            "password": "phong1994"
        }
     * @return std::string
     */
    std::string getCloneInfo(const char * cloneJsonPath);

    /**
     * @brief Get the Stored Clones in this device
     *
     * @return std::string
     */
    std::string getStoredClones();
    /**
     * @brief update clone info to server
     *
     * @param action
     * @param cloneJsonPath: example: cloneJsonPath: {
            "alive_status": "stored",
            "appname": "facebook",
            "id": "614ca178658a2ec8c04e67ae",
            "uid": "1000485734857394",
            "password": "phong1994"
        }
     * @return std::string
     */
    std::string updateClone(const char * action, const char * cloneJsonPath);

    /**
     * @brief Get the Jasmine Definitions object
     * @note this api is only for f_phone
     * @return std::string
     */
    std::string getJasmineDefinitions();

    /**
     * @brief submit clone ids which will open:
     * @example: stored clones:
     *  clone1_JsonPath: {
            "alive_status": "live",
            "appname": "facebook",
            "id": "614ca178658a2ec8c04e67ae",
            "uid": "1000485734857394",
            "password": "phong1994"
        }
        clone2_JsonPath: {
            "alive_status": "live",
            "appname": "facebook",
            "id": "32483hfuty9823798hd234556345",
            "uid": "1000485734857394",
            "password": "phong1994"
        }
     * @example: submitActiveClones("[614ca178658a2ec8c04e67ae, 32483hfuty9823798hd234556345]");
     * @param activeClones
     * @return std::string
     */
    std::string submitActiveClones(const char * activeClones);

    // Get actions APIs
    /**
     * @brief obtain actions for clone_id from server
     * @example:
        cloneJsonPath: {
            "alive_status": "live",
            "appname": "facebook",
            "id": "614ca178658a2ec8c04e67ae",
            "uid": "1000485734857394",
            "password": "phong1994"
        }
     * @param clone_id: example: 614ca178658a2ec8c04e67ae
     * @return std::string
     */
    std::string doAction(const char * clone_id);

    // Submit Result APIs
    /**
     * @brief submit task status to server
     * @example:
        cloneJsonPath: {
            "alive_status": "live",
            "appname": "facebook",
            "id": "614ca178658a2ec8c04e67ae",
            "uid": "1000485734857394",
            "password": "phong1994"
        }
     * @param clone_id : example: 614ca178658a2ec8c04e67ae
     * @param dataJsonPath: example: {"service_code":"XXXXX", "action":"FeedLike"}
     * @return std::string
     */
    std::string doResult(const char * clone_id, const char * dataJsonPath);

    // Dropbox APIs
    /**
     * @brief download file from dropbox
     *
     * @param pathFile : cloud file path
     * @param savePath : local file path
     * @return true if succeed | false if failed
     */
    bool downloadFileFromDropbox(const char* pathFile, const char* savePath);
    /**
     * @brief Get the Folder Content in dropbox
     *
     * @param folderPath
     * @return std::string
     */
    std::string getFolderContent(const char * folderPath);

    // Hotmail APIs
    /**
     * @brief Get the Hotmail object
     *
     * @return std::string
     */
    std::string getHotmail();

    /**
     * @brief Check hotmail live/died
     *
     * @param email
     * @param password
     * @return true
     * @return false
     */
    bool        checkLoginHotmail(std::string& email, std::string& password) const;

    /**
     * @brief Get the Facebook Code From CGB Domain Mail
     *
     * @param email
     * @return std::string
     */
    std::string getFacebookCodeFromCGBDomainMail(const char * email, const char * mailbox = "Spam") const;
    /**
     * @brief Get the Tiktok Code From CGB Domain Mail
     *
     * @param email
     * @return std::string
     */
    std::string getTiktokCodeFromCGBDomainMail(const char * email) const;

    /**
     * @brief Get the Facebook Code From Hotmail
     *
     * @param email
     * @param password
     * @return std::string
     */
    std::string getFacebookCodeFromHotmail(const char * email, const char * password, const char * mailbox = "Inbox") const;

    /**
     * @brief Get the Tiktok Code From Hotmail
     *
     * @param email
     * @param password
     * @return std::string
     */
    std::string getTiktokCodeFromHotmail(const char * email, const char * password) const;

    //util
    /**
     * @brief obtain tOPT
     *
     * @param secretkey
     * @return std::string
     */
    static std::string tOTP(const char * secretkey);

    /**
     * @brief download file by quickly GET request
     *
     * @param url
     * @param savedPath
     * @return true
     * @return false
     */
    static bool downloadFile(std::string& url, const std::string& savedPath);


private:
    bool initState() const;
    void setInitState(bool state);
    bool getDropboxToken(std::string &dropboxToken);
    std::string getDomain();
    std::string getUrlByAPI(std::string api);
    static const char * getKey();
    static const char * getIv();
    static bool unlockChilkat();
    bool encryptCloneInfo(std::string& cloneInfo);
    bool decryptCloneInfo(std::string& cloneInfo);
    bool sendRequest(const char * caller, CkJsonObject& data, CkJsonObject& response, const char * api, const char * extraDeviceInfo = nullptr);
    std::string getCodeFromImap(const char * imapServer, int port, const char * mailBox, const char * fromName, const char* toEmail, const char * login_email, const char * login_password) const;
    std::string token() const;
    DEVICE_INFO deviceInfo() const;
    void pLog(int level, const char * TAG, const char * fmt, ...) const;
    static void pushLog2Server(const char* level, const char * TAG, const char * msg);

private:
    bool m_initState;
    bool m_unlockState;
    std::string m_token;
    E_SUPPORTED_PLATFORM m_system_type;
    DEVICE_INFO m_deviceInfo;
    std::string m_dropBoxToken;
    std::string m_domain;
    std::list<std::string> m_existedPackagedList;
    std::vector<std::string> m_listKey;
    std::vector<std::string> m_listValue;
};



#endif //WEBAPI_H
