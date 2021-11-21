#include <stdbool.h>
#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/buttons.h"

void multi_player_screen() {
    while (1) {
        draw_clear();
        
        // player1 movement
        if (get_btn4()) draw_pixel(1, 1);       // implement BTN4 as player 1 move up
        if (get_btn3()) draw_pixel(10, 1);      // implement BTN3 as player 1 move down
        
        // player2 movement
        if (get_btn1()) draw_pixel(20, 1);      // implement BTN1 as player 2 move up
        if (get_btn2()) draw_pixel(30, 1);      // implement BTN2 as player 2 move down

        static int x = 0;
        static int y = 0;
        static bool x_forward = true;
        static bool y_up = true;

        if (x > 125) x_forward = false;
        if (x < 1) x_forward = true;
        if (y > 30) y_up = false;
        if (y < 1) y_up = true;

        x_forward ? x++ : x--;
        y_up ? y++ : y--;

        draw_pixel(x, y);
        draw_pixel(x+1, y);
        draw_pixel(x, y+1);
        draw_pixel(x+1, y+1);

        draw_canvas();

        delay(10);
    }
}
