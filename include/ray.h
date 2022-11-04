#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <limits.h>
#include "defs.h"
#include "map.h"
#include "player.h"


/**
 * Stores information about each individual ray casted.
 */
typedef struct {
  float angle;              ///< Angle of the ray.
  float length;             ///< Length of the ray.
  float target_x;           ///< x coordinate where the ray hit a wall.
  float target_y;           ///< y coordinate where the ray hit a wall.
  bool hits_vertically;     ///< whether the ray hits a vertical intersection.
  int target_wall_material; ///< material of the wall the ray hit.
} ray_t;

// Tells the compiler this is a global variable
extern ray_t rays[NUM_RAYS];


/**
 * Turns the given angle into a positive float and within 360 degrees.
 * @param[in] angle Angle of the ray, as emitted from player's position.
 * @return The normalized angle.
 *
 */
float normalize_angle(float angle);


/**
 * Calculates the distance between two points.
 * @param[in] x1 The x coordinate of the starting point.
 * @param[in] y1 The y coordinate of the starting point.
 * @param[in] x2 The x coordinate of the ending point.
 * @param[in] y2 The y coordinate of the ending point.
 * @return The distance between two points.
 *
 */
float distance_between_points(float x1, float y1, float x2, float y2);


/**
 * Calculates the properties of a ray once it's emitted.
 * Rays are emitted from the player's position. Properties like the length and
 * target_x are calculated based on whether the ray hits a wall that's closer
 * or further away. Upon hitting, the ray stores also information about the
 * wall, such as it's position and material.
 * @param angle The angle of the ray as originated from the player.
 * @param idx The index of the ray to be updated.
 *
 */
void cast_ray(float angle, int idx);


/**
 * Draws each ray as a line in the minimap.
 *
 */
void render_rays(void);


/**
 * Cast all rays to create a projection of the map.
 * For each ray, the angle is calculated based on the player's orientation and
 * distance to the projected plane.
 * 
 */
void create_scene(void);

#endif
