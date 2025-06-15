#include "print_char.h"

static int pos = 0;

void print_char(char c) {
    volatile char* video_memory = (volatile char*)0xb8000;
    if (c == '\b') {
        if (pos > 0) {
            pos--;
            video_memory[pos * 2] = ' ';
            video_memory[pos * 2 + 1] = 0x07;
        }
    } else {
        video_memory[pos * 2] = c;
        video_memory[pos * 2 + 1] = 0x07;
        pos++;
    }
}

void reset_print_pos(void) {
    pos = 0;
}
