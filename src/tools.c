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