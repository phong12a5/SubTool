#ifndef FDRIVER_DETAIL_TYPES_H
#define FDRIVER_DETAIL_TYPES_H

#include "../conversions.h"
#include "../capabilities.h"
#include "picojson.h"
#include <string>

namespace fdriver {
namespace detail {

struct SessionRef {
	std::string id;
	Capabilities capabilities;
};

struct ElementRef {
	std::string ref;
};

inline
picojson::value CustomToJson(const ElementRef& ref) {
	return JsonObject()
		.Set("ELEMENT", ref.ref)
		;
}

inline
void CustomFromJson(const picojson::value& value, ElementRef& result) {
	FDRIVER_CHECK(value.is<picojson::object>(), "ElementRef is not an object");
	result.ref = FromJson<std::string>(value.get("ELEMENT"));
}

inline
void CustomFromJson(const picojson::value& value, SessionRef& result) {
	FDRIVER_CHECK(value.is<picojson::object>(), "Session information is not an object");
	result.id = value.get("sessionId").to_str();
	if (value.get("capabilities").is<picojson::object>())
		result.capabilities = Capabilities(value.get("capabilities").get<picojson::object>());
}

} // namespace detail
} // namespace fdriver

#endif
