#include "include.h"
#include <string.h>
#include "init.h"

#ifdef _WIN32
#ifdef SSL
#define CURL_GLOBAL_FLAG CURL_GLOBAL_ALL
#else
#define CURL_GLOBAL_FLAG CURL_GLOBAL_WIN32
#endif
#else
#ifdef SSL
#define CURL_GLOBAL_FLAG CURL_GLOBAL_SSL
#else
#define CURL_GLOBAL_FLAG CURL_GLOBAL_NOTHING
#endif
#endif

int init(char *server_name, struct session *session_handle)
{
	int url_type = -1;

	#ifdef SSL
	const curl_ssl_backend ***avail = NULL;
	if (curl_global_sslset(CURLSSLBACKEND_OPENSSL, "", avail) != CURLSSLSET_OK) {
		return 2;
	}
	#endif

	if (curl_global_init(CURL_GLOBAL_FLAG) == CURLE_OK) {
		session_handle->curl_handle = curl_easy_init();
		url_type = parse_server_name(server_name);

		return 0;
	} else
		return 1;
}

int parse_server_name(char *server_name)
{
	return 1;
}
