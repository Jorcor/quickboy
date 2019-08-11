#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

int main() {
	struct gboy cpu;

	FILE* fp;
	fp = fopen("/home/sif/Dropbox/quickboy/cpu_instrs.gb", "rb");
	fread(cpu.ram, 1, 0xffff, fp);

	initCPU(&cpu);
	while(1) {
		status(&cpu);
		jump[cpu.ram[cpu.pc]](&cpu);
		// if(cpu.cycle % 15 == 0) {
		// 	getchar();
		// }
	}
	
	return 0;
}

