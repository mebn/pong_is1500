#include <stdbool.h>
#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/buttons.h"
#include "../include/display.h"
#include "../include/tools.h"

typedef struct {
    float x_pos;
    float y_pos;
    char size;
    float x_speed;
    float y_speed;
} Ball;

typedef struct {
    int x_pos;
    int y_pos;
    char x_size;
    char y_size;
    int score;
} Paddle;

/**
 * @brief Draw the ball used in game.
 * 
 * @param ball Ball struct.
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

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Update ball position and speed depending on 
 * if ball is going to hit roof or floor or collide with paddles.
 * 
 * @param ball Ball struct.
 * @param p1 Left paddle struct.
 * @param p2 Right paddle struct.
 */
void ball_update(Ball *ball, Paddle *p1, Paddle *p2) {
    // Bounce off roof and floor
    if (ball->y_pos + ball->y_speed < 0 || ball->y_pos + ball->y_speed >= DISPLAY_HEIGHT) {
        ball->y_speed *= -1;
    }

    // Check for bounce off Left paddle p1 iff it crosses border
    if (ball->x_pos + ball->size > p1->x_pos + p1->x_size &&
        ball->x_pos + ball->x_speed < p1->x_pos + p1->x_size) {
        float xBef = ball->x_pos; // left side of ball
        float yBef = ball->y_pos;
        float t = (p1->x_pos + p1->x_size - xBef) / (ball->x_speed);
        float yInt = yBef + t*ball->y_speed;  // intersection point
        // if overlap
        if ((yInt >= p1->y_pos && yInt < p1->y_pos + p1->y_size) || // upper corner
            (yInt + ball->size >= p1->y_pos && yInt + ball->size < p1->y_pos + p1->y_size)) { // lower corner
            // travel to intersection
            ball->x_pos += t*ball->x_speed;
            ball->y_pos += t*ball->y_speed;
            // get new speeds
            float modify = (ball->y_pos - (p1->y_pos + (p1->y_size - ball->size)/2.0)) / ((p1->y_size + ball->size)/2.0);
            ball_bounce(ball, &modify);
            // travel remaining distance
            ball->x_pos += (1-t)*ball->x_speed;
            ball->y_pos += (1-t)*ball->y_speed;
            return;
        }
    }

    // Check for bounce off Right paddle p2 iff it crosses border
    if (ball->x_pos < p2->x_pos &&
        ball->x_pos + ball->size + ball->x_speed > p2->x_pos) {
        float xBef = ball->x_pos + ball->size; // right side of ball
        float yBef = ball->y_pos;
        float t = (p2->x_pos - xBef) / (ball->x_speed);
        float yInt = yBef + t*ball->y_speed;  // intersection point
        // if overlap
        if ((yInt >= p2->y_pos && yInt < p2->y_pos + p2->y_size) || // upper corner
            (yInt + ball->size >= p2->y_pos && yInt + ball->size < p2->y_pos + p2->y_size)) { // lower corner
            // travel to intersection
            ball->x_pos += t*ball->x_speed;
            ball->y_pos += t*ball->y_speed;
            // get new speeds
            float modify = (ball->y_pos - (p2->y_pos + (p2->y_size - ball->size)/2.0)) / ((p2->y_size + ball->size)/2.0);
            ball_bounce(ball, &modify);
            // travel remaining distance
            ball->x_pos += (1-t)*ball->x_speed;
            ball->y_pos += (1-t)*ball->y_speed;
            return;
        }
    }

    ball->y_pos += ball->y_speed;
    ball->x_pos += ball->x_speed;
}

/**
 * @brief Draw score ingame for both players.
 * 
 * @param p1 Paddle struct. Player1
 * @param p2 Paddle struct. Player2
 */
void draw_score(Paddle *p1, Paddle * p2) {
    char b1[10], b2[10], b[20];
    itos(p1->score, b1);
    itos(p2->score, b2);

    int b_pos = 0, b12_pos = 0;
    while (b1[b12_pos]) b[b_pos++] = b1[b12_pos++]; // append p1 score
    b[b_pos++] = '-';
    b12_pos = 0;
    while (b2[b12_pos]) b[b_pos++] = b2[b12_pos++]; // append p2 score
    b[b_pos] = '\0';

    draw_string_grid(b, 0, CENTER);
}

/**
 * @brief When ball misses one of the paddles.
 * 
 * @param b Ball struct
 * @param p1 Paddle struct. Player1
 * @param p2 Paddle struct. Player2
 */
void ball_miss(Ball *b, Paddle *p1, Paddle *p2) {
    if (b->x_pos > 128 + 50) {
        p1->score++;
        b->x_pos = 128/2;
    } else if (b->x_pos < 0 - 50) {
        p2->score++;
        b->x_pos = 128/2;
    }
}

/**
 * @brief Draws the paddle.
 * 
 * @param paddle Paddle struct.
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

/**
 * @brief [PRIVATE] Used in move_paddle.
 * Used to know what direction player/paddle should move.
 * 
 */
typedef enum {UP, DOWN} move_dir;

/**
 * @brief Moves the paddle with restrictions
 * so paddle cannot go throught roof or floor.
 * 
 * @param p Paddle struct.
 * @param md move_dir enum value.
 */
void move_paddle(Paddle *p, move_dir md) {
    if (md == UP && p->y_pos > 0) {
        p->y_pos--;
    } else if (md == DOWN && p->y_pos + p->y_size < 31) {
        p->y_pos++;
    }
};

/**
 * @brief Custom sqrt function.
 * Source: https://ourcodeworld.com/articles/read/884/how-to-get-the-square-root-of-a-number-without-using-the-sqrt-function-in-c
 * 
 * @param number Number to sqrt.
 * @return float The sqrt of number.
 */
float my_sqrt(float number) {
    float sqrt = number / 2.0;
    float temp = 0;
    while(sqrt != temp){
        temp = sqrt;
        sqrt = ( number/temp + temp) / 2.0;
    }
    return sqrt;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Gives the ball new speed vectors on paddle bounce.
 * 
 * @param b Ball struct.
 * @param modify Modification factor [-1, 1].
 */
void ball_bounce(Ball *b, float *modify) {
    // max bounce angle +/- 60 degrees
    // tan(60deg) = sqrt(3)
    float ys = b->y_speed;
    float xs = b->x_speed;
    xs *= -1;

    // modify y-speed, modify = 0 -> no modification, modify = +/- 1 -> max modification
    float max_ys = my_sqrt(3)/2 * BALLSPEED;
    float diff = *modify > 0 ? max_ys - b->y_speed : -1*max_ys - b->y_speed;
    float absmod = *modify > 0 ? *modify : -1 * (*modify);
    b->y_speed += absmod*diff;

    if (b->y_speed > max_ys || b->y_speed < -1*max_ys) {
        b->y_speed = b->y_speed > 0 ? max_ys : -1*max_ys;
    }

    // corret x-speed
    b->x_speed = (xs > 0 ? 1 : -1) * my_sqrt(BALLSPEED*BALLSPEED - b->y_speed*b->y_speed);
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
        draw_clear();

        draw_string_grid("DIFFICULTY", 0, CENTER);
        Text_info easy = draw_string_grid("EASY", 15, LEFT);
        Text_info normal = draw_string_grid("NORMAL", 15, RIGHT);
        Text_info hard = draw_string_grid("HARD", 25, LEFT);
        Text_info impossible = draw_string_grid("IMPOSSIBLE", 25, RIGHT);

        Text_info options[] = {easy, normal, hard, impossible};
        draw_underline(&options[current_selection]);

        // up
        if (btn4_ispressed() && current_selection != EASY) {
            while (btn4_ispressed());
            current_selection--;
        }

        // down
        if (btn3_ispressed() && current_selection != IMPOSSIBLE) {
            while (btn3_ispressed());
            current_selection++;
        }

        // select
        if (btn1_ispressed()) {
            while (btn1_ispressed());
            return current_selection;
        }

        // back
        if (btn2_ispressed()) {
            while (btn2_ispressed());
            return -1;
        }

        draw_canvas();
        delay(10);
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
        if (difficulty == -1)
            return;
    }

    Ball ball = {
        .x_pos = 127/2,
        .y_pos = 31/2,
        .size = BALLSIZE,
        .x_speed = 1,
        .y_speed = 0
    };
    
    Paddle p1 = {
        .x_pos = 0 + PADDLEGAP,
        .y_pos = 32/2 - PADDLESIZE_Y/2,
        .x_size = PADDLESIZE_X,
        .y_size = PADDLESIZE_Y,
        .score = 0
    };
    
    Paddle p2 = {
        .x_pos = 128 - PADDLESIZE_X - PADDLEGAP,
        .y_pos = 32/2 - PADDLESIZE_Y/2,
        .x_size = PADDLESIZE_X,
        .y_size = PADDLESIZE_Y,
        .score = 0
    };

    if (mode == SINGLEPLAYER && difficulty == IMPOSSIBLE) {
        p2.y_pos = 0;
        p2.y_size = 32;
    }

    // loading screen
    draw_clear();
    draw_string_grid("GET READY...", 10, CENTER);
    draw_canvas();
    delay(1000);

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
            if (difficulty == HARD)
                p2.y_pos = ball.y_pos;
        }

        ball_update(&ball, &p1, &p2);
        ball_miss(&ball, &p1, &p2);

        draw_paddle(&p1);
        draw_paddle(&p2);
        draw_ball(&ball);
        draw_score(&p1, &p2);

        // for testing purposes.
        if (p1.score > 2) {
            break;
        }

        draw_canvas();
        delay(20);
    }

    draw_clear();
    draw_string_grid("GAME OVER!", 0, CENTER);
    draw_string_grid(p1.score > p2.score ? "PLAYER 1 WON!" : "PLAYER 2 WON!", 10, CENTER);
    draw_canvas();
    delay(2000);

    if (difficulty == NORMAL || difficulty == HARD) {
        int score = p1.score > p2.score ? p1.score : p2.score;
        input_name_screen(score, difficulty);
    }
    
}
