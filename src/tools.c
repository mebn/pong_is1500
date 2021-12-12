#include "../include/eeprom.h"
#include "../include/graphics.h"

/**
 * Written by: Marcus Nilszén
 * 
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
 * Written by: Marcus Nilszén
 * Source: https://ourcodeworld.com/articles/read/884/how-to-get-the-square-root-of-a-number-without-using-the-sqrt-function-in-c
 * 
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
        sqrt = (number/temp + temp) / 2.0;
    }
    return sqrt;
}

/**
 * Written by: Alex Gunnarsson
 * Source: https://github.com/alevarn/pic32-pong-game/blob/2eb1203e1593d5eb2d4c56830e10e86cdea170c1/tools/utility.c
 * 
 * @brief Seed used for pseudo-random function.
 * 
 */
static unsigned int seed = 326246914;

/**
 * Written by: Alex Gunnarsson
 * 
 * @brief Produces a pseudo-random unsigned integer.
 * 
 * @return unsigned int The pseudo-random result.
 */
unsigned int random() {
    seed = seed * 1103515245 + 26475;
    return seed;
}

/**
 * Written by: Alex Gunnarsson
 * Source: https://github.com/alevarn/pic32-pong-game/blob/2eb1203e1593d5eb2d4c56830e10e86cdea170c1/tools/utility.c
 * 
 * @brief 
 * 
 * @return unsigned int 
 */
unsigned int random_binary() {
    seed = seed * 1103515245 + 12345;
    return ((seed / 65536) % 2);
}

/**
 * Written by: Alex Gunnarsson
 * Source: https://github.com/alevarn/pic32-pong-game/blob/2eb1203e1593d5eb2d4c56830e10e86cdea170c1/tools/utility.c
 * 
 * @brief Produces a pseudo-random unsigned integer below a specified max.
 * 
 * @param max The maximum value for the return value (exclusive): [0, max)
 * @return unsigned int The pseudo-random result.
 */
unsigned int random_max(unsigned int max) {
    return random() % max;
}

/**
 * Written by: Alex Gunnarsson
 * Source: https://github.com/alevarn/pic32-pong-game/blob/2eb1203e1593d5eb2d4c56830e10e86cdea170c1/main.c
 * 
 * @brief Randomizes the seed before using it to ensure new results.
 * 
 */
void init_seed(void) {
    seed = eeprom_read_seed();

    // shuffle
    unsigned int i;
    for (i = 0; i < 1000000; i++) {
        random();
    }

    eeprom_write_seed(seed);
}

/**
 * Written by: Alex Gunnarsson & Marcus Nilszén
 * 
 * @brief Calculates the length (width) in pixels of a string.
 * 
 * @param s The string.
 * @return int Amount of pixels in width.
 */
char str_len(char *s) {
    char len = 0;
    while (*s) {
        len += font_width[*s-0x20] + FONT_SPACING;
        s++;
    }
    return len - FONT_SPACING;
}