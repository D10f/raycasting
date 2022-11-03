#ifndef PLAYER_H
#define PLAYER_H

#include "defs.h"
#include "map.h"
#include "graphics.h"

typedef struct {
  float x;
  float y;
  float width;
  float height;
  int turn_direction; // -1 for left, +1 for right
  int walk_direction; // -1 for back, +1 for front
  float facing;
  float movement_speed;
  float rotation_speed;
} player_t;

// declare it as a global variable (to be defined in an external file)
extern player_t player;

void render_player(void);
void move_player(float);

#endif
