#include "init.h"

static size_t parse_callback(char *data, size_t size, size_t nmemb, void *return_var)
{
	GumboOutput *parser_buffer = (GumboOutput *)return_var;
	parse_html(data, parser_buffer);
	return size * nmemb;
}

int check_website(GumboNode *nodeptr)
{
	GumboNode *head = (GumboNode *)(nodeptr->v.element.children.data[0]);

}

int init(char *server_name, struct session *session_handle)
{
	#ifdef SSL
	//set backend ssl ke openssl, gabakal gw ganti ke gnutls, cry all you want
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
		//e-learning madrasah semuanya pake http,  protokol normis
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		//ukuran hostname + ukuran path
		char buffer[sizeof (session_handle->session_url.hostname) + sizeof (session_handle->session_url.path) + 6];
		memset(buffer, 0, sizeof buffer);
		//buffer yang udah di-parse 
		GumboOutput parsed_buffer = {0};

		if (session_handle->session_url.protocol[0] != '\0') {
			strncat(buffer, session_handle->session_url.protocol, 100);
			strncat(buffer, "://", 4);
		}
		strncat(buffer, session_handle->session_url.hostname, 100);
		if (session_handle->session_url.port != 0)
			curl_easy_setopt(session_handle->curl_handle, CURLOPT_PORT, session_handle->session_url.port);

		curl_easy_setopt(session_handle->curl_handle, CURLOPT_URL, buffer);
		//ukuran buffer paling kecil karena cuma buat nge-cek
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_BUFFERSIZE, 1024);
		//teknik dari pegawai google, ngga tau detilnya gw
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_TCP_FASTOPEN, 1L);
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_WRITEFUNCTION, parse_callback);
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_WRITEDATA, (void *)&parsed_buffer);
		//banyak nigger yang pake self-signed certificate, contohnya man1balam
		curl_easy_setopt(session_handle->curl_handle, CURLOPT_SSL_VERIFYPEER, 0);

		if(curl_easy_perform(session_handle->curl_handle) == CURLE_OK) {
			check_website(parsed_buffer.root);

			return 0;
		}

		return 4;

	} else
		return 1;
}
