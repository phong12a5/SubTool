#ifndef FDRIVER_DETAIL_FINDER_H
#define FDRIVER_DETAIL_FINDER_H

#include "shared.h"
#include "resource.h"
#include "factories.h"
#include "../by.h"
#include <vector>
#include "../fdriver_global.h"

namespace fdriver {

class Element;

namespace detail {

class FDRIVER_EXPORT Finder { // copyable
public:
    Finder(
        const Shared<Resource>& context,
        const Shared<IElementFactory>& factory
        )
        : context_(context)
        , factory_(factory)
    {}

	Element FindElement(const By& by) const;
	std::vector<Element> FindElements(const By& by) const;

private:
	Shared<Resource> context_;
	Shared<IElementFactory> factory_;
};

} // namespace detail
} // namespace fdriver

#endif
