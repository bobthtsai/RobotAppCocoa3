/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2000  
 *     Brian Gerkey, Kasper Stoy, Richard Vaughan, & Andrew Howard
 *                      
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */
/********************************************************************
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 ********************************************************************/

/*
 * $Id$
 *
 * this is the WallclockTime class, which just calls gettimeofday() to get
 * the current time
 *
 */

#include <libplayercore/wallclocktime.h>

#ifdef CLOCKGETTIME_REPLACE_GETTIMEOFDAY
	#include <stdio.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <time.h>
	int clockgettime_replace_gettimeofday (struct timeval *tv, struct timezone *tz)
	{
		struct timespec time;

		if( clock_gettime(CLOCK_MONOTONIC, &time)==-1 )
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
	#define gettimeofday clockgettime_replace_gettimeofday
#endif

int WallclockTime::GetTime(struct timeval* time) 
{ 
  return(gettimeofday(time,0));
}

int WallclockTime::GetTimeDouble(double* time) 
{ 
  struct timeval ts;
  if(gettimeofday(&ts,0) < 0)
    return(-1);
  *time = ts.tv_sec + ts.tv_usec/1e6;
  return(0);
}
    
