#include "include.h"
void clean(struct session *session_handle)
{
	curl_global_cleanup();
}
