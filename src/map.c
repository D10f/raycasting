#include "../include/map.h"

static const int map[ROWS][COLS] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
  {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
  {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1},
  {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
  {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void get_random_neighbor(float x, float y, float *next_tile, const float *prev_tile) {

  int current_col = floor(x / TILE_SIZE);
  int current_row = floor(y / TILE_SIZE);
  int previous_col = floor(prev_tile[0] / TILE_SIZE);
  int previous_row = floor(prev_tile[1] / TILE_SIZE);

  // top, right, bottom, left
  int neighbors[4][2] = {
    { current_col, current_row - 1 },
    { current_col + 1, current_row },
    { current_col, current_row + 1 },
    { current_col - 1, current_row },
  };

  int random_tile_col;
  int random_tile_row;
  int random_tile;
  int random_idx;

  do {
    random_idx = rand() % 4;
    random_tile_col = neighbors[random_idx][0];
    random_tile_row = neighbors[random_idx][1];
    random_tile = map[random_tile_row][random_tile_col];

  } while (random_tile == 1 || (random_tile_col == previous_col && random_tile_row == previous_row));

  float random_tile_x = random_tile_col * TILE_SIZE;
  float random_tile_y = random_tile_row * TILE_SIZE;

  next_tile[0] = random_tile_x + TILE_SIZE * 0.5;
  next_tile[1] = random_tile_y + TILE_SIZE * 0.5;
}

int get_map_tile_at(float x, float y) {
  // if (x < 0 || x >= COLS * TILE_SIZE || y < 0 || y >= ROWS * TILE_SIZE) {
  //   return 1;
  // }

  if (!is_inside_map(x, y)) {
    return 1;
  }
  int floored_x = floor(x / TILE_SIZE);
  int floored_y = floor(y / TILE_SIZE);
  return map[floored_y][floored_x];
}

void render_map(void) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      int tile_x = j * TILE_SIZE;
      int tile_y = i * TILE_SIZE;
      color_t tile_color = map[i][j] != 0 ? 0x00000000 : 0x00F7F7F7;

      draw_rect(
        MINIMAP_SCALE_FACTOR * tile_x,
        MINIMAP_SCALE_FACTOR * tile_y,
        MINIMAP_SCALE_FACTOR * TILE_SIZE,
        MINIMAP_SCALE_FACTOR * TILE_SIZE,
        tile_color
      );
    }
  }
}

bool is_inside_map(float x, float y) {
  return (
    x >= 0 && x <= COLS * TILE_SIZE &&
    y >= 0 && y <= ROWS * TILE_SIZE
  );
}
