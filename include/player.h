#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"
#include "map.h"
#include "graphics.h"

/**
 * Stores a player's defails.
 * It stores information such as the position in the map, the direction it's
 * facing, movement speed, etc.
 *
 */
typedef struct player_t {
  float x;              ///< The player's x position.
  float y;              ///< The player's y position.
  float width;          ///< The width of the rectangle drawn in the map.
  float height;         ///< The height of the rectangle drawn in the map.
  float facing;         ///< Angle of the direction the player is facing.
  float movement_speed; ///< 1 for moving, 0 when not.
  float rotation_speed; ///< 1 for rotating, 0 when not.
  int turn_direction;   ///< -1 for left, +1 for right.
  int walk_direction;   ///< -1 for back, +1 for front.
  int *prev_tile[2];    ///< tuple with x,y coordinates of past player position
  int *next_tile[2];    ///< tuple with x,y coordinates of next player position
} player_t;

// declare it as a global variable (to be defined in an external file)
extern player_t player;


/**
 * Renders the player as a rectangle in the minimap.
 *
 */
void render_player(void);


/**
 * Updates the facing direction and position of the player.
 * Takes into account the deltatime calculated using the SDL_Delay function.
 * @param[in] deltatime 
 *
 */
void move_player(float deltatime);

#endif
