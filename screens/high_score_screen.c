#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"
#include "../include/buttons.h"
#include "../include/eeprom.h"

void itosss(int num, char *buffer) {
    int pos = 0;

    // zeros won't display otherwise.
    if (num == 0) {
        buffer[pos++] = 48;
    } else {
        while (num != 0) {
            buffer[pos++] = num % 10 + 48;
            num /= 10;
        }
    }

    char from = 0, to = pos - 1;
    while (from < to) {
        char temp = buffer[from];
        buffer[from++] = buffer[to];
        buffer[to--] = temp;
    }
    buffer[pos] = '\0';
}

void high_score_screen() {
    char *screens[] = {
        "HIGHSCORE: NORMAL",
        "HIGHSCORE: HARD"
    };

    char current_screen = 0;

    short name_addrs[2][4] = {
        {ADDR_NORMAL1_NAME, ADDR_NORMAL2_NAME, ADDR_NORMAL3_NAME, ADDR_NORMAL4_NAME},
        {ADDR_HARD1_NAME, ADDR_HARD2_NAME, ADDR_HARD3_NAME, ADDR_HARD4_NAME}
    };
    short score_addrs[2][4] = {
        {ADDR_NORMAL1_SCORE, ADDR_NORMAL2_SCORE, ADDR_NORMAL3_SCORE, ADDR_NORMAL4_SCORE},
        {ADDR_HARD1_SCORE, ADDR_HARD2_SCORE, ADDR_HARD3_SCORE, ADDR_HARD4_SCORE}
    };

    // [mode (normal/easy)][position (1-4)][len of string]
    char names[2][4][7], scores[2][4][10];
    char name_scores[2][4][30];
    char mode, place;
    for (mode = 0; mode < 2; mode++) {
        for (place = 0; place < 4; place++) {
            eeprom_read_str(name_addrs[mode][place], names[mode][place]);
            if (names[mode][place][0] == '0') {
                name_scores[mode][place][0] = '\0';
                continue;
            }
            int temp = eeprom_read_int(score_addrs[mode][place]);
            itosss(temp, scores[mode][place]);

            // add name and score into name_scores
            char pos1 = 0, pos2 = 0;
            // name
            while (names[mode][place][pos1]) name_scores[mode][place][pos1++] = names[mode][place][pos2++];
            name_scores[mode][place][pos1++] = ':';
            pos2 = 0;
            // score
            while (names[mode][place][pos1]) name_scores[mode][place][pos1++] = scores[mode][place][pos2++];
            name_scores[mode][place][pos1] = '\0';
        }
    }

    while (1) {
        draw_clear();
        draw_string_grid(screens[current_screen], 0, CENTER);

        draw_string_grid(name_scores[current_screen][0], 15, LEFT);
        draw_string_grid(name_scores[current_screen][1], 15, RIGHT);
        draw_string_grid(name_scores[current_screen][2], 25, LEFT);
        draw_string_grid(name_scores[current_screen][3], 25, RIGHT);

        // draw_string_grid(names[current_screen][0], 15, LEFT);
        // draw_string_grid(scores[current_screen][0], 15, RIGHT);
        // draw_string_grid(names[current_screen][1], 25, LEFT);
        // draw_string_grid(scores[current_screen][1], 25, RIGHT);

        // go left
        if (btn4_ispressed() && current_screen != 0) {
            while (btn4_ispressed());
            current_screen--;
        }

        // go right
        if (btn3_ispressed() && current_screen != 1) {
            while (btn3_ispressed());
            current_screen++;
        }

        // go back
        if (btn2_ispressed()) {
            while (btn2_ispressed());
            return;
        }

        draw_canvas();
        delay(10);
    }
}
