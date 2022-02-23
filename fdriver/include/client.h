#ifndef FDRIVER_CLIENT_H
#define FDRIVER_CLIENT_H

#include "session.h"
#include "capabilities.h"
#include "detail/resource.h"
#include "detail/http_connection.h"
#include "detail/picojson.h"
#include <string>
#include <vector>
#include "fdriver_global.h"

namespace fdriver {

const char *const kDefaultWebDriverUrl = "http://localhost:9515";

// Gives low level access to server's resources. You normally should not use it. 
class FDRIVER_EXPORT Client { // copyable
public:
	explicit Client(const std::string& url = kDefaultWebDriverUrl);
	virtual ~Client() {}

    picojson::object GetStatus() const;

	// Returns existing sessions.
    std::vector<Session> GetSessions() const;

	// Creates new session.
    Session CreateSession(
		const Capabilities& desired,
		const Capabilities& required
		) const;

private:
    Session MakeSession(
		const std::string& id,
		detail::Resource::Ownership mode
		) const;

private:
	detail::Shared<detail::Resource> resource_;
};

} // namespace fdriver

//#include "client.inl"

#endif
