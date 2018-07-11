#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

void emuExit() {
    printf("PC:%x\nSP:%x\nOP:%x\nCY:%ld\n", 
        cpu.pc, cpu.sp, memory[cpu.pc], cpu.cycleCount);
    printf("A:%02x - \n", cpu.a);
    printf("B:%02x - C:%02x (BC:%04x)\n", cpu.b, cpu.c, cpu.bc);
    printf("D:%02x - E:%02x (DE:%04x)\n", cpu.d, cpu.e, cpu.de);
    printf("H:%02x - L:%02x (HL:%04x)\n", cpu.h, cpu.l, cpu.hl);
    emuQuit = 1;
}

unsigned short get(int size) {
    if(size == 1) {
        return memory[cpu.pc+1];
    } else {
        return (memory[cpu.pc+2]<<8)|memory[cpu.pc+1];
    }
}

void nope() {
    printf("0x%02x", memory[cpu.pc]);
    printf("---not yet implemented\n\n");
    emuExit();
}

///////////////////////////////////////////////////////////////////////////////////////////
//0x
void op00() {
    printf("0x%02x - NOP\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.pc += 1;
}

void op01() {
    printf("0x%02x - LD BC,d16 - 0x%02x\n", memory[cpu.pc], get(2));
    cpu.cycleCount += 12;
    cpu.bc = get(2);
    cpu.pc += 3;
}

void op02() {
    printf("0x%02x - LD (BC),A\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    memory[cpu.bc] = cpu.a;
    cpu.pc += 1;
}

void op03() {
    printf("0x%02x - INC BC\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.bc++;
    cpu.pc += 1;
}

void op04() {
    printf("0x%02x - INC B\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.b++;
    cpu.pc += 1;
    //Z 0 H
    if(cpu.b == 0) {
        cpu.flag.Z = 1;
    }
    if((cpu.b-1&0x0f) == 0) {
        cpu.flag.H = 1;
    }
    cpu.flag.N = 0;
}

void op05() {
    printf("0x%02x - DEC B - ", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.b--;
    cpu.pc += 1;
    printf("0x%02x\n", cpu.b);
    if(cpu.b == 0) {
        cpu.flag.Z = 1;
    }
}

void op06() {
    printf("0x%02x - LD B,d8\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.b = get(1);
    cpu.pc += 2;
}

void op07() {
    printf("0x%02x - RLCA\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.flag.C = cpu.a >> 7;
    cpu.a <<= cpu.a;
    cpu.pc += 1;
}

void op08() {
    printf("0x%02x - LD (a16),SP\n", memory[cpu.pc]);
    cpu.cycleCount += 20;
    cpu.sp = get(2);
    cpu.pc += 3;
}

void op09() {
    printf("0x%02x - ADD HL,BC\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.hl += cpu.bc;
    cpu.pc += 1;
}

void op0a() {
    printf("0x%02x - LD A,(BC)\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.a = memory[cpu.bc];
    cpu.pc += 1;
}

void op0b() {
    printf("0x%02x - DEC BC\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.bc--;
    cpu.pc += 1;
}

void op0c() {
    printf("0x%02x - INC C\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.c++;
    cpu.pc += 1;
}

void op0d() {
    printf("0x%02x - DEC C\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.c--;
    cpu.pc += 1;
}

void op0e() {
    printf("0x%02x - LD C,d8 - 0x%02x\n", memory[cpu.pc], get(1));
    cpu.cycleCount += 8;
    cpu.c = get(1);
    cpu.pc += 2;
}

void op0f() {
    printf("0x%02x - RRCA\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.flag.C = cpu.a & 0x1;
    cpu.a >>= cpu.a;
}
///////////////////////////////////////////////////////////////////////////////////////////
//1x
void op10() {
    printf("0x%02x - STOP\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    //figure out
    cpu.pc += 2;
}

void op11() {
    printf("0x%02x - LD DE,d16\n", memory[cpu.pc]);
    cpu.cycleCount += 12;
    cpu.de = get(2);
    cpu.pc += 3;
}

void op12() {
    printf("0x%02x - LD (DE),A\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    memory[cpu.de] = cpu.a;
    cpu.pc += 1;
}

void op13() {
    printf("0x%02x - INC DE\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.de++;
    cpu.pc += 1;
}

void op14() {
    printf("0x%02x - INC D\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.d++;
    cpu.pc += 1;
}

void op15() {
    printf("0x%02x - DEC D\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.d--;
    cpu.pc += 1;
}

void op16() {
    printf("0x%02x - LD D,d8\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.d = get(1);
    cpu.pc += 2;
}

void op17() {
    printf("0x%02x - RLA\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    unsigned char newCarry = cpu.a >> 7;
    unsigned char oldCarry = cpu.flag.Z;
    cpu.flag.Z = newCarry;
    cpu.a = (cpu.a << 1) | oldCarry;
}

void op1e() {
    printf("0x%02x - LD E,d8\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.e = get(1);
    cpu.pc += 2;
}

///////////////////////////////////////////////////////////////////////////////////////////
//2x
void op20() {
    printf("0x%02x - JR NZ,r8 - +%d\n", memory[cpu.pc], get(1));
    cpu.cycleCount += 8;
    if(cpu.flag.Z == 0) {
        char x = get(1);
        cpu.pc = cpu.pc + x + 2;
    } else {
        cpu.pc += 2;
    }
}

void op21() {
    printf("0x%02x - LD HL,d16 - 0x%02x\n", memory[cpu.pc], get(2));
    cpu.cycleCount += 12;
    cpu.hl = get(2);
    cpu.pc += 3;
}

void op22() {
    printf("0x%02x - LD (HL+),A\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.a = memory[cpu.hl];
    cpu.hl++;
    cpu.pc += 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
//3x
void op31() {
    printf("0x%02x - LD SP,d16\n", memory[cpu.pc]);
    cpu.cycleCount += 12;
    cpu.sp = get(2);
    cpu.pc += 3;
}

void op32() {
    printf("0x%02x - LD (HL-),A - ", memory[cpu.pc]);
    cpu.cycleCount += 8;
    cpu.a = memory[cpu.hl];
    cpu.hl--;
    cpu.pc += 1;
    printf("0x%02x - 0x%02x\n", cpu.a, cpu.hl);
}

void op36() {
    printf("0x%02x - LD (HL),d8\n", memory[cpu.pc]);
    cpu.cycleCount += 12;
    cpu.hl = get(1);
    cpu.pc += 2;
}

void op3e() {
    printf("0x%02x - LD A,d8 - 0x%02x\n", memory[cpu.pc], get(1));
    cpu.cycleCount += 8;
    cpu.a = get(1);
    cpu.pc += 2;
}

///////////////////////////////////////////////////////////////////////////////////////////
//4x

//5x

//6x

//7x

//8x

//9x

//Ax
void opaf() {
    printf("0x%02x - XOR A\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.a ^= cpu.a;
    cpu.pc++;
}

//Bx

//Cx
void opc3() {
    printf("0x%02x - JP a16 - 0x%02x\n", memory[cpu.pc], get(2));
    cpu.cycleCount += 16;
    cpu.pc = get(2);
}

//Dx

//Ex
void ope0() {
    printf("0x%02x - LDH (a8),A - 0x%02x\n", memory[cpu.pc], 0xff00 + get(1));
    cpu.cycleCount += 12;
    memory[0xff00 + get(1)] = cpu.a;
    cpu.pc += 2;
}

void opea() {
    printf("0x%02x - LD (a16),A - 0x%02x\n", memory[cpu.pc], get(2));
    cpu.cycleCount += 16;
    memory[get(2)] = cpu.a;
    cpu.pc += 3;
}

//Fx
void opf0() {
    printf("0x%02x - LDH A,(a8) - 0x%02x\n", memory[cpu.pc], 0xff00 + get(1));
    cpu.cycleCount += 12;
    cpu.a = memory[0xff00 + get(1)];
    cpu.pc += 2;
}

void opf3() {
    printf("0x%02x - DI\n", memory[cpu.pc]);
    cpu.cycleCount += 4;
    cpu.interruptsFlag = 0;
    cpu.pc += 1;
}

void opfe() {
    printf("0x%02x - CP d8\n", memory[cpu.pc]);
    cpu.cycleCount += 8;
    unsigned char s = get(1);
    if(cpu.a == s) {
        cpu.flag.Z = 1;
    }
    cpu.flag.N = 1;
    if(cpu.a < s) {
        cpu.flag.C = 1;
    }
    cpu.pc += 2;
}