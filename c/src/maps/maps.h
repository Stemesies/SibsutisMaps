#ifndef MAPS_H_
#define MAPS_H_

#include <libmaps/search.h>

#define DATABASE_NAME "input"

/*Читает базу данных и записывает все в карту*/
Map* fetch_map_data();

int construct_paths(MapConfig* mapconfig);

#endif