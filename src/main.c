#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include "constants.h"
#include "textures.h"

int map[ROWS][COLS] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 1, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 1},
  {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 1},
  {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

struct Player {
  float x;
  float y;
  float width;
  float height;
  int turn_direction; // -1 for left, +1 for right
  int walk_direction; // -1 for back, +1 for front
  float facing;
  float movement_speed;
  float rotation_speed;
} player;

struct Ray {
  float angle;
  float length;
  float target_x;
  float target_y;
  int target_wall_material;
  int hits_vertically;
} rays[NUM_RAYS];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int is_game_running = false;
int last_frame_time = 0;

// This will be used for the color and textures
uint32_t* color_buffer = NULL;
uint32_t* wall_texture = NULL;
uint32_t* textures[TOTAL_TEXTURES];

SDL_Texture* color_buffer_texture;

int initialize_window(void);
void process_input(void);
void setup(void);
void render(void);
void update(void);
void move_player(float);
float normalize_angle(float);
int get_map_tile_at(float, float);
float distance_between_points(float, float, float, float);
void create_scene(void);
void cast_ray(float, int);
void destroy_window(void);
void render_map(void);
void render_player(void);
void render_rays(void);


int main() {
  is_game_running = initialize_window();

  setup();

  while (is_game_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
  return 0;
}


void setup() {
  player.x = WINDOW_WIDTH / 2;
  player.y = WINDOW_HEIGHT / 2;
  player.width = 5;
  player.height = 5; // ??
  player.turn_direction = 0;
  player.walk_direction = 0;
  player.facing= PI / 2;
  player.movement_speed = 100;
  player.rotation_speed = 90 * (PI / 180);  // 45deg, converted to radians

  // Because each pixel in the screen will be treated separately we need to have
  // an array to store information about each one of them. We're doing this by
  // allocating space in memory, equal to the dimensions of the window in the
  // form of uint32_t or 4 bytes per pixel.
  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * (uint32_t)WINDOW_WIDTH * (uint32_t)WINDOW_HEIGHT);

  // create an SDL_Texture to display the color_buffer
  color_buffer_texture = SDL_CreateTexture(
    renderer, // what is the renderer?
    SDL_PIXELFORMAT_RGBA32,     // what is the pixel format? e.g. order of colors, transparency, etc
    SDL_TEXTUREACCESS_STREAMING,  // update pixel frame by frame
    WINDOW_WIDTH,
    WINDOW_HEIGHT
  );

  // Similar to the color buffer we have a texture that will be "painted" over the projecttion
  // wall_texture = (uint32_t*) malloc(sizeof(uint32_t) * (uint32_t)TEXTURE_WIDTH * (uint32_t)TEXTURE_HEIGHT);
  // for (int x = 0; x < TEXTURE_WIDTH; x++) {
  //   for (int y = 0; y < TEXTURE_HEIGHT; y++) {
  //     wall_texture[TEXTURE_WIDTH * y + x] = ((x & 8) && (y & 8))
  //       ? 0xFF0000FF
  //       : 0xFF000000;
  //   }
  // }

  // textures[0] = (uint32_t*) REDBRICK_TEXTURE;
  // textures[1] = (uint32_t*) PURPLESTONE_TEXTURE;
  // textures[2] = (uint32_t*) MOSSYSTONE_TEXTURE;
  // textures[3] = (uint32_t*) GRAYSTONE_TEXTURE;
  // textures[4] = (uint32_t*) COLORSTONE_TEXTURE;
  // textures[5] = (uint32_t*) BLUESTONE_TEXTURE;
  // textures[6] = (uint32_t*) WOOD_TEXTURE;
  // textures[7] = (uint32_t*) EAGLE_TEXTURE;

  // Decode PNG files and loads the textures array
  load_wall_textures();
}


void process_input(void) {
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
      }
      if (event.key.keysym.sym == SDLK_LEFT) {
        player.turn_direction = -1;
      }
      if (event.key.keysym.sym == SDLK_RIGHT) {
        player.turn_direction = 1;
      }
      if (event.key.keysym.sym == SDLK_UP) {
        player.walk_direction = 1;
      }
      if (event.key.keysym.sym == SDLK_DOWN) {
        player.walk_direction = -1;
      }
      break;
    case SDL_KEYUP:
      if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
        player.turn_direction = 0;
      }
      if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
        player.walk_direction = 0;
      }
      break;
  }
}


void update() {

  // Calculate if ellapsed time, in ms, is in schedule with target frame speed
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

  // Delay execution of the program if it's running too fast (non-blocking)
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  // Time difference in seconds between current and last frame
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

  last_frame_time = SDL_GetTicks();

  // update player's orientation and position
  move_player(delta_time);

  // cast rays from player's position
  create_scene();
}


void generate_3D_projection(void) {
  for (int i = 0; i < NUM_RAYS; i++) {

    // float projection_plane_distance = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);

    // Get ray distance to wall, adjusting for "bowl" or "fish-eye" effect
    float ray_length = CORRECT_FISH_EYE_EFFECT
      ? rays[i].length * cos(rays[i].angle - player.facing)
      : rays[i].length;

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
    if (rays[i].hits_vertically) {
      texture_offset_x = (int)rays[i].target_y % TILE_SIZE;
    } else {
      texture_offset_x = (int)rays[i].target_x % TILE_SIZE;
    }

    // What material the wall hit by current ray has:
    int texture_index = rays[i].target_wall_material - 1;

    // Loop over each pixel from the projection vertically
    for (int y = 0; y < WINDOW_HEIGHT; y++) {

      // Paint the ceiling
      if (y < top_wall_pixel) {
        color_buffer[WINDOW_WIDTH * y + i] = 0xEEEEEEEE;
      }

      // ...whereas the y offset is calculated as the wall is rendered from top
      // to bottom. We are also accouting for the projected height of the wall.
      int distance_to_top = y - ((WINDOW_HEIGHT / 2) - (projection_wall_height / 2));
      texture_offset_y = distance_to_top * ((float)TEXTURE_HEIGHT / projection_wall_height);

      // Paiting the walls
      if (y >= top_wall_pixel && y <= bot_wall_pixel) {

        // Set the color of each pixel based on the wall_texture
        // uint32_t texel_color = wall_texture[TEXTURE_WIDTH * texture_offset_y + texture_offset_x];
        uint32_t texel_color = wall_textures[texture_index].texture_buffer[TEXTURE_WIDTH * texture_offset_y + texture_offset_x];

        color_buffer[WINDOW_WIDTH * y + i] = texel_color;
        // color_buffer[WINDOW_WIDTH * y + i] = rays[i].hits_vertically
        // ? 0xFFFFFFFF
        // : 0xFFCCCCCC;
      }

      // Painting the floor
      if (y > bot_wall_pixel) {
        color_buffer[WINDOW_WIDTH * y + i] = 0xFFEEDDCC;
      }
    }
  }
}


void clearColorBuffer(uint32_t color) {
  for (int x = 0; x < WINDOW_WIDTH; x++) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
      color_buffer[WINDOW_WIDTH * y + x] = color;
    }
  }
}


void renderColorBuffer() {
  SDL_UpdateTexture(
    color_buffer_texture,
    NULL,
    color_buffer,
    (int)((uint32_t) WINDOW_WIDTH * sizeof(uint32_t))
  );
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}


void render(void) {
  // Draw the background of the window
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  generate_3D_projection();

  renderColorBuffer();
  clearColorBuffer(0x00000000);

  // display the minimap
  render_map();
  render_player();
  render_rays();

  SDL_RenderPresent(renderer);
}


void render_rays() {
  SDL_SetRenderDrawColor(renderer, 255, 50, 50, 100);

  for (int i = 0; i < NUM_RAYS; i++) {
    SDL_RenderDrawLine(
      renderer,
      MINIMAP_SCALE_FACTOR * player.x,
      MINIMAP_SCALE_FACTOR * player.y,
      MINIMAP_SCALE_FACTOR * rays[i].target_x,
      MINIMAP_SCALE_FACTOR * rays[i].target_y
    );
  }
}


int get_map_tile_at(float x, float y) {
  if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
    return 1;
  }
  int floored_x = floor(x / TILE_SIZE);
  int floored_y = floor(y / TILE_SIZE);
  return map[floored_y][floored_x];
}


void move_player(float delta_time) {
  // Note the delta time determines the speed of animation
  player.facing += player.turn_direction * player.rotation_speed * delta_time;

  // Calculate point for the next step
  float step = player.walk_direction * player.movement_speed * delta_time;
  float next_pos_x = player.x + cos(player.facing) * step;
  float next_pos_y = player.y + sin(player.facing) * step;

  // Check wall collisions
  if (get_map_tile_at(next_pos_x, next_pos_y) == 0) {
    player.x = next_pos_x;
    player.y = next_pos_y;
  }
}


float normalize_angle(float angle) {
  angle = remainder(angle, TWO_PI);
  if (angle < 0) {
    angle = TWO_PI + angle;
  }
  return angle;
}

float distance_between_points(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void cast_ray(float angle, int idx) {

  // DEFINE RAY PROPERTIES LIKE ANGLE AND ORIENTATION
  angle = normalize_angle(angle);

  int facingSouth = angle > 0 && angle < PI;
  int facingNorth = !facingSouth;
  int facingEast  = angle < 0.5 * PI || angle > 1.5 * PI;
  int facingWest  = !facingEast;

  float xintersect, yintersect, xstep, ystep;

  /*** HORIZONTAL RAY-GRID INTERSECTION ***/
  int has_horizontal_target;
  float horizontal_target_x = FLT_MAX;
  float horizontal_target_y = FLT_MAX;

  yintersect = facingSouth
    ? floor(player.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE
    : floor(player.y / TILE_SIZE) * TILE_SIZE;

  xintersect = player.x + (yintersect - player.y) / tan(angle);

  ystep = facingNorth ? -TILE_SIZE : TILE_SIZE;
  xstep = TILE_SIZE / tan(angle);

  if ((facingWest && xstep > 0) || (facingEast && xstep < 0)) {
    xstep *= -1;
  }

  while (
    xintersect >= 0 && xintersect <= WINDOW_WIDTH &&
    yintersect >= 0 && yintersect <= WINDOW_HEIGHT
  ) {

    float yOffset = facingNorth ? yintersect - 1 : yintersect;

    if (get_map_tile_at(xintersect, yOffset)) {
      has_horizontal_target = 1;
      horizontal_target_x = xintersect;
      horizontal_target_y = yOffset;
      break;
    }

    xintersect += xstep;
    yintersect += ystep;
  }

  /*** VERTICAL RAY-GRID INTERSECTION ***/
  int has_vertical_target;
  float vertical_target_x = FLT_MAX;
  float vertical_target_y = FLT_MAX;

  xintersect = facingEast
    ? floor(player.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE
    : floor(player.x / TILE_SIZE) * TILE_SIZE;

  yintersect = player.y + (xintersect - player.x) * tan(angle);

  xstep = facingWest ? -TILE_SIZE : TILE_SIZE;
  ystep = TILE_SIZE * tan(angle);

  if ((facingNorth && ystep > 0) || (facingSouth && ystep < 0)) {
    ystep *= -1;
  }

  while (
    xintersect >= 0 && xintersect <= WINDOW_WIDTH &&
    yintersect >= 0 && yintersect <= WINDOW_HEIGHT
  ) {

    float xOffset = facingWest ? xintersect - 1 : xintersect;

    if (get_map_tile_at(xOffset, yintersect)) {
      has_vertical_target = 1;
      vertical_target_x = xOffset;
      vertical_target_y = yintersect;
      break;
    }

    xintersect += xstep;
    yintersect += ystep;
    vertical_target_x = xintersect;
  }

  // Find distance to horizontal and vertical ray-grid intersection points
  float horizontal_dist = has_horizontal_target
    ? distance_between_points(player.x, player.y, horizontal_target_x, horizontal_target_y)
    : FLT_MAX;

  float vertical_dist = has_vertical_target
    ? distance_between_points(player.x, player.y, vertical_target_x, vertical_target_y)
    : FLT_MAX;

  if (horizontal_dist < vertical_dist) {
    rays[idx].length = horizontal_dist;
    rays[idx].target_x = horizontal_target_x;
    rays[idx].target_y = horizontal_target_y;
    rays[idx].hits_vertically = 0;
  } else {
    rays[idx].length = vertical_dist;
    rays[idx].target_x = vertical_target_x;
    rays[idx].target_y = vertical_target_y;
    rays[idx].hits_vertically = 1;
  }

  rays[idx].angle = angle;
  rays[idx].target_wall_material = get_map_tile_at(
    rays[idx].target_x,
    rays[idx].target_y
  );
}


// NOTE: This function is updated from using the same angle per ray, which creates
// a slight distorted representation of the walls. The updated version (uncommented)
// assumes equal distance between rays hitting the wall, and changes the angle accordingly.
void create_scene(){
  // start first ray subtracting half of the FOV (see update note)
  // float ray_angle = player.facing - (FOV_ANGLE / 2);

  for (int i = 0; i < NUM_RAYS; i++) {
    float ray_angle = player.facing + atan((i - NUM_RAYS / 2) / DISTANCE_PROJ_PLANE);

    // Create rays from player's position
    cast_ray(ray_angle, i);

    // increase angle based on perception and casted rays
    // ray_angle += FOV_ANGLE / NUM_RAYS; (see updated note)
  }
}


void render_map() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      int tileX = j * TILE_SIZE;
      int tileY = i * TILE_SIZE;
      int tileColor = map[i][j] != 0 ? 50 : 225;

      // Draw rectangle per each tile
      SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
      SDL_Rect tile = {
        MINIMAP_SCALE_FACTOR * tileX,
        MINIMAP_SCALE_FACTOR * tileY,
        MINIMAP_SCALE_FACTOR * TILE_SIZE,
        MINIMAP_SCALE_FACTOR * TILE_SIZE
      };
      SDL_RenderFillRect(renderer, &tile);
    }
  }
}


void render_player() {
  // Define the player rectangle
  SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
  SDL_Rect player_rect = {
    MINIMAP_SCALE_FACTOR * player.x,
    MINIMAP_SCALE_FACTOR * player.y,
    MINIMAP_SCALE_FACTOR * player.width,
    MINIMAP_SCALE_FACTOR * player.height
  };

  // Draw player and a line to signal player orientation
  // NOTE: cos and sin may not be available to the compiler. Add "-lm" after
  // "-lSDL2" to link to the math module.
  // SDL_RenderFillRect(renderer, &player_rect);
  // SDL_RenderDrawLine(
  //   renderer,
  //   MINIMAP_SCALE_FACTOR * player.x,
  //   MINIMAP_SCALE_FACTOR * player.y,
  //   MINIMAP_SCALE_FACTOR * player.x + cos(player.facing) * 40,
  //   MINIMAP_SCALE_FACTOR * player.y + sin(player.facing) * 40
  // );
}


void destroy_window(void) {
  // Release memory allocated by the program
  free(color_buffer);
  free_wall_textures();
  SDL_DestroyTexture(color_buffer_texture);

  // Cleaning up function when quiting the program, the order matters
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}


int initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initizializing SDL.\n");
    return false;
  }

  window = SDL_CreateWindow(
    "Raycasting Rendering in C with SDL",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
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

  return true;
}
