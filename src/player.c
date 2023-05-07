#include "../include/player.h"
#include <math.h>

player_t player = {
  /* .x = WINDOW_WIDTH / 2, */
  /* .y = WINDOW_HEIGHT / 2, */
  /* .x = TILE_SIZE * 2 - (float)TILE_SIZE / 2, */
  /* .y = TILE_SIZE * 2 - (float)TILE_SIZE / 2, */
  .x = TILE_SIZE * 1.5,
  .y = TILE_SIZE * 1.5,
  .width = 10,
  .height = 10,
  .turn_direction = 0,
  .walk_direction = 0,
  .facing = PI / 2,
  .movement_speed = 100,
  .rotation_speed = 90 * (PI / 180),  // 45deg, converted to radians
  .prev_tile = { TILE_SIZE * 1.5, TILE_SIZE * 1.5 },
  .next_tile = { TILE_SIZE * 1.5, TILE_SIZE * 2.5 }
};

float distance_between_points(float x, float y, float x2, float y2);

void render_player() {
  draw_rect(
    MINIMAP_SCALE_FACTOR * player.x,
    MINIMAP_SCALE_FACTOR * player.y,
    MINIMAP_SCALE_FACTOR * player.width,
    MINIMAP_SCALE_FACTOR * player.height,
    0xFFFF9999
  );
  draw_rect(
    MINIMAP_SCALE_FACTOR * player.next_tile[0],
    MINIMAP_SCALE_FACTOR * player.next_tile[1],
    MINIMAP_SCALE_FACTOR * player.width,
    MINIMAP_SCALE_FACTOR * player.height,
    0xFF0000FF
  );
}

float angle_between_points(float x1, float y1, float x2, float y2) {
  return atan2f(y2 - y1, x2 - x1);
}

void move_player(float delta_time) {
  // Note the delta time determines the speed of animation
  player.facing += player.turn_direction * player.rotation_speed * delta_time;

  // Calculate point for the next step
  float step = player.walk_direction * player.movement_speed * delta_time;
  float next_pos_x = player.x + cos(player.facing) * step;
  float next_pos_y = player.y + sin(player.facing) * step;

  player.x = next_pos_x;
  player.y = next_pos_y;

  float angle = angle_between_points(player.x, player.y, player.next_tile[0], player.next_tile[1]);
  player.facing = angle;
  /* printf("%f\n", angle); */


  // Check wall collisions
  /* if (get_map_tile_at(next_pos_x, next_pos_y) == 0) { */
  /*   player.x = next_pos_x; */
  /*   player.y = next_pos_y; */
  /* } */

  if (distance_between_points(player.x, player.y, player.next_tile[0], player.next_tile[1]) > 10) return;

  get_random_neighbor(player.x, player.y, player.next_tile, player.prev_tile);
  player.prev_tile[0] = player.x;
  player.prev_tile[1] = player.y;
}
