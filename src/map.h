#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "defs.h"
#include "graphics.h"

#define COLS 20
#define ROWS 13

int get_map_tile_at(float, float);
void render_map(void);
bool is_inside_map(float, float);

#endif
