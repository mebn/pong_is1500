#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"
#include "../include/buttons.h"

void high_score_screen() {
    char *screens[] = {
        "HIGHSCORE: EASY",
        "HIGHSCORE: NORMAL",
        "HIGHSCORE: HARD",
        "HIGHSCORE: IMPOSSIBLE"
    };

    char current_selection = 0;

    while (1) {
        draw_clear();
        draw_string_grid(screens[current_selection], 0, CENTER);
        Text_info easy = draw_string_grid("NAME 10", 15, LEFT);
        Text_info normal = draw_string_grid("NAME 8", 25, LEFT);
        Text_info hard = draw_string_grid("NAME 6", 15, RIGHT);
        Text_info impossible = draw_string_grid("NAME 4", 25, RIGHT);

        // go left
        if (btn4_ispressed() && current_selection != 0) {
            while (btn4_ispressed());
            current_selection--;
        }

        // go right
        if (btn3_ispressed() && current_selection != 3) {
            while (btn3_ispressed());
            current_selection++;
        }

        // go back
        if (btn1_ispressed()) {
            while (btn1_ispressed());
            return;
        }

        draw_canvas();
        delay(10);
    }
}
