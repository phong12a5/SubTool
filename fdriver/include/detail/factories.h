#ifndef FDRIVER_DETAIL_FACTORIES_H
#define FDRIVER_DETAIL_FACTORIES_H

#include "shared.h"
#include <string>

namespace fdriver {

class Element;

namespace detail {

class Finder;
class Resource;

struct IFinderFactory {
	virtual Finder MakeFinder(const Shared<Resource>& context) = 0;
	virtual ~IFinderFactory() {}

};

struct IElementFactory {
	virtual Element MakeElement(const std::string& id) = 0;
	virtual ~IElementFactory() {}
};

} // namespace detail
} // namespace fdriver

#endif
