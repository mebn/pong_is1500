#include <pic32mx.h>
#include "include/display.h"
#include "include/graphics.h"
#include "include/isr.h"
#include "include/timer.h"
#include "include/buttons.h"
#include "include/screens.h"

typedef enum {
    SINGLE_PLAYER,
    MULTI_PLAYER,
    HIGH_SCORE,
} selection;

selection menu() {
    selection current_selection = SINGLE_PLAYER;
    int y_pos = 8;

    while (1) {
        draw_clear();
        
        // draw_text(0, 0, "PONG!");
        draw_text(30, 8, "1 player");
        draw_text(30, 16, "2 player");
        draw_text(30, 24, "highscore");

        // selection indicator
        draw_pixel(0, y_pos);

        // up
        if (get_btn4() && y_pos != 8) {
            y_pos -= 8;
            current_selection--;
        }

        // down
        if (get_btn3() && y_pos != 24) {
            y_pos += 8;
            current_selection++;
        }

        // select
        if (get_btn1()) {
            return current_selection;
        }

        draw_canvas();
        delay(100);
    }
}

void loading() {
    display_fullscreen_image(0, mario);
    delay(1500);
}

void inits() {
    timer2_init();
    buttons_init();
    isr_init();
    display_init();
}

int main() {
    inits();
    loading();
    
    draw_clear();
    draw_canvas();

    while (1) {
        selection screen = menu();

        if (screen == SINGLE_PLAYER) single_player_screen();
        if (screen == MULTI_PLAYER) multi_player_screen();
        if (screen == HIGH_SCORE) high_score_screen();
    }

    return 0;
}
