#include "include/display.h"
#include "include/graphics.h"
#include "include/isr.h"
#include "include/timer.h"
#include "include/buttons.h"
#include "include/screens.h"
#include "include/eeprom.h"
#include <stdbool.h>

/**
 * Written by: Marcus Nilszén
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
 * Written by: Marcus Nilszén
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

        draw_string_grid("PONG!", 0, CENTER);
        Text_info single = draw_string_grid("SINGLEPLAYER", 15, LEFT);
        Text_info multi = draw_string_grid("MULTIPLAYER", 15, RIGHT);
        Text_info high = draw_string_grid("HIGHSCORE", 25, CENTER);

        Text_info options[] = {single, multi, high};
        display_invert_ti(&options[current_selection]);

        // up
        if (btn4_ispressed()) {
            while (btn4_ispressed());
            if (current_selection == SINGLE_PLAYER) {
                current_selection = HIGH_SCORE;  
            } else {
                current_selection--; 
            }
        }

        // down
        if (btn3_ispressed()) {
            while (btn3_ispressed());
            if (current_selection == HIGH_SCORE) {
                current_selection = SINGLE_PLAYER;
            } else {
                current_selection++;
            }
        }

        // select
        if (btn1_ispressed()) {
            while (btn1_ispressed());
            return current_selection;
        }

        draw_canvas();
        delay(10);
    }
}

/**
 * Written by: Marcus Nilszén
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
    eeprom_init();
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
