#include "clean.h"

void clean(struct session *session_handle)
{
	curl_easy_cleanup(session_handle->curl_handle);
	curl_global_cleanup();
}
