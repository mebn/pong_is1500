#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

void display_init();

void draw_pixel(char x, char y);
void draw_text(unsigned int x, unsigned int y, char *s);

void draw_clear();
void draw_canvas();

void display_image(const uint8_t *data);

#endif