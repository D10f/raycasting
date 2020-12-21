#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdint.h>
#include "constants.h"
#include "upng.h"

typedef struct {
  upng_t* upng_texture;
  int width;
  int height;
  uint32_t* texture_buffer;
} texture_t;

texture_t wall_textures[TOTAL_TEXTURES];

void load_wall_textures();
void free_wall_textures();

#endif
