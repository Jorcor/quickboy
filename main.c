#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "defs.h"
#include "opcodes.h"

void emuInit() {
    for(int i = 0; i < 0xff; i++) {
        jump[i] = nope;
    }
    cpu.pc = 0x100;
    cpu.sp = 0xFFFE;
    cpu.cycleCount = 0;
    cpu.interruptsFlag = 1;
    cpu.vblankFlag = 0;
    emuQuit = 0;

    //jumpTable
    ////0X
    jump[0x00] = op00;
    jump[0x01] = op01;
    jump[0x02] = op02;
    jump[0x03] = op03;
    jump[0x04] = op04;
    jump[0x05] = op05;
    jump[0x06] = op06;
    jump[0x07] = op07;
    jump[0x08] = op08;
    jump[0x09] = op09;
    jump[0x0a] = op0a;
    jump[0x0b] = op0b;
    jump[0x0c] = op0c;
    jump[0x0d] = op0d;
    jump[0x0e] = op0e;
    jump[0x0f] = op0f;
    ////1X
    jump[0x10] = op10;
    jump[0x11] = op11;
    jump[0x12] = op12;
    jump[0x13] = op13;
    jump[0x14] = op14;
    jump[0x15] = op15;
    jump[0x16] = op16;
    jump[0x1e] = op1e;
    ////2X
    jump[0x20] = op20;
    jump[0x21] = op21;
    ////3X
    jump[0x31] = op31;
    jump[0x32] = op32;
    jump[0x36] = op36;
    jump[0x3e] = op3e;
    ////4X
    ////5X
    ////6X
    ////7X
    ////8X
    ////9X
    ////Ax
    jump[0xaf] = opaf;
    ////Bx
    ////Cx
    jump[0xc3] = opc3;
    ////Dx
    ////Ex
    jump[0xe0] = ope0;
    jump[0xea] = opea;
    ////Fx
    jump[0xf0] = opf0;
    jump[0xf3] = opf3;
    jump[0xfe] = opfe;
}

void *timer() {
    printf("timer started");

    while(1) {
        usleep(16);
        cpu.vblankFlag = 1;
        if(emuQuit == 1) {
            pthread_exit(NULL);
        }
    }
}

int main(int argc, char** argv) {
    printf("%d, %s\n", argc, argv[1]);

    //Load rom into ram
    FILE *fileptr;
    long filelen;
    fileptr = fopen(argv[1], "rb");
    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);
    fread(memory, filelen, 1, fileptr);
    fclose(fileptr);

    SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	} else {
		window = SDL_CreateWindow( "SDL Tutorial", 100, 100, 256, 256, SDL_WINDOW_SHOWN );
		if( window == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		} else {
			screenSurface = SDL_GetWindowSurface( window );
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
			SDL_UpdateWindowSurface( window );
			SDL_Delay( 2000 );
		}
	}


    emuInit();

    pthread_t timerTID;
    pthread_create(&timerTID, NULL, timer, NULL);

    //emu Loop
    while (1) {
        if(cpu.interruptsFlag == 1) {
            if(cpu.vblankFlag == 1) {
                printf("-- vblank --\n");
                cpu.vblankFlag = 0;
            }
        }
        jump[memory[cpu.pc]]();
        if(emuQuit == 1) {
            break;
        }
    }

    pthread_join(timerTID, NULL);

	SDL_DestroyWindow( window );
	SDL_Quit();

    return 0;
}