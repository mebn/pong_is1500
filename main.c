#include <pic32mx.h>
#include "include/display.h"
#include "include/graphics.h"
#include "include/isr.h"
#include "include/timer.h"
#include "include/buttons.h"
#include "include/screens.h"
#include <stdbool.h>

/**
 * Written by Marcus Nilszén
 * 
 * @brief The different screens a user
 * can navigate to after bootup and games.
 * Not including subscreens from screens.
 * 
 */
typedef enum {
    SINGLE_PLAYER,
    MULTI_PLAYER,
    HIGH_SCORE,
} selection;

/**
 * Written by Marcus Nilszén
 * 
 * @brief The main menu a user see after bootup.
 * Button 4 and 3 to navigate up respectively down in the menu.
 * Button 1 to select and navigate to the selected screen.
 * (NEEDS IMPROVMENT)
 * 
 * @return selection An enum value indicating what screen
 * to navigate to.
 */
selection menu() {
    selection current_selection = SINGLE_PLAYER;

    while (1) {
        draw_clear();

        draw_string_align_menu("PONG!", 0, CENTER);
        Text_info single = draw_string_align_menu("SINGLEPLAYER", 15, LEFT);
        Text_info multi = draw_string_align_menu("MULTIPLAYER", 15, RIGHT);
        Text_info high = draw_string_align_menu("HIGHSCORE", 25, CENTER);

        Text_info options[] = {single, multi, high};
        draw_underline(&options[current_selection]);

        // up
        if (btn4_ispressed() && current_selection != SINGLE_PLAYER) {
            current_selection--;
        }

        // down
        if (btn3_ispressed() && current_selection != HIGH_SCORE) {
            current_selection++;
        }

        // select
        if (btn1_ispressed()) {
            return current_selection;
        }

        draw_canvas();
        delay(100); // this causes som issues
    }
}

/**
 * Written by Marcus Nilszén
 * 
 * @brief Displays a loading image on bootup and
 * delays it for some specified time.
 * 
 */
void loading() {
    display_fullscreen_image(0, pong_image);
    delay(1500);
}

/**
 * @brief Collection of all Initializations
 * from different files and hardware.
 * 
 */
void inits() {
    timer2_init();
    buttons_init();
    isr_init();
    display_init();
}

/**
 * @brief Entrypoint of our program.
 * Loads inits, loading screen and
 * displays the main menu.
 * 
 */
int main() {
    inits();
    loading();
    
    draw_clear();
    draw_canvas();

    while (1) {
        selection screen = menu();

        if (screen == SINGLE_PLAYER) game_screen(SINGLE_PLAYER);
        if (screen == MULTI_PLAYER) game_screen(MULTI_PLAYER);
        if (screen == HIGH_SCORE) high_score_screen();
    }

    return 0;
}
