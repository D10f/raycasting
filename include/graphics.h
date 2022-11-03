#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "defs.h"

/**
 * Opens a new window to visualize the game.
 * Initializes the SDL window, renderer and texture to process and output the
 * game. It also initializes the color buffer used to draw pixels separately.
 *
 */
bool initialize_window(void);


/**
 * Frees the heap memory allocated during the duration of the program.
 * Calls to free the SDL window, renderer and texture instances, as well as
 * the color buffer.
 *
 */
void destroy_window(void);


/**
 * Resets the color buffer to a fixed color.
 * @param[in] color color used to fill each pixel in hexadecimal notation.
 *
 */
void clear_color_buffer(uint32_t color);


/**
 * Fills the texture buffer using data from the color buffer.
 * Copies over all the data from color buffer into the texture buffer. This is
 * how SDL achieves the "dual buffer" mechanism to render pixels to the screen.
 *
 */
void render_color_buffer(void);


/**
 * Paints a single pixel using the color variable provided.
 * @param[in] x The x coordinate of the pixel to be drawn
 * @param[in] y The y coordinate of the pixel to be drawn
 * @param[in] color The color to draw the pixel with.
 *
 */
void draw_pixel(int x, int y, uint32_t color);


/**
 * Draws a rectangle of a given size and color at the given coordinates.
 * @param[in] x The x coordinate to draw the rectangle.
 * @param[in] y The y coordinate to draw the rectangle.
 * @param[in] width The width of the rectangle.
 * @param[in] height The height of the rectangle.
 * @param[in] color The fill color of the rectangle.
 *
 */
void draw_rect(int x, int y, int width, int height, uint32_t color);


/**
 * Draws a line between two points
 * Uses the DDA (Digital Differential Analzer) algorithm
 * @param[in] x1 The x coordinate of the starting point.
 * @param[in] y1 The y coordinate of the starting point.
 * @param[in] x2 The x coordinate of the ending point.
 * @param[in] y2 The y coordinate of the ending point.
 * @param[in] color The fill color of the line.
 */
void draw_line(int x1, int y1, int x2, int y2, uint32_t color);

#endif
