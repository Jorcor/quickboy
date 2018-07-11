all:
	gcc main.c opcodes.c -F/Library/Frameworks -framework SDL2
	./a.out tetris.gb