#ifndef defs_h
#define defs_h

#include "opcodes.h"

typedef void (*Handler)(void);

struct gameboy {
    unsigned char a;
    union {
        struct {
            unsigned char c;
            unsigned char b;
        };
        unsigned short bc;
    };
    union {
        struct {
            unsigned char e;
            unsigned char d;
        };
        unsigned short de;
    };
    union {
        struct {
            unsigned char l;
            unsigned char h;
        };
        unsigned short hl;
    };
    unsigned short pc;
    unsigned short sp;
    long cycleCount;
    struct {
        unsigned char Z;
        unsigned char N;
        unsigned char H;
        unsigned char C;
    }flag;
    unsigned char interruptsFlag;
    unsigned char vblankFlag;
};

Handler jump[0xff];
struct gameboy cpu;
unsigned char memory[0xffff+1];

int emuQuit;

#endif