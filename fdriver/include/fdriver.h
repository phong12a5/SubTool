#ifndef FDRIVER_FDRIVER_H
#define FDRIVER_FDRIVER_H

#include "client.h"
#include "session.h"

namespace fdriver {

// The main class for interactions with a server. Automatically connects to a server,
// creates and deletes a session and gives access to session's API.
class FDRIVER_EXPORT FDriver // copyable
        : public Client
        , public Session {
public:
    explicit FDriver(
            const Capabilities& desired = Capabilities(),
            const Capabilities& required = Capabilities(),
            const std::string& url = kDefaultWebDriverUrl
            );

    static bool unlockFDriver(const char* token);

    static FDriver Start(
            const Capabilities& desired,
            const Capabilities& required = Capabilities(),
            const std::string& url = kDefaultWebDriverUrl
            );

    static FDriver Start(const Capabilities& desired,
                         const std::string& url);
};
} // namespace fdriver

#endif
