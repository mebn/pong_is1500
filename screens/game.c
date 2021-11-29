#include <stdbool.h>
#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/buttons.h"
#include "../include/display.h"

/**
 * Written by: Marcus Nilszén
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

/**
 * @brief The ball object.
 * 
 */
typedef struct {
    double x_pos;
    double y_pos;
    char size;
    int x_speed;
    int y_speed;
} Ball;

/**
 * @brief 
 * 
 * @param ball 
 */
void draw_ball(Ball *ball) {
    char size = ball->size;
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            draw_pixel(ball->x_pos + i, ball->y_pos + j);
        }
    }
}

void ball_update(Ball *ball) {
    if (ball->y_pos + ball->y_speed < 0 || ball->y_pos + ball->y_speed >= DISPLAY_HEIGHT) {
        ball->y_speed *= -1;
    }

    if (ball->x_pos > 128 + 50 || ball->x_pos < 0 - 50) ball->x_pos = 128/2;

    ball->y_pos += ball->y_speed;
    ball->x_pos += ball->x_speed;
}

/**
 * @brief The paddle object. 
 * 
 */
typedef struct {
    double x_pos;
    double y_pos;
    char x_size;
    char y_size;
} Paddle;

/**
 * @brief 
 * 
 * @param paddle 
 */
void draw_paddle(Paddle *paddle) {
    char x, y;
    char xStart = paddle->x_pos, yStart = paddle->y_pos;
    char xSize = paddle->x_size, ySize = paddle->y_size;
    for (x = 0; x < xSize; x++) {
        for(y = 0; y < ySize; y++) {
            draw_pixel(xStart + x, yStart + y);
        }
    }
}

typedef enum {UP, DOWN} move_dir;

void move_paddle(Paddle *p, move_dir md) {
    if (md == UP && p->y_pos > 0) {
        p->y_pos -= 1; 
    } else if (md == DOWN && p->y_pos + p->y_size < 31) {
        p->y_pos += 1; 
    }
};

void ball_bounce(Ball *b) {
    b->x_speed *= -1;
}


void ball_collision(Ball *b, Paddle *p) {
    if (p->x_pos <= b->x_pos && p->x_pos + p->x_size >= b->x_pos + b->size &&
        p->y_pos <= b->y_pos && p->y_pos + p->y_size >= b->y_pos + b->size) {
        ball_bounce(b);
    }

    // char paddleUp, paddleDown, paddleLeft, paddleRight;
    // char ballUp, ballDown, ballLeft, ballRight;
    // paddleUp = p->y_pos;
    // paddleDown = p->y_pos + p->y_size;
    // paddleLeft = p->x_pos;
    // paddleRight = p->x_pos + p->x_size;
    // ballUp = b->y_pos;
    // ballDown = b->y_pos + b->size;
    // ballLeft = b->x_pos;
    // ballRight = b->x_pos + b->size;
    // if (paddleUp > ballDown) return;
    // if (ballUp > paddleDown) return;
    // if (paddleLeft > ballRight) return;
    // if (ballLeft > paddleRight) return;
    // ball_bounce(b);
}

/**
 * Written by: Marcus Nilszén
 * 
 * @brief Singleplayer difficulty selection menu.
 * 
 * @return game_difficulty Enum value. The difficulty
 * of a singleplayer game.
 */
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

/**
 * Written by: Alex Gunnarsson & Marcus Nilszén
 * 
 * @brief Run game routine for singleplayer and multiplayer gamemode.
 * 
 * @param mode Enum value for playing singleplayer or multiplayer.
 * Determines if other paddle is player- or AI-controlled, respectively.
 * @param difficulty Enum value deciding difficult level
 * in singleplayer vs AI. Has no effect when mode is set to multiplayer.
 */
void game_screen(game_mode mode) {
    game_difficulty difficulty;

    if (mode == SINGLEPLAYER) {
        difficulty = difficulty_selection();
    }

    Ball ball = {
        .x_pos = 0,
        .y_pos = 0,
        .size = BALLSIZE,
        .x_speed = 1,
        .y_speed = 1
    };
    
    Paddle p1 = {
        .x_pos = 0 + PADDLEGAP,
        .y_pos = 32/2 - PADDLESIZE_Y/2,
        .x_size = PADDLESIZE_X,
        .y_size = PADDLESIZE_Y
    };
    
    Paddle p2 = {
        .x_pos = 128 - PADDLESIZE_X - PADDLEGAP,
        .y_pos = 32/2 - PADDLESIZE_Y/2,
        .x_size = PADDLESIZE_X,
        .y_size = PADDLESIZE_Y
    };

    if (mode == SINGLEPLAYER && difficulty == IMPOSSIBLE) {
        p2.y_pos = 0;
        p2.y_size = 32;
    }

    while (1) {
        draw_clear();
        
        // player 1
        if (!(btn4_ispressed() && btn3_ispressed())) {
            if (btn4_ispressed()) move_paddle(&p1, UP);
            if (btn3_ispressed()) move_paddle(&p1, DOWN);
        }
        
        // player2 movement
        if (mode == MULTIPLAYER) {
            if (!(btn1_ispressed() && btn2_ispressed())) {
                if (btn1_ispressed()) move_paddle(&p2, UP);
                if (btn2_ispressed()) move_paddle(&p2, DOWN);
            }
        } else {
            // AI
            p2.y_pos = ball.y_pos;
        }

        draw_paddle(&p1);
        draw_paddle(&p2);

        ball_collision(&ball, &p1);
        ball_collision(&ball, &p2);

        ball_update(&ball);
        draw_ball(&ball);

        draw_canvas();

        delay(10);
    }
}
