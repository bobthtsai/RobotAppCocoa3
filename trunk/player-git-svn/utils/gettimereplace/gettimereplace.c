// From: http://www.openasthra.com/c-tidbits/gettimeofday-function-for-windows/
// No official copyright in source link or license info
// but code should be trivial enough to not be an issue

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int gettimeofday (struct timeval *tv, struct timezone *tz)
{
    //printf("The replace get time of day \n");

    struct timespec time;

    if( clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time)==-1 )
        return -1;
    else
    {
        tv->tv_sec = time.tv_sec;
        long nsec_long = time.tv_nsec/1000;
        int usec_int = (int)nsec_long;
        tv->tv_usec = (suseconds_t)usec_int;
    }

	return 0;
}
