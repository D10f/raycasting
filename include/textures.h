#ifndef TEXTURES_H
#define TEXTURES_H

#include "defs.h"
#include "upng.h"
#include <stdint.h>

/**
 * Stores information about a particular texture.
 * It serves as a wrapper to the upng_t pointer which contains the actual
 * data loaded by the upng library. The rest of the properties are used as
 * convenience to interact with the rest of the program.
 *
 */
typedef struct texture_t {
  int width;                ///< Width of the texture.
  int height;               ///< Height of the texture.
  color_t *texture_buffer;  ///< Buffer data of the texture.
  upng_t *upng_texture;     ///< Pointer to the texture data as loaded by upng.
} texture_t;

extern texture_t wall_textures[TOTAL_TEXTURES];


/**
 * Loads all the textures from the images directory.
 * The total number of textures loaded is predefined in defs.h and not by the
 * number of files in the images directory. The path to the images is also
 * defined in a separate variable inside textures.c; this function loops over
 * it and loads the appropriate file into a texture_t struct.
 *
 */
void load_wall_textures();

/**
 * Frees all the allocated memory used to load the image files.
 * It uses the upng_t pointer inside each struct to call the provided upng_free
 * function.
 *
 */
void free_wall_textures();

#endif
