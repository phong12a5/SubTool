//
// Created by phong.dang on 9/5/2019.
//

#ifndef WEBAPI_H
#define WEBAPI_H

#include <vector>
#ifdef ANDROID
#include <jni.h>
#endif
#include <string>
#include <list>
#include "WebAPI_global.h"

using namespace std;

#ifndef ANDROID
class JNIEnv {};
#endif

class CkJsonObject;

class WEBAPI_EXPORT WebAPI {
private:
    WebAPI();

public:
    enum E_WEBAPI_TEST_MODE: int {
        E_PRODUCTION_MODE = 0,
        E_TEST_MODE,
        E_FULL_TEST_MODE
    };

    static WebAPI* getInstance();

    bool initWebAPIs(JNIEnv* env, const char * token, const char * deviceInfo);
    bool initWebAPIs(JNIEnv* env, const char * token, const char * deviceName, const char * deviceId, const char * appVersionName);

    // Config APIs
    std::string upsertDevice(JNIEnv* env, const char * extraDeviceInfo);
    std::string updateDeviceInfo(JNIEnv* env, const char * extraDeviceInfo);
    std::string getConfig(JNIEnv* env);
    std::string getClone(JNIEnv* env, const char * appName);
    std::string getCloneInfo(JNIEnv* env, const char *appName, const char * clone_info);
    std::string getStoredClones(JNIEnv* env, const char *appName);
    std::string updateClone(JNIEnv* env, const char * action, const char *appName, const char * cloneJsonPath);
    std::string getJasmineDefinitions(JNIEnv* env);
    std::string getVer3Definitions(JNIEnv* env);
    std::string submitActiveClones(JNIEnv* env, const char * appName, const char * activeClones);

    // Get actions APIs
    std::string doAction(JNIEnv* env, const char *appName, const char * clone_id);

    // Submit Result APIs
    bool doResult(JNIEnv* env, const char *appName, const char * clone_id, const char * dataJsonPath);

    // Dropbox APIs
    static bool downLoadFile(const char * urlImage, const char * outputPah);
    bool downloadFileFromDropbox(JNIEnv* env, const char* pathFile, const char* savePath);
    static bool downloadFile(std::string& url, const std::string& savedPath);
    std::string getFolderContent(JNIEnv* env, const char * folderPath);

    // Hotmail APIs
    std::string getHotmail(JNIEnv* env);
    std::string getFacebookCodeFromCGBDomainMail(JNIEnv* env, const char * email) const;
    std::string getTiktokCodeFromCGBDomainMail(JNIEnv *env, const char * email) const;
    std::string getFacebookCodeFromHotmail(JNIEnv* env, const char * email, const char * password) const;
    std::string getTiktokCodeFromHotmail(JNIEnv *env, const char * email, const char * password) const;
    bool        checkLoginHotmail(JNIEnv* env, std::string& email, std::string& password) const;

    static std::string tOTP(const char * secretkey);


private:
    bool initState() const;
    void setInitState(bool state);
    bool getDropboxToken(JNIEnv* env, std::string &dropboxToken);
    std::string getDomain();
    void setDomain(std::string newDomain);
    std::string getUrlByAPI(std::string api);
    static const char * getKey();
    static const char * getIv();
    static bool unlockChilkat();
#ifdef ANDROID
    static bool makeDir(const char * folderName);
#endif
    bool encryptCloneInfo(JNIEnv* env, std::string& cloneInfo);
    bool decryptCloneInfo(JNIEnv* env, std::string& cloneInfo);
    bool sendRequest(JNIEnv* env, const char * caller, CkJsonObject& data, CkJsonObject& response, const char * api, const char * extraDeviceInfo = nullptr);
    std::string getFacebookCodeFromImap(JNIEnv* env, const char * imapServer, int port, const char * mailBox, const char* toEmail, const char * login_email, const char * login_password) const;
    std::string getTiktokCodeFromImap(JNIEnv* env, const char * imapServer, int port, const char * mailBox, const char* toEmail, const char * login_email, const char * login_password) const;
    std::string token() const;
    std::string deviceInfo() const;
    void pLog(JNIEnv* env, int level, const char * TAG, const char * fmt, ...) const;
    static void pushLog2Server(JNIEnv* env,const char* level, const char * TAG, const char * msg);
#ifdef ANDROID
    static jobject getGlobalContext(JNIEnv* env);
#endif
private:
    bool m_initState;
    bool m_unlockState;
    std::string m_token;
    std::string m_deviceInfo;
    std::string m_dropBoxToken;
    int         m_debugMode;
    std::string m_domain;
    std::list<std::string> m_existedPackagedList;
    std::vector<std::string> m_listKey;
    std::vector<std::string> m_listValue;
};



#endif //WEBAPI_H
