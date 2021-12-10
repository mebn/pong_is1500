#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"
#include "../include/buttons.h"
#include "../include/eeprom.h"

void high_score_screen() {
    char *screens[] = {
        "HIGHSCORE: EASY",
        "HIGHSCORE: NORMAL",
        "HIGHSCORE: HARD"
    };

    char current_selection = 0;

    // IMPROVE
    short name_addrs[] = {ADDR_EASY1_NAME, ADDR_EASY2_NAME, ADDR_EASY3_NAME, ADDR_EASY4_NAME};
    short score_addrs[] = {ADDR_EASY1_SCORE, ADDR_EASY2_SCORE, ADDR_EASY3_SCORE, ADDR_EASY4_SCORE};

    char name1[10];
    char score1[10];
    eeprom_read_str(name_addrs[0], name1);
    eeprom_read_str(score_addrs[0], score1);

    while (1) {
        draw_clear();
        draw_string_grid(screens[current_selection], 0, CENTER);
        draw_string_grid(name1, 15, LEFT);
        draw_string_grid(score1, 15, RIGHT);
        draw_string_grid("NAME 6", 25, LEFT);
        draw_string_grid("NAME 4", 25, RIGHT);

        // go left
        if (btn4_ispressed() && current_selection != 0) {
            while (btn4_ispressed());
            current_selection--;
        }

        // go right
        if (btn3_ispressed() && current_selection != 2) {
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
