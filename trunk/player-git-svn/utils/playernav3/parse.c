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

extern double dumpfreq;
extern double mapupdatefreq;

/* Parse command line arguments, of the form host:port */
int parse_args(int argc, char** argv, gui_data_t* gui_data)
{
	if (argc > 3)
		return -1;
	else if (argc == 3)
		gui_data->hostname = argv[2];
	else if (argc == 2)
		gui_data->hostname = argv[1];
	else
		gui_data->hostname = "localhost";

	gui_data->port = 6665;

	return(0);
}
