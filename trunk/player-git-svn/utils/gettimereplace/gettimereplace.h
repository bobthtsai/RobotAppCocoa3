#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

static int clockgettime_replace_gettimeofday (struct timeval *tv, struct timezone *tz)
{
    //printf("The replace get time of day \n");

    struct timespec time;

    if( clock_gettime(CLOCK_MONOTONIC, &time)==-1 )
        return -1;
    else
    {
        tv->tv_sec = time.tv_sec;
        long nsec_long = time.tv_nsec/1000;
        tv->tv_usec = (suseconds_t)nsec_long;        

        /*int usec_int = (int)nsec_long;
        tv->tv_usec = (suseconds_t)usec_int;*/
    }

	return 0;
}
