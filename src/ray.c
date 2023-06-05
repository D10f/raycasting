#include "../include/ray.h"
#include <float.h>

ray_t rays[NUM_RAYS];

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

  bool facingSouth = angle > 0 && angle < PI;
  bool facingNorth = !facingSouth;
  bool facingEast  = angle < 0.5 * PI || angle > 1.5 * PI;
  bool facingWest  = !facingEast;

  float xintersect, yintersect, xstep, ystep;

  /*** HORIZONTAL RAY-GRID INTERSECTION ***/
  bool has_horizontal_target = false;
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

  while (is_inside_map(xintersect, yintersect)) {

    float yOffset = facingNorth ? yintersect - 1 : yintersect;

    if (get_map_tile_at(xintersect, yOffset) > 0) {
      has_horizontal_target = true;
      horizontal_target_x = xintersect;
      horizontal_target_y = yOffset;
      break;
    }

    xintersect += xstep;
    yintersect += ystep;
  }

  /*** VERTICAL RAY-GRID INTERSECTION ***/
  bool has_vertical_target = false;
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

  while (is_inside_map(xintersect, yintersect)) {

    float xOffset = facingWest ? xintersect - 1 : xintersect;

    if (get_map_tile_at(xOffset, yintersect) > 0) {
      has_vertical_target = true;
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
    rays[idx].hits_vertically = false;
  } else {
    rays[idx].length = vertical_dist;
    rays[idx].target_x = vertical_target_x;
    rays[idx].target_y = vertical_target_y;
    rays[idx].hits_vertically = true;
  }

  rays[idx].angle = angle;
  rays[idx].target_wall_material = get_map_tile_at(
    rays[idx].target_x,
    rays[idx].target_y
  );
}

void render_rays() {
  for (int i = 0; i < NUM_RAYS; i++) {
    draw_line(
      MINIMAP_SCALE_FACTOR * player.x,
      MINIMAP_SCALE_FACTOR * player.y,
      MINIMAP_SCALE_FACTOR * rays[i].target_x,
      MINIMAP_SCALE_FACTOR * rays[i].target_y,
      0x0044CCFF
    );
  }
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
