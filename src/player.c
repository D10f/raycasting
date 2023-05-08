#include "../include/player.h"
#include <math.h>
#include <stdlib.h>

player_t player = {
  /* .x = WINDOW_WIDTH / 2, */
  /* .y = WINDOW_HEIGHT / 2, */
  /* .x = TILE_SIZE * 2 - (float)TILE_SIZE / 2, */
  /* .y = TILE_SIZE * 2 - (float)TILE_SIZE / 2, */
  .x = TILE_SIZE * 6.5,
  .y = TILE_SIZE * 1.5,
  .width = 10,
  .height = 10,
  .turn_direction = 0,
  .walk_direction = 1,
  .facing = PI,
  .prev_facing = PI,
  .movement_speed = 100,
  .rotation_speed = 90 * (PI / 180),  // 45deg, converted to radians
  .prev_tile = { TILE_SIZE * 6.5, TILE_SIZE * 1.5 },
  .next_tile = { TILE_SIZE * 5.5, TILE_SIZE * 1.5 }
};

float distance_between_points(float x, float y, float x2, float y2);
float normalize_angle(float angle);

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

void facing_direction(float angle) {
  /* angle = normalize_angle(angle); */

  bool facing_south = angle >= 0 && angle <= PI;
  bool facing_north = angle >= PI && angle <= TWO_PI;
  bool facing_east  = angle <= PI * 0.5 && angle >= PI + PI * 0.5;
  bool facing_west  = angle >= PI * 0.5 && angle <= PI + PI * 0.5;

  printf("south: %d\n", facing_south);
  printf("north: %d\n", facing_north);
  printf("east: %d\n", facing_east);
  printf("west: %d\n", facing_west);
}

void move_player(float delta_time) {
  /* float angle = angle_between_points(player.x, player.y, player.next_tile[0], player.next_tile[1]); */
  /* player.facing += player.turn_direction * player.rotation_speed * delta_time; */

  float angle = angle_between_points(player.x, player.y, player.next_tile[0], player.next_tile[1]);

  if (player.turn_direction != 0) {
    player.facing += player.turn_direction * player.rotation_speed * delta_time;
    if (fabsf(player.facing - player.prev_facing) < PI / 2) return;
    player.x = floor(player.x / TILE_SIZE) * TILE_SIZE + 32;
    player.y = floor(player.y / TILE_SIZE) * TILE_SIZE + 32;
    player.turn_direction = 0;
    player.facing = angle;
  }

  // Calculate point for the next step
  float step = player.walk_direction * player.movement_speed * delta_time;
  float next_pos_x = player.x + cos(player.facing) * step;
  float next_pos_y = player.y + sin(player.facing) * step;

  player.x = next_pos_x;
  player.y = next_pos_y;

  if (distance_between_points(player.x, player.y, player.next_tile[0], player.next_tile[1]) > 10) return;

  get_random_neighbor(player.x, player.y, player.next_tile, player.prev_tile);

  // Update player's position once more to account for the 10px radius to detect target tile
  player.x = player.x + cos(player.facing) * step;
  player.y = player.y + sin(player.facing) * step;

  angle = angle_between_points(player.x, player.y, player.next_tile[0], player.next_tile[1]);

  if (fabsf(player.facing - angle) > 0.2) {
    if (player.facing > angle) {
      if ((int)player.facing == 3 && angle < 0) {
        player.turn_direction = 1;
      } else {
        player.turn_direction = -1;
      }
    } else {
      player.turn_direction = 1;
    }
    player.x = floor(player.x / TILE_SIZE) * TILE_SIZE + 32;
    player.y = floor(player.y / TILE_SIZE) * TILE_SIZE + 32;
  }

  player.prev_tile[0] = player.x;
  player.prev_tile[1] = player.y;
  player.prev_facing = player.facing;
}
