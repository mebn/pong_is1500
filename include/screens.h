#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>

#define PADDLESIZE_X 2
#define PADDLESIZE_Y 10
#define PADDLEGAP 0
#define PADDLESPEED 1

#define BALLSIZE 2
#define BALLSPEED 1
#define FREEZETIME 50

#define TOLERANCE 0.001

/**
 * @brief There's two different
 * game modes, singleplayer and multiplayer.
 * 
 */
typedef enum {
    SINGLEPLAYER,
    MULTIPLAYER
} game_mode;

/**
 * Written by: Marcus Nilszén
 * 
 * @brief Enum containing the dirrerent
 * game difficulties.
 * 
 */
typedef enum {
    EASY,
    NORMAL,
    HARD,
    IMPOSSIBLE
} game_difficulty;

void game_screen(game_mode mode);
void high_score_screen();
void input_name_screen(char score, game_difficulty difficulty);

#endif