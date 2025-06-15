#include "types.h"
#include "print_char.h"

#define BUFFER_SIZE 64

extern uint8_t get_scancode(void);
extern char scancode_to_ascii(uint8_t sc);

char input_line[BUFFER_SIZE];
int input_pos = 0;

void print_string(const char* str) {
    while (*str) {
        print_char(*str++);
    }
}

void clear_input_buffer() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        input_line[i] = 0;
        input_pos = 0;
    }
}

void clear_input_line() {
    volatile char* video_memory = (volatile char*)0xb8000;

    for (int i = 0; i < 80; i++) {
        video_memory[i * 2] = ' ';       // space char
        video_memory[i * 2 + 1] = 0x07; // attribute (light grey on black)
    }
}

int str_equal(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return *a == 0 && *b == 0;
}

void shell_loop() {
    while (1) {
        uint8_t sc = get_scancode();
        char c = scancode_to_ascii(sc);

        if (!c) continue;

        if (c == '\b') {
            if (input_pos > 0) {
                input_pos--;
                print_char(c);
            }
        } else if (c == '\n') {
            print_char(c);
            input_line[input_pos] = 0; // null-terminate

            if (str_equal(input_line, "test")) {
                print_string("Command test recognized\n");
                clear_input_buffer();
            }

            if (str_equal(input_line, "clear")) {
                clear_input_line();
                clear_input_buffer();
                reset_print_pos();
            }

        } else {
            if (input_pos < BUFFER_SIZE - 1) {
                input_line[input_pos++] = c;
                print_char(c);
            }
        }
    }
}

