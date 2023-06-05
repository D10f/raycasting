#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include <SDL2/SDL.h>

#include "../include/defs.h"
#include "../include/textures.h"
#include "../include/graphics.h"
#include "../include/map.h"
#include "../include/ray.h"
#include "../include/player.h"

#if __EMSCRIPTEN__
#include <emscripten.h>
#endif

bool is_game_running = false;
bool is_game_paused = false;
int last_frame_time = 0;

color_t* wall_texture = NULL;

void process_input(void);
void setup(void);
void render(void);
bool update(void);
void main_loop(void);
void release_resources(void);

int main() {

  srand(time(NULL));

  is_game_running = initialize_window();

  setup();

  #if __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, FPS, 1);
  #else
    while(is_game_running) main_loop();
  #endif

  release_resources();
  return 0;
}

void main_loop() {
  process_input();
  if (update()) {
    render();
  }
}


void setup() {
  // Decode PNG files and loads the textures array
  load_wall_textures();
}

void toggle_play() {
  is_game_paused = !is_game_paused;
}

void process_input(void) {

  // Remove event handling as to not interfere with browser events
  #if __EMSCRIPTEN__
    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_DISABLE);
  #endif

  // Create an event object that listens for user input
  SDL_Event event;
  SDL_PollEvent(&event);

  // Exit game when user clicks on the window's X or presses the escape key
  switch (event.type) {
    case SDL_QUIT:
      is_game_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_game_running = false;
        break;
      }
      if (event.key.keysym.sym == SDLK_p) {
        toggle_play();
        break;
      }
      /* if (event.key.keysym.sym == SDLK_LEFT) { */
        /* player.turn_direction = -1; */
      /* } */
      /* if (event.key.keysym.sym == SDLK_RIGHT) { */
        /* player.turn_direction = 1; */
      /* } */
      /* if (event.key.keysym.sym == SDLK_UP) { */
        /* player.walk_direction = 1; */
      /* } */
      /* if (event.key.keysym.sym == SDLK_DOWN) { */
        /* player.walk_direction = -1; */
      /* } */
      /* break; */
    /* case SDL_KEYUP: */
      /* if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) { */
        /* player.turn_direction = 0; */
      /* } */
      /* if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) { */
        /* player.walk_direction = 0; */
      /* } */
      /* break; */
  }
}

bool update() {

  // Calculate if ellapsed time, in ms, is in schedule with target frame speed
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

  // Delay execution of the program if it's running too fast (non-blocking)
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  // Time difference in seconds between current and last frame
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

  last_frame_time = SDL_GetTicks();

  if (is_game_paused) return false;

  // update player's orientation and position
  move_player(delta_time);

  // cast rays from player's position
  create_scene();

  return true;
}


void render_wall_projection(void) {
  for (int x = 0; x < NUM_RAYS; x++) {

    // float projection_plane_distance = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);

    // Get ray distance to wall, adjusting for "bowl" or "fish-eye" effect
    float ray_length = CORRECT_FISH_EYE_EFFECT
      ? rays[x].length * cos(rays[x].angle - player.facing)
      : rays[x].length;

    // Find the height of the projection per strip
    int projection_wall_height = (TILE_SIZE / ray_length) * DISTANCE_PROJ_PLANE;

    // Find height where the wall projections starts
    int top_wall_pixel = (WINDOW_HEIGHT / 2) - (projection_wall_height / 2);
    top_wall_pixel = top_wall_pixel < 0 ? 0 : top_wall_pixel;

    // Find height where the wall projections ends
    int bot_wall_pixel = (WINDOW_HEIGHT / 2) + (projection_wall_height / 2);
    bot_wall_pixel = bot_wall_pixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : bot_wall_pixel;

    // Get the offset of the ray hitting a wall, to know the texture pixel
    // that corresponds and will be painted for that projection. The offset will
    // be different depending if the ray hit vertically on the wall or not.
    int texture_offset_x;
    int texture_offset_y;

    // The x offset is calculated once per ray...
    if (rays[x].hits_vertically) {
      texture_offset_x = (int)rays[x].target_y % TILE_SIZE;
    } else {
      texture_offset_x = (int)rays[x].target_x % TILE_SIZE;
    }

    // What material the wall hit by current ray has:
    int texture_index = rays[x].target_wall_material - 1;

    // Loop over each pixel from the projection vertically
    for (int y = 0; y < WINDOW_HEIGHT; y++) {

      // Paint the ceiling
      if (y < top_wall_pixel) {
        draw_pixel(x, y, 0xEEEEEEEE);
      }

      // ...whereas the y offset is calculated as the wall is rendered from top
      // to bottom. We are also accouting for the projected height of the wall.
      int distance_to_top = y - ((WINDOW_HEIGHT / 2) - (projection_wall_height / 2));
      texture_offset_y = distance_to_top * ((float)TEXTURE_HEIGHT / projection_wall_height);

      // Paiting the walls
      if (y >= top_wall_pixel && y <= bot_wall_pixel) {

        // Set the color of each pixel based on the wall_texture
        color_t texel_color = wall_textures[texture_index].texture_buffer[TEXTURE_WIDTH * texture_offset_y + texture_offset_x];
        draw_pixel(x,y, texel_color);
      }

      // Painting the floor
      if (y > bot_wall_pixel) {
        draw_pixel(x, y, 0xFFEEDDCC);
      }
    }
  }
}

void render(void) {
  clear_color_buffer(0x00000000);
  render_wall_projection();

  /* render_map(); */
  /* render_rays(); */
  /* render_player(); */

  render_color_buffer();
}

void release_resources(void) {
  free_wall_textures();
  destroy_window();
}
