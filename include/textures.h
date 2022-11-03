#ifndef TEXTURES_H
#define TEXTURES_H

#include "defs.h"
#include "upng.h"
#include <stdint.h>

typedef struct {
  int width;
  int height;
  uint32_t *texture_buffer;
  upng_t *upng_texture;
} texture_t;

extern texture_t wall_textures[TOTAL_TEXTURES];

void load_wall_textures();
void free_wall_textures();

#endif
