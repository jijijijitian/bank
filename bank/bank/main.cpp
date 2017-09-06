#include "myhead.h"

int main()
{
	do_server();

	return 0;
}

char* bank :: gettime()
{
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	static char days[20] = { 0 };
	sprintf_s(days, "%d-%02d-%02d %02d:%02d:%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

	return days;
}