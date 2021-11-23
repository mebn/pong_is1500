#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

void display_init();

typedef enum {
    LEFT,
    CENTER,
    RIGHT
} alignment;

void draw_string(char *str, unsigned int x, unsigned int y);
void draw_string_align(char *str, unsigned int y, alignment align);

void draw_pixel(char x, char y);

void display_invert(char xStart, char yStart, char xEnd, char yEnd);

void draw_clear();
void draw_canvas();

void display_image(const uint8_t *data);

#endif