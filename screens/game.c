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
 * @brief Update ball position when hitting roof and floor.
 * 
 * @param ball Ball struct.
 */
void ball_update(Ball *ball) {
    if (ball->y_pos + ball->y_speed < 0 || ball->y_pos + ball->y_speed >= DISPLAY_HEIGHT) {
        ball->y_speed *= -1;
    }

    ball->y_pos += ball->y_speed;
    ball->x_pos += ball->x_speed;
}


/**
 * @brief Int to string.
 * 
 * @param num The integer number to convert.
 * @param buffer The output location.
 */
void itos(int num, char *buffer) {
    int pos = 0;

    // zeros won't display otherwise.
    if (num == 0) {
        buffer[pos++] = 48;
    } else {
        while (num != 0) {
            buffer[pos++] = num % 10 + 48;
            num /= 10;
        }
    }

    char from = 0, to = pos - 1;
    while (from < to) {
        char temp = buffer[from];
        buffer[from++] = buffer[to];
        buffer[to--] = temp;
    }
    buffer[pos] = '\0';
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
 * @brief Get the ball relative position to paddle
 * 
 * @param b The ball
 * @param p The paddle
 * @return float Value between 1.0 and -1.0 depending on hit
 */
float get_ball_paddle_relative_pos(Ball *b, Paddle *p) {
    float p_mid = (p->y_pos + ((float) p->y_size)) / 2.0;
    float b_mid = (b->y_pos + ((float) b->size)) / 2.0;

    float val = (p_mid - b_mid) / ((float) p->y_size / 2.0);
    return val / 10;
}

/**
 * @brief Custom sqrt function.
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
 * @brief Handles the ball bounce on paddles.
 * 
 * @param b Ball struct.
 * @param p Paddle struct.
 */
void ball_bounce(Ball *b, Paddle *p) {
    float ys = b->y_speed;
    float xs = b->x_speed;

    float hyp = my_sqrt(xs*xs + ys*ys);
    float rel_pos = get_ball_paddle_relative_pos(b, p);
    float new_ys = my_sqrt((hyp*hyp) - 1) + rel_pos;

    // ball goes down
    if (b->y_speed < 0) {
        new_ys *= -1;
    }

    b->y_speed = new_ys;
    b->x_speed *= -1;
}

/**
 * @brief A bad collision that checks when
 * ball collides with a paddle.
 * 
 * @param b Ball Struct.
 * @param p Paddle struct.
 */
void ball_collision(Ball *b, Paddle *p) {
    if (p->x_pos <= b->x_pos &&
        p->x_pos + p->x_size >= b->x_pos + b->size &&
        p->y_pos <= b->y_pos &&
        p->y_pos + p->y_size >= b->y_pos + b->size) {
        ball_bounce(b, p);
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

        draw_paddle(&p1);
        draw_paddle(&p2);

        ball_collision(&ball, &p1);
        ball_collision(&ball, &p2);

        ball_miss(&ball, &p1, &p2);

        ball_update(&ball);
        draw_ball(&ball);

        draw_score(&p1, &p2);

        if (p1.score > 4) {
            break;
        }

        draw_canvas();

        delay(20);
    }

    draw_clear();
    draw_string_grid("GAME OVER!", 0, CENTER);
    draw_string_grid("PLAYER 1 WON!", 10, CENTER);
    // draw_string_grid(p1.score > p2.score ? "PLAYER 1 WON!" : "PLAYER " WON!", 10, CENTER);
    draw_canvas();
    delay(1000);
    score_screen(p1.score);
}
