struct session_handle {
	char url[100];
	struct session_cookie;
	CURL *curl_handle;
}
