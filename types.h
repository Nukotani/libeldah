struct url {
	char hostname[100];
	char path[100];
	char protocol[6];
}
struct session_cookie {
	char username[100];
	char password[100];
	char cookie_code[100];
};
struct session {
	struct url session_url;
	struct session_cookie cookie;
	CURL *curl_handle;
};
