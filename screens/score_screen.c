#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"
#include "../include/timer.h"

void score_screen() {
    int current_selection = 0;
    char name[] = "AAAAAA";
    
    while (1) {
        draw_clear();
        
        draw_string_grid("NEW SCORE", 0, CENTER);
        int i, spacing = 3, len = 5, y = 20;
        int x = 35;
        draw_string("NAME:", 5, y-6);

        // all underlines
        for (i = 0; i < 6; i++) {
            draw_line(x + i*(len+spacing), y, len, 1);
        }
        draw_string_spacing(&name, x, y - 6, spacing+1);

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
        if (btn2_ispressed() && name[current_selection] != 'Z') {
            name[current_selection]++;
        }

        // char down
        if (btn1_ispressed() && name[current_selection] != 'A') {
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
