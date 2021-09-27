/** test.c **/
 

#include <syslog.h>

int main(int argc,char **argv){
	openlog("isula",LOG_CONS | LOG_PID,LOG_LOCAL2);
	syslog(LOG_DEBUG,"THIS is a\n");
	closelog();
	return 0;
}