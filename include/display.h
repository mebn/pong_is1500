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

void draw_string(const char *str, unsigned int x, unsigned int y);
void draw_string_align(const char *str, unsigned int y, alignment align);

void draw_pixel(char x, char y);
void draw_text(unsigned int x, unsigned int y, char *s);

void display_invert(char xStart, char yStart, char xEnd, char yEnd);

void draw_clear();
void draw_canvas();

void display_image(const uint8_t *data);

#endif