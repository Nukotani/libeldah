#include "init.h"

static size_t check_website(char *data, size_t size, size_t nmemb, void *return_var)
{
	parse_html(data, size * nmemb,  "", "utf8", (xmlNode *) return_var);
	return size * nmemb;
}

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

		int url_ret = parse_server_name(server_name, &session_handle->session_url);
		if (url_ret)
			return 2;

		curl_easy_setopt(session_handle->curl_handle, CURLOPT_DEFAULT_PROTOCOL, DEFAULT_PROTOCOL);
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		char buffer[400];
		memset(buffer, 0, 400);
		xmlNode parsed_buffer[1];

		if (session_handle->session_url.protocol[0] != '\0') {
			strncat(buffer, session_handle->session_url.protocol, 100);
			strncat(buffer, "://", 4);
		}
		strncat(buffer, session_handle->session_url.hostname, 100);
		if (session_handle->session_url.port != 0)
			curl_easy_setopt(session_handle->curl_handle, CURLOPT_PORT, session_handle->session_url.port);

		curl_easy_setopt(session_handle->curl_handle, CURLOPT_URL, buffer);
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_BUFFERSIZE, 1024);
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_TCP_FASTOPEN, 1L);
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_WRITEFUNCTION, check_website);
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_WRITEDATA, check_website);

		if(curl_easy_perform(session_handle->curl_handle) == CURLE_OK) {

			return 0;
		}

		return 4;

	} else
		return 1;
}


