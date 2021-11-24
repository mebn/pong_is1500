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

typedef struct {
    char x;
    char y;
    int len;
} Text_info;

void draw_underline(const Text_info *ti);
void draw_string(char *str, unsigned int x, unsigned int y);
Text_info draw_string_align_menu(char *str, unsigned int y, alignment align);

void draw_pixel(char x, char y);

void display_invert(char xStart, char yStart, char xEnd, char yEnd);

void draw_clear();
void draw_canvas();

void display_image(const uint8_t *data);

#endif