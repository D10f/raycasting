#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "defs.h"
#include "graphics.h"

#define COLS 20 ///< Number of columns used in the map.
#define ROWS 13 ///< Number of rows used in the map.


/**
 * Returns the tile of the map at the given coordinates.
 * @param[in] x The x coordinate belonging to a tile in the map.
 * @param[in] y The y coordinate belonging to a tile in the map.
 * @return The number of the tile, indicating the texture to use to draw it.
 *
 */
int get_map_tile_at(float x, float y);


/**
 * Checks if the given coordinates are within the bounds of the map.
 * @param[in] x The x coordinate to check.
 * @param[in] y The y coordinate to check.
 *
 */
bool is_inside_map(float x, float y);


/**
 * Draws the minimap on the screen.
 * It uses the defined constant variable MINIMAP_SCALE_FACTOR to calculate the
 * size of the minimap.
 *
 */
void render_map(void);

#endif
