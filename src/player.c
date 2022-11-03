#include "../include/player.h"

player_t player = {
  .x = WINDOW_WIDTH / 2,
  .y = WINDOW_HEIGHT / 2,
  .width = 10,
  .height = 10,
  .turn_direction = 0,
  .walk_direction = 0,
  .facing = PI / 2,
  .movement_speed = 100,
  .rotation_speed = 90 * (PI / 180)  // 45deg, converted to radians
};

void render_player() {
  draw_rect(
    MINIMAP_SCALE_FACTOR * player.x,
    MINIMAP_SCALE_FACTOR * player.y,
    MINIMAP_SCALE_FACTOR * player.width,
    MINIMAP_SCALE_FACTOR * player.height,
    0xFFFF9999
  );
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
