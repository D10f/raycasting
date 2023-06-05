#include "../include/graphics.h"

// These are not global variables anymore (previously declared as such in main.c)
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* color_buffer_texture = NULL;
static color_t* color_buffer = NULL;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Error initizializing SDL.\n");
    return false;
  }

  // Query screen dimensions for full-screen mode
  // Note that the resolution defined in constants remains the same, SDL only
  // takes advantage of the display size and scales things up accordingly
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  int full_screen_width = display_mode.w;
  int full_screen_height = display_mode.h;

  window = SDL_CreateWindow(
    "Raycasting Rendering in C with SDL",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    /* full_screen_width, */
    /* full_screen_height, */
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_BORDERLESS
  );

  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  // Create a renderer with default options
  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }

  // Define default draw mode
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // Because each pixel in the screen will be treated separately we need to have
  // an array to store information about each one of them. We're doing this by
  // allocating space in memory, equal to the dimensions of the window in the
  // form of color_t or 4 bytes per pixel.
  color_buffer = (color_t*) malloc(sizeof(color_t) * (color_t)WINDOW_WIDTH * (color_t)WINDOW_HEIGHT);

  // create an SDL_Texture to display the color_buffer
  color_buffer_texture = SDL_CreateTexture(
    renderer, // what is the renderer?
    SDL_PIXELFORMAT_RGBA32,     // what is the pixel format? e.g. order of colors, transparency, etc
    SDL_TEXTUREACCESS_STREAMING,  // update pixel frame by frame
    WINDOW_WIDTH,
    WINDOW_HEIGHT
  );

  return true;
}

void destroy_window(void) {
  // Release memory allocated by the program
  free(color_buffer);
  SDL_DestroyTexture(color_buffer_texture);

  // Cleaning up function when quiting the program, the order matters
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void clear_color_buffer(color_t color) {
  for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
    color_buffer[i] = color;
  }
}

void render_color_buffer() {
  SDL_UpdateTexture(
    color_buffer_texture,
    NULL,
    color_buffer,
    (int)((color_t) WINDOW_WIDTH * sizeof(color_t))
  );
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void draw_pixel(int x, int y, color_t color) {
  color_buffer[WINDOW_WIDTH * y + x] = color;
}

void draw_rect(int x, int y, int width, int height, color_t color) {
  for (int i = x; i <= x + width; i++) {
    for (int j = y; j <= y + height; j++) {
      draw_pixel(i, j, color);
    }
  }
}

// Draws a line using the DDA (Digital Differential Analyzer) algorithm
void draw_line(int x1, int y1, int x2, int y2, color_t color) {

  // Find the sides of the "triangle"
  int delta_x = (x2 - x1);
  int delta_y = (y2 - y1);

  // Find the longest side
  int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

  // Find how much the values increase on each iteration
  float x_increment = delta_x / (float)side_length;
  float y_increment = delta_y / (float)side_length;

  float current_x = x1;
  float current_y = y1;

  // Paint the nearest pixel found after rounding x and y coordinates
  for (int i = 0; i <= side_length; i++) {
    draw_pixel(round(current_x), round(current_y), color);
    current_x += x_increment;
    current_y += y_increment;
  }
}
