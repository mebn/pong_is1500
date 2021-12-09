#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"
#include "../include/timer.h"

void input_name_screen() {
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
            current_selection--;
            while (btn4_ispressed());
        }

        // right
        if (btn3_ispressed()) {
            current_selection++;
            while (btn3_ispressed());
        }

        // char up
        if (btn2_ispressed() && name[current_selection] != 'Z') {
            name[current_selection]++;
            while (btn2_ispressed());
        }

        // char down
        if (btn1_ispressed() && name[current_selection] != 'A' - 1) {
            name[current_selection]--;
            while (btn1_ispressed());
        }

        if (current_selection == 6) {
            // add name to highscore list
            break;
        }

        draw_canvas();
        delay(10);
    }
}
