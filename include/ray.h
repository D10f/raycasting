#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <limits.h>
#include "defs.h"
#include "map.h"
#include "player.h"

typedef struct {
  float angle;
  float length;
  float target_x;
  float target_y;
  bool hits_vertically;
  int target_wall_material;
} ray_t;

// Tells the compiler this is a global variable
extern ray_t rays[NUM_RAYS];

float normalize_angle(float);
float distance_between_points(float, float, float, float);
void cast_ray(float, int);
void render_rays(void);
void create_scene(void);

#endif
