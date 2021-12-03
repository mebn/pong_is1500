#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"
#include "../include/timer.h"

void score_screen() {
    while (1) {
        draw_clear();

        draw_string_grid("new score", 0, CENTER);

        char start_x = 49, margin = 1, len = 4;
        draw_line(start_x + 0*(margin+len), 10, 4, 1);
        draw_line(start_x + 1*(margin+len), 10, 4, 1);
        draw_line(start_x + 2*(margin+len), 10, 4, 2);
        draw_line(start_x + 3*(margin+len), 10, 4, 1);
        draw_line(start_x + 4*(margin+len), 10, 4, 1);
        draw_line(start_x + 5*(margin+len), 10, 4, 1);

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