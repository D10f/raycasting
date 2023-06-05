#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

#define PI 3.14159265
#define TWO_PI 6.28318530

#define TILE_SIZE 64

#define MINIMAP_SCALE_FACTOR 0.2
#define WINDOW_WIDTH 535
#define WINDOW_HEIGHT 250

#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64
#define TOTAL_TEXTURES 9

#define FOV_ANGLE 60 * (PI / 180)
#define PROJECTION_THICKNESS 1
#define NUM_RAYS (WINDOW_WIDTH / PROJECTION_THICKNESS)
#define DISTANCE_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))

#define CORRECT_FISH_EYE_EFFECT 1

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

typedef uint32_t color_t;

#endif
