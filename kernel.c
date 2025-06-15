typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64

uint8_t get_scancode() {
    while (!(inb(KBD_STATUS_PORT) & 1))
        ;
    return inb(KBD_DATA_PORT);
}

char scancode_to_ascii(uint8_t sc) {
    static const char map[0x58] = {
        0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
        0,   'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
        '\\','z','x','c','v','b','n','m',',','.','/', 0,  '*', 0,
        ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if (sc < sizeof(map))
        return map[sc];
    return 0;
}

void print_char(char c) {
    static int pos = 0;
    volatile char* video_memory = (volatile char*)0xb8000;

    if (c == '\b') {
        if (pos > 0) {
            pos--;
            video_memory[pos * 2] = ' ';       // erase character
            video_memory[pos * 2 + 1] = 0x07;  // attribute
        }
    } else {
        video_memory[pos * 2] = c;
        video_memory[pos * 2 + 1] = 0x07;
        pos++;
    }
}

void main(){
    while (1) {
        uint8_t sc = get_scancode();
        char c = scancode_to_ascii(sc);
        if (c) {
            print_char(c);
        }
    }
    return;
}
