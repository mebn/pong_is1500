#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"
#include "../include/buttons.h"
#include "../include/eeprom.h"
#include "../include/tools.h"

/**
 * Written by: Marcus Nilszén
 * 
 * @brief The leaderboard for different difficulty
 * levels. Shows top 4 players (names and scores).
 * 
 */
void high_score_screen() {
    char *screens[DIFFICULTYLEVELS] = {
        "HIGHSCORE: EASY",
        "HIGHSCORE: NORMAL",
        "HIGHSCORE: HARD",
        "HIGHSCORE: IMPOSSIBLE"
    };

    char current_screen = 0;
    char entries[DIFFICULTYLEVELS][TOPNPLAYERS][15];
    
    char mode, place;
    for (mode = 0; mode < DIFFICULTYLEVELS; mode++) {
        for (place = 0; place < TOPNPLAYERS; place++) {
            char temp_name[7], temp_score[7];
            eeprom_read_str(name_addrs[mode][place], temp_name);
            
            if (temp_name[0] == '0') {
                entries[mode][place][0] = '-';
                entries[mode][place][1] = '\0';
                continue;
            }

            char temp = eeprom_read(score_addrs[mode][place]);
            itos(temp, temp_score);

            // add name and score into entries
            char entry_pos = 0, name_score_pos = 0;
            
            // name
            while (temp_name[name_score_pos])
                entries[mode][place][entry_pos++] = temp_name[name_score_pos++];
            
            entries[mode][place][entry_pos++] = ':';
            entries[mode][place][entry_pos++] = ' ';
            name_score_pos = 0;
            
            // score
            while (temp_score[name_score_pos])
                entries[mode][place][entry_pos++] = temp_score[name_score_pos++];
            entries[mode][place][entry_pos] = '\0';
        }
    }

    while (1) {
        draw_clear();

        Text_info ti = draw_string_grid(screens[current_screen], 1, CENTER);
        display_invert_ti(&ti);

        // displays top 4 players
        draw_string_grid(entries[current_screen][0], 15, LEFT);
        draw_string_grid(entries[current_screen][1], 15, RIGHT);
        draw_string_grid(entries[current_screen][2], 25, LEFT);
        draw_string_grid(entries[current_screen][3], 25, RIGHT);

        // go left
        if (btn4_ispressed()) {
            while (btn4_ispressed());
            if (current_screen == 0) current_screen = DIFFICULTYLEVELS - 1;
            else current_screen--;
        }

        // go right
        if (btn3_ispressed()) {
            while (btn3_ispressed());
            if (current_screen == DIFFICULTYLEVELS - 1) current_screen = 0;
            else current_screen++;
        }

        // go back
        if (btn2_ispressed()) {
            while (btn2_ispressed());
            return;
        }

        // reset leaderboard
        if (btn1_ispressed()) {
            while (btn1_ispressed());
            eeprom_reset(current_screen);
        }

        draw_canvas();
        delay(10);
    }
}
