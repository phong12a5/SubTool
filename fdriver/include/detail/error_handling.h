#ifndef FDRIVER_DETAIL_ERROR_HANDLING_H
#define FDRIVER_DETAIL_ERROR_HANDLING_H

#include "../errors.h"
#include <string>
#include <sstream>

namespace fdriver {
namespace detail {

class Fmt {
public:
	template<typename T>
	Fmt& operator << (const T& value) {
		stream_ << value;
		return *this;
	}

	operator std::string() const {
		return stream_.str();
	}

private:
	std::ostringstream stream_;
};

template<typename T>
bool BoolCast(T value) {
	return !!value;
}

} // namespace detail
} // namespace fdriver

#if __cplusplus >= 201103L
    #define FDRIVER_CURRENT_FUNCTION __func__
#else
    #define FDRIVER_CURRENT_FUNCTION __FUNCTION__
#endif

#define FDRIVER_FUNCTION_CONTEXT_BEGIN() \
	try {

#define FDRIVER_FUNCTION_CONTEXT_END() \
	} catch (const std::exception& e) { \
        throw ::fdriver::FDriverException(std::string(e.what()) \
            + " called from " + FDRIVER_CURRENT_FUNCTION); \
	}

#define FDRIVER_FUNCTION_CONTEXT_END_EX(details) \
	} catch (const std::exception& e) { \
        throw ::fdriver::FDriverException(std::string(e.what()) \
            + " called from " + FDRIVER_CURRENT_FUNCTION \
			+ " (" + std::string(details) + ")"); \
	}

#define FDRIVER_THROW(message) \
    throw ::fdriver::FDriverException(::fdriver::detail::Fmt() \
		<< std::string(message) \
		<< " at line " << __LINE__ \
		<< ", file " << __FILE__ \
		)

#define FDRIVER_CHECK(pred, message) \
	for (;!detail::BoolCast(pred);) \
        FDRIVER_THROW(message)

#endif
