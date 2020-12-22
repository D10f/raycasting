#include "map.h"

static const int map[ROWS][COLS] = {
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
      uint32_t tile_color = map[i][j] != 0 ? 0x00000000 : 0x00F7F7F7;

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
