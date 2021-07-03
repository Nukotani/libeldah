struct url {
	char path[200]
	char hostname[100];
	char protocol[5];
	int port;
};
struct session_cookie {
	char username[100];
	char password[100];
	char cookie_code[100];
};
struct session {
	struct session_cookie cookie;
	struct url session_url;
	CURL curl_handle[1];
};
