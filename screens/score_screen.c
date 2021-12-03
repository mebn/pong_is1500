#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"
#include "../include/timer.h"

void score_screen() {
    int current_selection = 0;
    char *name[] = {"A", "A", "A", "A", "A", "A"};

    while (1) {
        draw_clear();

        draw_string_grid("NEW SCORE", 0, CENTER);
        int i, spacing = 3, len = 5, y = 20;
        int x = 35;
        draw_string("NAME:", 5, y-6);

        // all underlines
        for (i = 0; i < 6; i++) {
            draw_line(x + i*(len+spacing), y, len, 1);
            draw_string(name[i], x + i*(len+spacing), y - 6);
        }

        draw_line(x + current_selection*(len+spacing), y, len, 2);

        // left
        if (btn4_ispressed() && current_selection != 0) {
            current_selection--;
        }

        // right
        if (btn3_ispressed()) {
            current_selection++;
        }

        // char up
        if (btn2_ispressed() && name[current_selection] != "Z") {
            *name[current_selection]++;
        }

        // char down
        if (btn1_ispressed() && name[current_selection] != "A") {
            name[current_selection]--;
        }

        if (current_selection == 6) {
            // add name to highscore list
            break;
        }

        draw_canvas();
        delay(100);
    }
}


/*
game_difficulty difficulty_selection() {
    game_difficulty current_selection = EASY;

    while (1) {
        delay(100); // this causes som issues
        draw_clear();

        draw_string_grid("DIFFICULTY", 0, CENTER);
        Text_info easy = draw_string_grid("EASY", 15, LEFT);
        Text_info normal = draw_string_grid("NORMAL", 25, LEFT);
        Text_info hard = draw_string_grid("HARD", 15, RIGHT);
        Text_info impossible = draw_string_grid("IMPOSSIBLE", 25, RIGHT);

        Text_info options[] = {easy, normal, hard, impossible};
        draw_underline(&options[current_selection]);

        // up
        if (btn4_ispressed() && current_selection != EASY) {
            current_selection--;
        }

        // down
        if (btn3_ispressed() && current_selection != IMPOSSIBLE) {
            current_selection++;
        }

        // select
        if (btn1_ispressed()) {
            return current_selection;
        }

        draw_canvas();
    }
}
*/