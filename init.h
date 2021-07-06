#include <string.h>
#include <stdio.h>
#include "include.h"
#include "parsers.h"

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

int init(char *server_name, struct session *session_handle);
int parse_server_name(char *server_name, struct url *retvar);
