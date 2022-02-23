#ifndef CAPTCHAAPI_H
#define CAPTCHAAPI_H
#include <QString>
#include "log.h"

class captChaAPI
{
public:
    captChaAPI();
    ~captChaAPI();
    QString getId2Captcha(QString dataSideKey);
};

#endif // CAPTCHAAPI_H
