struct session_cookie {
	char username[100];
	char password[100];
	char cookie_code[100];
};
struct session {
	char url[100];
	struct session_cookie cookie;
	CURL *curl_handle;
};
