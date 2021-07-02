#include <stdio.h>


struct session {
	char url[100];
};
int init(char *server_name, struct session *session_handle);
int main() 
{
	struct session *session_handle;
	printf("%d", init("dd", session_handle));
	clean(session_handle);
}
