#ifndef PHONENUMBERAPI_H
#define PHONENUMBERAPI_H

#include <QString>
#include "log.h"

class PhoneNumberAPI
{
public:
    PhoneNumberAPI();
    ~PhoneNumberAPI();

    QString getPhoneNumber();
    QString getOTP();

private:
    bool m_isReleased;
    QString m_userData;
};

#endif // PHONENUMBERAPI_H
