/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2004
 *     Brian Gerkey
 */

#include <assert.h>
#include "playernav.h"

/*
 * Connect to player at each host:port pair, as specified by the global
 * vars 'hostnames' and 'ports'.  Also subscribes to each device, and adds 
 * each client into a new multiclient (which is returned)
 */
int init_player(gui_data_t* gui_data)
{
	/* Connect to Player */
	gui_data->client = playerc_client_create(NULL, gui_data->hostname, gui_data->port);
	assert(gui_data->client);

	if(playerc_client_connect(gui_data->client) < 0)
	{
		fprintf(stderr, "Failed to connect to %s:%d\n\n", gui_data->hostname, gui_data->port);
		return -1;
	}

	// create map
	gui_data->map = playerc_map_create(gui_data->client, 0);
	assert(gui_data->map);
	if(playerc_map_subscribe(gui_data->map, PLAYER_OPEN_MODE) < 0)
	{
		fprintf(stderr, "Failed to subscribe to map (map0)\n\n");
		return -1;
	}
	/*  
	// virtual wall map 預先讀取，可在選單更換Subscribe
	gui_data->mapVW = playerc_map_create(gui_data->client, gui_data->mapVW_idx);
	assert(gui_data->mapVW);
	if(playerc_map_subscribe(gui_data->mapVW, PLAYER_OPEN_MODE) < 0)
	{
	printf("Failed to subscribe to virtual wall map (map%d)\n\n", gui_data->mapVW_idx);
	gui_data->mapVW = NULL;
	}
	 */
	// foot print map
	gui_data->mapFP = playerc_map_create(gui_data->client, 70);
	assert(gui_data->mapFP);
	if(playerc_map_subscribe(gui_data->mapFP, PLAYER_OPEN_MODE) < 0)
	{
		printf("Failed to subscribe to foot print map (map70)\n\n");
		gui_data->mapFP = NULL;
	}

	/* Get the map, just so we know how big to make the window */
	if(playerc_map_get_map(gui_data->map) < 0)
	{
		fprintf(stderr, "Failed to get map\n");
		// no map yet; guess some values to initialize the GUI
		gui_data->map->width = gui_data->map->height = 400;
		gui_data->map->resolution = 0.1;
		gui_data->map->origin[0] = -20.0;
		gui_data->map->origin[1] = -20.0;
	}
	gui_data->mapSize  = gui_data->map->width * gui_data->map->height;
	gui_data->mapWidth  = gui_data->map->width; // int
	gui_data->mapHeight = gui_data->map->height;
	mapOrigin[0]  = gui_data->map->origin[0];
	mapOrigin[1]  = gui_data->map->origin[1];
	mapResolution = gui_data->map->resolution;

	printf("Map size:(%d,%d), origin:(%.4f,%.4f) resolution:%.4f\n\n"
			, gui_data->map->width,     gui_data->map->height
			, gui_data->map->origin[0], gui_data->map->origin[1], gui_data->map->resolution);

	// 創立與初始化行經路線圖
	gui_data->moving_path = malloc(  gui_data->mapSize * sizeof(int));
	memset(gui_data->moving_path, 0, gui_data->mapSize * sizeof(int));

	gui_data->planner = playerc_planner_create(gui_data->client, 0);
	assert(gui_data->planner);
	if(playerc_planner_subscribe(gui_data->planner, PLAYER_OPEN_MODE) < 0)
	{
		fprintf(stderr, "Warning: Failed to subscribe to planner on robot; you won't be able to set robot's goal.\n");
		playerc_planner_destroy(gui_data->planner);
		gui_data->planner = NULL;
	}
	return 1;
}

void fini_player(gui_data_t* gui_data)
{
	puts("\nDestroy Connect\n");
	if(gui_data->map)
		playerc_map_destroy(gui_data->map);
	if(gui_data->mapVW)
		playerc_map_destroy(gui_data->mapVW);
	if(gui_data->mapZONE)
		playerc_map_destroy(gui_data->mapZONE);
	if(gui_data->mapPOWER)
		playerc_map_destroy(gui_data->mapPOWER);
	if(gui_data->mapFP)
		playerc_map_destroy(gui_data->mapFP);
	if(gui_data->planner)
		playerc_planner_destroy(gui_data->planner);
	playerc_client_destroy(gui_data->client); 
}
