#include <string.h>
#include <regex.h>
#include <libxml/HTMLparser.h>
#include "include.h"

int parse_server_name(char *server_name, struct url *retvar);
int parse_html(char *buffer);
