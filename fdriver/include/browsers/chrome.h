#ifndef FDRIVER_BROWSERS_CHROME_H
#define FDRIVER_BROWSERS_CHROME_H

#include "../capabilities.h"

namespace fdriver {
namespace chrome {

struct PerfLoggingPrefs : JsonObject {
	FDRIVER_PROPERTIES_BEGIN(PerfLoggingPrefs)
	FDRIVER_PROPERTY(EnableNetwork, "enableNetwork", bool)
	FDRIVER_PROPERTY(enablePage, "enablePage", bool)
	FDRIVER_PROPERTY(enableTimeline, "enableTimeline", bool)
	FDRIVER_PROPERTY(tracingCategories, "tracingCategories", std::string)
	FDRIVER_PROPERTY(bufferUsageReportingInterval, "bufferUsageReportingInterval", int)
	FDRIVER_PROPERTIES_END()
};

} // namespace chrome

struct Chrome : Capabilities { // copyable
	Chrome(const Capabilities& defaults = Capabilities())
		: Capabilities(defaults) {
		SetBrowserName(browser::Chrome);
		SetVersion("");
		SetPlatform(platform::Any);
	}

	// See https://sites.google.com/a/chromium.org/chromedriver/capabilities for details
	FDRIVER_PROPERTIES_BEGIN(Chrome)
	FDRIVER_PROPERTY(LoggingPrefs, "loggingPrefs", LoggingPrefs)
	FDRIVER_PROPERTY(Args, "args", std::vector<std::string>)
	FDRIVER_PROPERTY(Binary, "binary", std::string)
    FDRIVER_PROPERTY(ChromeOptions, "chromeOptions", JsonObject)

   	// Each extension is a base64-encoded .crx file
	FDRIVER_PROPERTY(Extensions, "extensions", std::vector<std::string>)
	FDRIVER_PROPERTY(LocalState, "localState", JsonObject)
	FDRIVER_PROPERTY(Prefs, "prefs", JsonObject)
	FDRIVER_PROPERTY(Detach, "detach", bool)
	FDRIVER_PROPERTY(DebuggerAddress, "debuggerAddress", std::string)
	FDRIVER_PROPERTY(ExcludeSwitches, "excludeSwitches", std::vector<std::string>)
	FDRIVER_PROPERTY(MinidumpPath, "minidumpPath", std::string)
	FDRIVER_PROPERTY(MobileEmulation, "mobileEmulation", JsonObject)
	FDRIVER_PROPERTY(PerfLoggingPrefs, "perfLoggingPrefs", chrome::PerfLoggingPrefs)
	FDRIVER_PROPERTIES_END()
};

} // namespace fdriver

#endif
