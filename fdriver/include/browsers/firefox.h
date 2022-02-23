#ifndef FDRIVER_BROWSERS_FIREFOX_H
#define FDRIVER_BROWSERS_FIREFOX_H

#include "../capabilities.h"

namespace fdriver {

struct Firefox : Capabilities { // copyable
	Firefox(const Capabilities& defaults = Capabilities())
		: Capabilities(defaults) {
		SetBrowserName(browser::Firefox);
		SetVersion("");
		SetPlatform(platform::Any);
	}

	FDRIVER_PROPERTIES_BEGIN(Firefox)
	// Profile is a profile folder, zipped and base64 encoded.
	// TODO: add FirefoxProfile 
	FDRIVER_PROPERTY(Profile, "firefox_profile", std::string)
	FDRIVER_PROPERTY(LoggingPrefs, "loggingPrefs", LoggingPrefs)
	FDRIVER_PROPERTY(FirefoxBinary, "firefox_binary", std::string)
	FDRIVER_PROPERTIES_END()
};

} // namespace fdriver

#endif
