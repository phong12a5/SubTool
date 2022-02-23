#ifndef FDRIVER_BROWSERS_IE_H
#define FDRIVER_BROWSERS_IE_H

#include "../capabilities.h"

namespace fdriver {
namespace ie {

namespace log_level {
typedef std::string Value;
typedef const char* const ConstValue;
ConstValue Trace = "TRACE";
ConstValue Debug = "DEBUG";
ConstValue Info = "INFO";
ConstValue Warning = "WARN";
ConstValue Error = "ERROR";
ConstValue Fatal = "FATAL";
} // namespace log_level

} // namespace ie

struct InternetExplorer : Capabilities { // copyable
	InternetExplorer(const Capabilities& defaults = Capabilities())
		: Capabilities(defaults) {
		SetBrowserName(browser::InternetExplorer);
		SetVersion("");
		SetPlatform(platform::Any);
	}

	FDRIVER_PROPERTIES_BEGIN(InternetExplorer)
	FDRIVER_PROPERTY(SkipProtectedModeCheck, "ignoreProtectedModeSettings", bool)
	FDRIVER_PROPERTY(IgnoreZoomSetting, "ignoreZoomSetting", bool)
	FDRIVER_PROPERTY(InitialUrl, "initialBrowserUrl", std::string)
	FDRIVER_PROPERTY(EnablePersistentHover, "enablePersistentHover", bool)
	FDRIVER_PROPERTY(EnableElementCacheCleanup, "enableElementCacheCleanup", bool)
	FDRIVER_PROPERTY(RequireWindowFocus, "requireWindowFocus", bool)
	FDRIVER_PROPERTY(BrowserAttachTimeoutMs, "browserAttachTimeout", int)
	FDRIVER_PROPERTY(ForceCreateProcessApi, "ie.forceCreateProcessApi", bool)
	FDRIVER_PROPERTY(CommandLineSwitches, "ie.browserCommandLineSwitches", std::string)
	FDRIVER_PROPERTY(UsePerProcessProxy, "ie.usePerProcessProxy", bool)
	FDRIVER_PROPERTY(EnsureCleanSession, "ie.ensureCleanSession", bool)
	FDRIVER_PROPERTY(LogFile, "logFile", std::string)
	FDRIVER_PROPERTY(LogLevel, "logLevel", ie::log_level::Value)
	FDRIVER_PROPERTY(Host, "host", std::string)
	FDRIVER_PROPERTY(ExtractPath, "extractPath", std::string)
	FDRIVER_PROPERTY(Silent, "silent", bool)
	FDRIVER_PROPERTY(ProxyByServer, "ie.setProxyByServer", bool)
	FDRIVER_PROPERTIES_END()
};

} // namespace fdriver

#endif
