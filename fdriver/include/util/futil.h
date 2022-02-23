#ifndef FUTIL_H
#define FUTIL_H

#include "../fdriver_global.h"
#include <iostream>

namespace fdriver {

class FDRIVER_EXPORT FUtil
{
public:
    static std::string tOTP(const char * secretkey);

};
}

#endif // FUTIL_H
