#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

void display_init();

/**
 * Written by: Marcus Nilszén
 * 
 *  @brief The display is divided in
 * 4 quadrants and text in these
 * quadrants are centered. Not to be confused with 
 * grid_pos.CENTER which centers text
 * on x-axis on display.
 * 
 */
typedef enum {
    LEFT,
    CENTER,
    RIGHT
} grid_pos;

/**
 * Written by: Marcus Nilszén
 * 
 * @brief Used when building menus
 * to make it easy to draw a selection
 * underline.
 * 
 */
typedef struct {
    char x;
    char y;
    int len;
} Text_info;

void draw_underline(const Text_info *ti);
void draw_string(char *str, unsigned int x, unsigned int y);
void draw_string_spacing(char *str, unsigned int x, unsigned int y, int spacing);
Text_info draw_string_grid(char *str, unsigned int y, grid_pos pos);

void draw_pixel(char x, char y);
void draw_line(char x, char y, char length, char thickness);

void display_invert(char xStart, char yStart, char xEnd, char yEnd);
void display_invert_ti(Text_info *ti);

void draw_clear();
void draw_canvas();

void display_image(const uint8_t *data);
void display_fullscreen_image(unsigned int x, const uint8_t *data);

#endif