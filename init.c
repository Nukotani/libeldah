#include "include.h"
#include <string.h>
#include <stdio.h>
#include "init.h"
#include <regex.h>

#ifdef SSL
	#define DEFAULT_PROTOCOL "https"
	#ifdef _WIN32
		#define CURL_GLOBAL_FLAG CURL_GLOBAL_ALL
	#else
		#define CURL_GLOBAL_FLAG CURL_GLOBAL_SSL
	#endif
#else
	#define DEFAULT_PROTOCOL "http"
	#ifdef _WIN32
		#define CURL_GLOBAL_FLAG CURL_GLOBAL_WIN32
	#else
		#define CURL_GLOBAL_FLAG CURL_GLOBAL_NOTHING
	#endif
#endif

int init(char *server_name, struct session *session_handle)
{
	#ifdef SSL
	const curl_ssl_backend ***avail = NULL;
	if (curl_global_sslset(CURLSSLBACKEND_OPENSSL, "", avail) != CURLSSLSET_OK) {
		return 3;
	}
	#endif

	if (curl_global_init(CURL_GLOBAL_FLAG) == CURLE_OK) {
		session_handle->curl_handle = curl_easy_init();

		memset(session_handle->session_url.hostname, 0, 100);
		memset(session_handle->session_url.path, 0, 200);
		memset(session_handle->session_url.protocol, 0, 6);
		session_handle->session_url.port = 0;

		int url_ret parse_server_name(server_name, &session_handle->session_url);
		if (url_ret)
			return 2;

		curl_easy_setopt(session_handle->curl_handle, CURLOPT_DEFAULT_PROTOCOL, DEFAULT_PROTOCOL);
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		char buffer[400];
		memset(buffer, 0, 400);

		if (session_handle->session_url.protocol[0] != '\0') {
			strncat(buffer, session_handle->session_url.protocol, 100);
			strncat(buffer, "://", 3);
		}
		strncat(buffer, session_handle->session_url.hostname, 100);
		if (session_handle->session_url.port != 0)
			curl_easy_setopt(session_handle->curl_handle, CURLOPT_PORT, session_handle->session_url.port);

		return 0;
	} else
		return 1;
}

int parse_server_name(char *server_name, struct url *retvar)
{
	regex_t compiled_regex;
	regmatch_t matchptr[7];

	int regcomp_retval = regcomp(&compiled_regex, "^((https?)://)?([^:]*(\.[^:]*)*)(:([1-9]\d*))?$", REG_EXTENDED);
	if (!regcomp_retval) {
		regexec(&compiled_regex, server_name, 7, matchptr, 0);

		if (matchptr[3].rm_so < 0)
			return 1;
		strncpy(retvar->hostname, server_name + matchptr[3].rm_so, matchptr[3].rm_eo - matchptr[3].rm_so);
		if (matchptr[2].rm_so >= 0) {
			strncpy(retvar->protocol, server_name + matchptr[2].rm_so, matchptr[2].rm_eo - matchptr[2].rm_so);
		}
		if (matchptr[6].rm_so > 0) {
			char buff[6];
			char *end;
			strncpy(buff, server_name + matchptr[6].rm_so, matchptr[6].rm_eo - matchptr[6].rm_so);
			retvar->port = strtol(buff, &end, 10);
		}
	}

	regfree(&compiled_regex);

	return 0;
}
