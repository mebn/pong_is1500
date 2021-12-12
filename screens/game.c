#include <stdbool.h>
#include "../include/timer.h"
#include "../include/screens.h"
#include "../include/buttons.h"
#include "../include/display.h"
#include "../include/tools.h"
#include "../include/eeprom.h"
#include "../include/graphics.h"

/**
 * Written by: Alex Gunnarsson & Marcus Nilszén
 * 
 * @brief Ball struct containing position, size, and speed.
 * 
 */
typedef struct {
    float x_pos;
    float y_pos;
    char size;
    float x_speed;
    float y_speed;
} Ball;

/**
 * Written by: Alex Gunnarsson & Marcus Nilszén
 * 
 * @brief Paddle struct containing position, size, and speed.
 * 
 */
typedef struct {
    int x_pos;
    int y_pos;
    char x_size;
    char y_size;
    char score;
} Paddle;


/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Responsible for freezing the ball for an amount of updates.
 * 
 */
static int updateTimer;
static bool freeze;

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Used for minimizing amount of calculations
 * for the predicition used in IMPOSSIBLE.
 * 
 */
static int endPos;
static bool calculated;

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Keeps track of the in-game time. 5 units is 1 second.
 * 
 */
static short timer;

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Decrements the in-game time.
 * 
 */
void decrement_timer() {
    timer--;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Draws the in-game time.
 * 
 */
void draw_timer() {
    unsigned char time = timer / 5;
    char string[6];
    char digit[2];
    
    itos((time / 60) / 10, digit);
    string[0] = digit[0];

    itos((time / 60) % 10, digit);
    string[1] = digit[0];
    
    string[2] = ':';
    
    itos((time % 60) / 10, digit);
    string[3] = digit[0];

    itos((time % 60) % 10, digit);
    string[4] = digit[0];
    
    string[5] = '\0';

    draw_string_grid(string, DISPLAY_HEIGHT - FONT_SIZE, CENTER);
}

/**
 * Written by: Marcus Nilszén & Alex Gunnarsson
 * 
 * @brief Draw the ball used in game.
 * 
 * @param ball Ball struct.
 */
void draw_ball(Ball *ball) {
    if (freeze && updateTimer >= FREEZETIME) return;
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
 * @brief Gives the ball new speed vectors on paddle bounce.
 * 
 * @param b Ball struct.
 * @param modify Modification factor [-1, 1].
 */
void ball_bounce(Ball *b, float *modify) {
    // char buffer[10];
    // itos((int) 100*(*modify), buffer);
    // draw_string(buffer, 10, 10);
    // draw_canvas();
    // delay(1000);

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

    // correct x-speed
    b->x_speed = (xs > 0 ? 1 : -1) * my_sqrt(BALLSPEED*BALLSPEED - b->y_speed*b->y_speed);
    // new direction, prediciton calculation invalid
    calculated = false;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Spawns the ball in the middle and begins freezetime.
 * 
 * @param b The ball struct.
 */
void ball_spawn(Ball *b) {
    b->x_pos = DISPLAY_WIDTH/2 - b->size/2;
    b->y_pos = DISPLAY_HEIGHT/2 - b->size/2;
    b->x_speed = (random_binary() == 1 ? 1 : -1);
    b->y_speed = 0;
    float y = (float) random_max(2000001) / 1000000 - 1;    // range [-1, 1]
    ball_bounce(b, &y);
    freeze = true;
    updateTimer = FREEZETIME + PASSTIME;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief When ball misses one of the paddles.
 * 
 * @param b Ball struct
 * @param p1 Paddle struct. Player1
 * @param p2 Paddle struct. Player2
 */
void ball_miss(Ball *b, Paddle *p1, Paddle *p2) {
    if (b->x_pos > DISPLAY_WIDTH) {
        p1->score++;
        ball_spawn(b);
    } else if (b->x_pos < 0 - b->size) {
        p2->score++;
        ball_spawn(b);
    }
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Increment ball position, keeping inbetween roof and floor. 
 * 
 * @param ball The ball struct.
 */
void ball_incr(Ball *ball) {
    // Bounce off roof and floor
    if (ball->y_pos + ball->y_speed < 0 || ball->y_pos + ball->size + ball->y_speed >= DISPLAY_HEIGHT) {
        ball->y_speed *= -1;
    }

    ball->y_pos += ball->y_speed;
    ball->x_pos += ball->x_speed;

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
    if (freeze) {
        updateTimer--;
        if (updateTimer < 0) freeze = false;
        return;
    }
  
    // Check for bounce off Left paddle p1 iff it crosses border
    if (ball->x_pos + TOLERANCE > p1->x_pos + p1->x_size &&
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
    if (ball->x_pos + ball->size - TOLERANCE < p2->x_pos &&
        ball->x_pos + ball->x_speed > p2->x_pos) {
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

    ball_incr(ball);
    ball_miss(ball, p1, p2);
}

/**
 * Written by: Marcus Nilszén
 * 
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
 * Written by: Alex Gunnarsson
 * 
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
 * Written by: Alex Gunnarsson
 * 
 * @brief [PRIVATE] Used in move_paddle.
 * Used to know what direction player/paddle should move.
 * 
 */
typedef enum {UP, DOWN} move_dir;

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Moves the paddle with specified speed and restrictions
 * so paddle cannot go through roof or floor.
 * 
 * @param p Paddle struct.
 * @param md move_dir enum value {UP, DOWN}.
 * @param speed Speed value to move by.
 */
void move_paddle_speed(Paddle *p, move_dir md, int speed) {
    if (md == UP && p->y_pos > 0) {
        p->y_pos -= speed;
    } else if (md == DOWN && p->y_pos + p->y_size < DISPLAY_HEIGHT) {
        p->y_pos += speed;
    }
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Moves the paddle with restrictions
 * so paddle cannot go through roof or floor.
 * 
 * @param p Paddle struct.
 * @param md move_dir enum value {UP, DOWN}.
 */
void move_paddle(Paddle *p, move_dir md) {
    move_paddle_speed(p, md, PADDLESPEED);
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Move the AI's paddle based on the ball's position
 * and a delay value.
 * 
 * @param p2 The AI's paddle struct.
 * @param b The ball struct.
 * @param delay The number of game updates until a reaction.
 */
void move_ai_incr(Paddle *p2, Ball *b, int delay) {
    // not moving towards, ignore
    if (b->x_speed < 0) return;
    // just recently bounced, ignore
    if (b->x_pos - delay*b->x_speed < PADDLEGAP + PADDLESIZE_X) return;

    float past_pos = b->y_pos - delay*b->y_speed;
    float ball_mid = past_pos + b->size/2;
    float paddle_mid = p2->y_pos + p2->y_size/2;
    if (ball_mid - paddle_mid < (-1) * PADDLESPEED || past_pos < 0) {
        move_paddle_speed(p2, UP, PADDLESPEED);
    } else if (ball_mid - paddle_mid > PADDLESPEED || past_pos >= DISPLAY_HEIGHT) {
        move_paddle_speed(p2, DOWN, PADDLESPEED);
    }
}

/**
 * Written by: Alex Gunnarsson
 * Source: https://stackoverflow.com/a/14997413
 * 
 * @brief Performs modulo operation but ensures output is positive.
 * 
 * @param num The dividend.
 * @param mod The divisor.
 * @return int The positive remainder.
 */
int positive_modulo(int num, int mod) {
    return (num % mod + mod) % mod;
}

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Control the AI's paddle with respect to difficulty.
 * 
 * @param p2 The AI's paddle struct.
 * @param b The ball struct.
 * @param difficulty The difficulty of the AI.
 */
void move_ai(Paddle *p2, Ball *b, game_difficulty difficulty) {
    switch (difficulty) {
        // follow ball's y-position, 20 game updates delay
        case EASY:
            move_ai_incr(p2, b, 20);
            break;

        // follow ball's y-position, 10 game updates delay
        case NORMAL:
            move_ai_incr(p2, b, 10);
            break;

        // simulate where the ball is going to end up and move there
        case HARD:
        case IMPOSSIBLE:
            if ((b->x_speed > 0) && ((difficulty == HARD && b->x_pos > DISPLAY_WIDTH/2) || difficulty == IMPOSSIBLE)) {
                if (!calculated) {
                    float yp = b->y_pos;
                    float xp = b->x_pos;
                    float ys = b->y_speed;
                    float xs = b->x_speed;

                    while (b->x_pos < DISPLAY_WIDTH - PADDLEGAP - PADDLESIZE_X - b->size) {
                        ball_incr(b);
                    }

                    endPos = (int) b->y_pos;
                    if (difficulty == HARD) {   // introduce random offset for chance to miss
                        endPos += (random_binary() == 1 ? 1 : -1) * random_max(PADDLESIZE_Y / 3);
                    }

                    b->y_pos = yp;
                    b->x_pos = xp;
                    b->y_speed = ys;
                    b->x_speed = xs;

                    // old calculation method which for some reason does not work... :C
                    // float t = (DISPLAY_WIDTH - PADDLESIZE_X - PADDLEGAP - b->size - b->x_pos) / b->x_speed;  // game updates (time) until ball crosses paddle border
                    // // global variables
                    // endPos = positive_modulo((int) (b->y_pos + t*b->y_speed), DISPLAY_HEIGHT - b->size);   // unfold (mirror) display, act as if ball wouldnt bounce off roof/floor, positive modulo
                    // int numBounces = (int) (b->y_pos + t*b->y_speed) / (DISPLAY_HEIGHT - b->size);  
                    // char* buffer[2];
                    // itos(numBounces, buffer);
                    // draw_string(buffer, 10, 10);
                    // draw_canvas();
                    // delay(1000);
                    // if (positive_modulo(numBounces, 2) == 1) endPos = DISPLAY_HEIGHT - endPos;  // invert if odd amount of edge bounces
                    calculated = true;
                } 

                float distance = (endPos - p2->y_size/2 + b->size/2) - p2->y_pos;
                if (distance > BALLSPEED || distance < (-1) * BALLSPEED) {
                    move_paddle(p2, (distance > 0 ? DOWN : UP));
                }
            } else {
                char middle = DISPLAY_HEIGHT/2 - p2->y_size/2;
                if (p2->y_pos != middle) {
                    if (p2->y_pos > middle) {
                        move_paddle(p2, UP);
                    } else {
                        move_paddle(p2, DOWN);
                    }
                }
            }
            break;
    }
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
        .size = BALLSIZE
    };
    ball_spawn(&ball);
    
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

    // potentially different spawn conditions for singleplayer
    if (mode == SINGLEPLAYER) {
        if (difficulty == EASY) {
            p1.y_pos = 0;
            p1.y_size = 32;
        }
    }

    // loading screen
    draw_clear();
    draw_string_grid("GET READY...", 10, CENTER);
    draw_canvas();
    
    init_seed();
    timer3_init();

    // init global vars
    calculated = false;         // used for reducing amount of calculations for HARD difficulty, not yet calculated
    freeze = true;              // used to freeze the ball on respawn, inactive
    updateTimer = FREEZETIME;   // different freezetime for first spawn
    timer = 5 * GAMETIME - 1;   // amount of seconds for each game

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
            move_ai(&p2, &ball, difficulty);
        }

        ball_update(&ball, &p1, &p2);

        if (timer <= -1) break;     // termination condition
        draw_timer();
        draw_paddle(&p1);
        draw_paddle(&p2);
        draw_ball(&ball);
        draw_score(&p1, &p2);

        draw_canvas();
        delay(20);
    }

    timer3_uninit();

    draw_clear();
    draw_string_grid("GAME OVER!", 0, CENTER);
    if (p1.score == p2.score) {
        draw_string_grid("TIE!", 10, CENTER);
    } else {
        if (mode == SINGLEPLAYER) {
            draw_string_grid(p1.score > p2.score ? "YOU WON!" : "THE AI WON!", 10, CENTER);
        } else {
            draw_string_grid(p1.score > p2.score ? "PLAYER 1 WON!" : "PLAYER 2 WON!", 10, CENTER);
        }
    } 
    draw_canvas();
    delay(2000);

    if (mode == SINGLEPLAYER) {
        char lowest_score = eeprom_read(score_addrs[difficulty][3]);
        if (p1.score > lowest_score)
            input_name_screen(p1.score, difficulty);
    }
    
}
