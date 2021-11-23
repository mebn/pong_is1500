#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>

typedef enum {
    SINGLEPLAYER,
    MULTIPLAYER
} game_mode;

void game_screen(game_mode mode);
void high_score_screen();

#endif