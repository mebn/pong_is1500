#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/display.h"

void high_score_screen() {
    while (1) {
        draw_clear();
        draw_string_grid("buffer", 10, CENTER);
        draw_canvas();
        delay(100);
    }
}
