#ifndef FDRIVER_DETAIL_HTTP_CONNECTION_H
#define FDRIVER_DETAIL_HTTP_CONNECTION_H

#include "http_client.h"
#include "http_request.h"
#include "error_handling.h"
#include "shared.h"
#include <curl/curl.h>
#include <iostream>

namespace fdriver {
namespace detail {

class HttpConnection // noncopyable
	: public IHttpClient
	, public SharedObjectBase
{
public:
	HttpConnection()
		: connection_(InitCurl())
	{}

	~HttpConnection() {
		curl_easy_cleanup(connection_);
	}

	HttpResponse Get(const std::string& url) const {
#ifdef DEBUG_MODE
        std::cout << "[" << __FUNCTION__ << "]\n[ url: " << url << " ]" << std::endl;
#endif
		return HttpGetRequest(connection_, url).Execute();
	}
	
	HttpResponse Delete(const std::string& url) const {
#ifdef DEBUG_MODE
        std::cout << "[" << __FUNCTION__ << "]\n[ url: " << url << " ]" << std::endl;
#endif
		return HttpDeleteRequest(connection_, url).Execute();
	}
	
	HttpResponse Post(
		const std::string& url,
		const std::string& upload_data
		) const {
#ifdef DEBUG_MODE
        std::cout << "[" << __FUNCTION__ << "]\n[ url: " << url << "]\n[ upload_data: " << upload_data <<" ]" << std::endl;
#endif
		return HttpPostRequest(connection_, url, upload_data).Execute();
	}

private:
	static
	CURL* InitCurl() {
		CURL *const result = curl_easy_init();
		FDRIVER_CHECK(result, "Cannot initialize CURL");
		return result;
	}

private:
	CURL *const connection_;
};

} // namespace detail
} // namespace fdriver

#endif
