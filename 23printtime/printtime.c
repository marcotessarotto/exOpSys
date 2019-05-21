#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void get_current_time(char * buffer)
{
    time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}


int main(int argc, char * argv[]) {

	char buffer[32];

	get_current_time(buffer);

	printf(buffer);

	return 0;
}
