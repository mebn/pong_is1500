#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>

/**
 * @brief There's two different
 * game modes, singleplayer and multiplayer.
 * 
 */
typedef enum {
    SINGLEPLAYER,
    MULTIPLAYER
} game_mode;

void game_screen(game_mode mode);
void high_score_screen();

#endif