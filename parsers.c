#include "parsers.h"

int parse_server_name(char *server_name, struct url *retvar)
{
	regex_t compiled_regex;
	regmatch_t matchptr[7];

	int regcomp_retval = regcomp(&compiled_regex, "^((https?)://)?([^:]*(\.[^:]*)*)(:([1-9]\d*))?$", REG_EXTENDED);
	if (!regcomp_retval) {
		regexec(&compiled_regex, server_name, 7, matchptr, 0);

		if (matchptr[3].rm_so < 0)
			return 1;
		strncpy(retvar->hostname, server_name + matchptr[3].rm_so, matchptr[3].rm_eo - matchptr[3].rm_so);
		if (matchptr[2].rm_so >= 0) {
			strncpy(retvar->protocol, server_name + matchptr[2].rm_so, matchptr[2].rm_eo - matchptr[2].rm_so);
		}
		if (matchptr[6].rm_so > 0) {
			char buff[6];
			char *end;
			strncpy(buff, server_name + matchptr[6].rm_so, matchptr[6].rm_eo - matchptr[6].rm_so);
			retvar->port = strtol(buff, &end, 10);
		}
	}

	regfree(&compiled_regex);

	return 0;
}

int parse_html(char *buffer, int size, char *url, char *encoding, xmlNode *first_element)
{
	htmlDocPtr document_ptr = htmlReadMemory(buffer, size, url, encoding, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NOIMPLIED);
	if (document_ptr == NULL)
		return 1;
	first_element = xmlDocGetRootElement(document_ptr);
	if (first_element  == NULL)
		return 2;

	return 0;
}
