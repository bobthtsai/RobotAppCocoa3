/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2004
 *     Brian Gerkey
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

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "playernav.h"

extern int flag_showWaypoints;
extern int flag_dockSimultion;

/* Parse command line arguments, of the form host:port */
int parse_args(int argc, char** argv, gui_data_t* gui_data)
{
	int i;
  for( i=1; (i<argc) && (argv[i][0] == '-'); i++)
  {
	  if(!strcmp(argv[i],"-h"))
	  {
	  	flag_dockSimultion = 0;
	  	if( (i+1) < argc)
	    	gui_data->hostname = argv[++i];
	    else
    return -1;
	  }
	  else if(!strcmp(argv[i],"-d"))
  {
	    flag_showWaypoints = 1;
	    printf("\n[Debug] Show waypoints\n\n");
  }
  else
	  	return -1;
  }
  return(0);
}
void print_usage(int argc, char** argv)
{
  printf("\nUSAGE error [options] can be:\n");
  printf("  -h <hostname> : IP to connect to\n");
  printf("  -d            : debug mode - show waypoints and use planner plug-in\n\n");
} // end print_usage

/*
	strcmp() ，需要兩個字串當作參數，比較兩個字串是否相等，相等就回傳0，第一個字串大於第二個字串回傳正值，反之回傳負值。
*/