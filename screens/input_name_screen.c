#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"
#include "../include/timer.h"
#include "../include/eeprom.h"

// IMPROVE
void add_to_eeprom(char *name, int score, game_mode gm) {
    short name_addrs[] = {ADDR_EASY1_NAME, ADDR_EASY2_NAME, ADDR_EASY3_NAME, ADDR_EASY4_NAME};
    short score_addrs[] = {ADDR_EASY1_SCORE, ADDR_EASY2_SCORE, ADDR_EASY3_SCORE, ADDR_EASY4_SCORE};
    char pos = -1;
    int i;
    for (i = 3; i >= 0; i--) {
        char eeprom_score[10];
        eeprom_read_str(score_addrs[i], eeprom_score);
        if (score >= eeprom_score) pos = i; // problem comp. int to char
    }
    if (pos != -1) {
        eeprom_write_str(name_addrs[pos], name);
        eeprom_write_str(score_addrs[pos], &score);
    }

}

void input_name_screen(int score, game_mode gm) {
    int current_selection = 0;
    char name[] = {'A'-1, 'A'-1, 'A'-1, 'A'-1, 'A'-1, 'A'-1, '\0'};
    
    char i, spacing = 3, len = 5, x = 43, y = 20;
    while (1) {
        draw_clear();
        
        draw_string_grid("ENTER YOUR NAME", 0, CENTER);
        for (i = 0; i < 6; i++) {
            char temp[] = {name[i], '\0'};
            draw_string_spacing(&temp, x+i*(len+spacing), y - 6, spacing);
            draw_line(x+i*(len+spacing), y, len, 1);
        }

        // draw extra line under selected char.
        draw_line(x + current_selection*(len+spacing), y, len, 2);

        // left
        if (btn4_ispressed() && current_selection != 0) {
            while (btn4_ispressed());
            current_selection--;
        }

        // right
        if (btn3_ispressed()) {
            while (btn3_ispressed());
            current_selection++;
        }

        // char up
        if (btn2_ispressed() && name[current_selection] != 'Z') {
            while (btn2_ispressed());
            name[current_selection]++;
        }

        // char down
        if (btn1_ispressed() && name[current_selection] != 'A' - 1) {
            while (btn1_ispressed());
            name[current_selection]--;
        }

        if (current_selection == 6) {
            // add name and score to highscore list
            add_to_eeprom(name, score, gm);
            break;
        }

        draw_canvas();
        delay(10);
    }
}
