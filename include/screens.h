#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>

typedef enum {
    SINGLEPLAYER,
    MULTIPLAYER
} game_mode;

typedef enum {
    EASY,
    NORMAL,
    HARD,
    IMPOSSIBLE
} game_difficulty;

void game_screen(game_mode mode, game_difficulty difficulty);
void high_score_screen();

#endif