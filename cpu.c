#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

//basically missing all the z,n,h,c checks 
//flag checks are next on my list

void setJumps() {
	for (int i = 0; i < 0xff; ++i) {
		jump[i] = emuExit;
	}
	//0x0X
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
	jump[0x0A] = op0A;
	jump[0x0B] = op0B;
	jump[0x0C] = op0C;
	jump[0x0D] = op0D;
	jump[0x0E] = op0E;
	jump[0x0F] = op0F;

	//0x1X
	jump[0x18] = op18;

	//0x2X
	jump[0x20] = op20;
	jump[0x21] = op21;
	jump[0x23] = op23;
	jump[0x28] = op28;
	jump[0x2A] = op2A;

	//0x3X
	jump[0x30] = op30;
	jump[0x31] = op31;
	jump[0x32] = op32;
	jump[0x3C] = opC3;
	jump[0x3E] = op3E;

	//0x4X
	jump[0x4F] = op4F;

	//0x7X
	jump[0x77] = op77;
	jump[0x78] = op78;
	jump[0x7c] = op7C;
	jump[0x7D] = op7D;

	//0xAX
	jump[0xAF] = opAF;

	//0xBX
	jump[0xB1] = opB1;
	jump[0xB8] = opB8;
	jump[0xB9] = opB9;

	//0xCX
	jump[0xC0] = opC0;
	jump[0xC1] = opC1;
	jump[0xC2] = opC2;
	jump[0xC3] = opC3;
	jump[0xC5] = opC5;
	jump[0xC9] = opC9;
	jump[0xCD] = opCD;

	//0xEX
	jump[0xE0] = opE0;
	jump[0xE1] = opE1;
	jump[0xE5] = opE5;
	jump[0xEA] = opEA;

	//0xFX
	jump[0xF0] = opF0;
	jump[0xF1] = opF1;
	jump[0xF3] = opF3;
	jump[0xF5] = opF5;
	jump[0xFE] = opFE;
}

void initCPU(struct gboy* cpu) {
	cpu->a = 0;
	cpu->b = 0;
	cpu->c = 0;
	cpu->d = 0;
	cpu->e = 0;
	cpu->f = 0;
	cpu->h = 0;
	cpu->l = 0;
	cpu->pc = 0x100;
	cpu->sp = 0xFFFE;
	cpu->cycle = 0;
	cpu->ff = 0;
	setJumps();
}
void status(struct gboy* cpu) {
	printf("PC:%x\nSP:%x\nOP:%x\nCY:%d\n", cpu->pc, cpu->sp, cpu->ram[cpu->pc], cpu->cycle);
	printf("A:%02x - F:%02x (AF:%04x)\n", cpu->a, cpu->f, cpu->af);
	printf("B:%02x - C:%02x (BC:%04x)\n", cpu->b, cpu->c, cpu->bc);
	printf("D:%02x - E:%02x (DE:%04x)\n", cpu->d, cpu->e, cpu->de);
	printf("H:%02x - L:%02x (HL:%04x)\n", cpu->h, cpu->l, cpu->hl);
}
void emuExit(struct gboy* cpu) {
	printf("\n%02x : Unknown op\n", cpu->ram[cpu->pc]);
	exit(3);
}
unsigned char getd8(struct gboy* cpu) {
	unsigned char d8;
	d8 = cpu->ram[cpu->pc + 1];
	return d8;
}
unsigned short getd16(struct gboy* cpu) {
	unsigned short d16;
	d16 = (cpu->ram[cpu->pc + 2] << 8) | cpu->ram[cpu->pc + 1];
	return d16;
}
char getr8(struct gboy* cpu) {
	short r8;
	r8 = cpu->ram[cpu->pc + 1];
	return r8;
}
void cycle(struct gboy* cpu, int num) {
	for (int i = 0; i < num; i++) {
		//add per-cycle interrupt checking
		cpu->cycle++;
	}
}
unsigned short popd16(struct gboy* cpu) {
	unsigned short d16;
	d16 = (cpu->ram[cpu->sp + 1] << 8) | cpu->ram[cpu->sp];
	return d16;
}
void pushd16(struct gboy* cpu, unsigned char high, unsigned char low) {
	cpu->ram[cpu->sp-1] = high;
	cpu->ram[cpu->sp-2] = low;
}

//0x 
void op00(struct gboy* cpu) {
	printf("%02x : 00 : NOP\n", cpu->pc);
	cpu->pc += 1;
	cycle(cpu, 4);
}
void op01(struct gboy* cpu) {
	printf("%02x : 01 : LD BC,d16(%x)\n", cpu->pc, cpu->bc);
	cpu->bc = getd16(cpu);
	cpu->pc += 3;
	cycle(cpu, 12);
}
void op02(struct gboy* cpu) {
	printf("%02x : 02 : LD (BC),A(%x)\n", cpu->pc, cpu->a);
	cpu->bc = cpu->a;
	cpu->pc += 1;
	cycle(cpu, 8);
}
void op03(struct gboy* cpu) {
	printf("%02x : 03 : INC BC(%x)\n", cpu->pc, cpu->bc);
	cpu->bc++;
	cpu->pc += 1;
	cycle(cpu, 8);
}
void op04(struct gboy* cpu) {
	printf("%02x : 04 : INC B(%x)\n", cpu->pc, cpu->b+1);
	cpu->b++;
	//Z 0 H -
	if (cpu->b == 0) {
		cpu->fz = 1;
	}
	cpu->fn = 0;
	//dont know how to set H flag
	cpu->pc += 1;
	cycle(cpu, 4);
	cpu->cycle += 4;
}
void op05(struct gboy* cpu) {
	printf("%02x : 05 : DEC B(%x)\n", cpu->pc, cpu->b-1);
	cpu->b--;
	cpu->pc += 1;
	if (cpu->b == 0) {
		cpu->fz = 1;
	}
	else {
		cpu->fz = 0;
	}
	cpu->fn = 1;
	//dont know how to set H flag
	cycle(cpu, 4);
}
void op06(struct gboy* cpu) {
	printf("%02x : 06 : LD B,d8(%x)\n", cpu->pc, getd8(cpu));
	cpu->b = getd8(cpu);
	cpu->pc += 2;
	cycle(cpu, 8);
}
void op07(struct gboy* cpu) { //--needs work still--
	printf("%02x : 07 : RLCA\n", cpu->pc);
	cpu->fc = cpu->a >> 7;
	cpu->a = cpu->a << 1;
	cpu->pc += 1;
	cycle(cpu, 4);
}
void op08(struct gboy* cpu) {
	printf("%02x : 08 : LD (a16)(%x),SP(%x)\n", cpu->pc, getd16(cpu), cpu->sp);
	cpu->ram[getd16(cpu)] = cpu->sp;
	cpu->pc += 3;
	cpu->cycle += 20;
}
void op09(struct gboy* cpu) {
	printf("%02x : 09 : ADD HL(%x),BC(%x)\n", cpu->pc, cpu->hl, cpu->bc);
	cpu->hl += cpu->bc;
	cpu->fn = 0;
	//dont know how to set H flag
	//dont know hot to set C flag
	cpu->pc += 1;
	cpu->cycle += 8;
}
void op0A(struct gboy* cpu) {
	printf("%02x : 0A : LD A,(BC)(%x)\n", cpu->pc, cpu->ram[cpu->bc]);
	cpu->a = cpu->ram[cpu->bc];
	cpu->pc += 1;
	cpu->cycle += 8;
}
void op0B(struct gboy* cpu) {
	printf("%02x : 0B : DEC BC(%x)\n", cpu->pc, cpu->bc - 1);
	cpu->bc--;
	cpu->pc += 1;
	cpu->cycle += 8;
}
void op0C(struct gboy* cpu) {
	printf("%02x : 0C : INC C(%x)\n", cpu->pc, cpu->c + 1);
	cpu->c++;
	//Z 0 H -
	if (cpu->b == 0) {
		cpu->fz = 1;
	}
	else {
		cpu->fz = 0;
	}
	cpu->fn = 0;
	//Dont know how to set H flag
}
void op0D(struct gboy* cpu) {
	printf("%02x : 0D : DEC C(%x)\n", cpu->pc, cpu->c-1);
	cpu->c -= 1;
	if (cpu->c == 0) {
		cpu->fz = 1;
	}
	else {
		cpu->fz = 0;
	}
	cpu->pc += 1;
	cpu->fn = 1;
	//dont know how to set H flag
	cpu->cycle += 4;
}
void op0E(struct gboy* cpu) {
	printf("%02x : 0E : LD C,d8(%x)\n", cpu->pc, getd8(cpu));
	cpu->c = getd8(cpu);
	cpu->pc += 2;
	cpu->cycle += 8;
}
void op0F(struct gboy* cpu) {
	printf("%02x : 0F : RRCA(%x)\n", cpu->pc, cpu->a);
	cpu->a = (cpu->a >> 1) | (cpu->a << 7);
	cpu->pc += 1;
	//dont know how to set C flag
	cpu->cycle += 4;
}
//1x
void op18(struct gboy* cpu) {
	printf("%02x : 18 : JR r8(%x)\n", cpu->pc, getr8(cpu));
	cpu->pc += getr8(cpu);
	cpu->pc += 2;
	cpu->cycle += 12;
}
//2x
void op20(struct gboy* cpu) {
	if (cpu->fz == 0) {
		printf("%02x : 20 : JR NZ,r8(%d)\n", cpu->pc, getr8(cpu));
		cpu->pc += getr8(cpu);
		cpu->cycle += 12;
	}
	else {
		printf("%02x : 20 : JR NZ,r8(no)\n", cpu->pc+2);
		cpu->pc += 2;
		cpu->cycle += 8;
	}
}
void op21(struct gboy* cpu) {
	printf("%02x : 21 : LD HL,d16(%x)\n", cpu->pc, getd16(cpu));
	cpu->hl = getd16(cpu);
	cpu->pc += 3;
	cpu->cycle += 12;
}
void op23(struct gboy* cpu) {
	printf("%02x : 23 : LD (HL+)(%x), A(%x)\n", cpu->pc, cpu->hl, cpu->a);
	cpu->ram[cpu->hl] = cpu->a;
	cpu->hl += 1;
	cpu->pc += 1;
	cpu->cycle += 8;
}
void op28(struct gboy* cpu) {
	printf("%02x : 28 : JR Z, r8(%x)\n", cpu->pc, getr8(cpu));
	if(cpu->fz == 1) {
		cpu->pc += (unsigned char)getr8(cpu);
		cycle(cpu, 12);
	} else {
		cpu->pc += 2;
		cycle(cpu, 8);
	}
}
void op2A(struct gboy* cpu) {
	printf("%02x : 2A : LD A(%x), (HL+)(%x)\n", cpu->pc, cpu->a, cpu->hl);
	cpu->a = cpu->ram[cpu->hl];
	cpu->hl += 1;
	cpu->pc += 1;
	cpu->cycle += 8;
}
//3x
void op30(struct gboy* cpu) {
	printf("%02x : 30 : JR NC,r8(%x)\n", cpu->pc, getr8(cpu));
	if (cpu->fc == 0) {
		cpu->pc += getr8(cpu);
		cpu->cycle += 12;
	}
	else {
		cpu->pc += 2;
		cpu->cycle += 8;
	}
}
void op31(struct gboy* cpu) {
	printf("%02x : 31 : LD SP,d16(%x)\n", cpu->pc, getd16(cpu));
	cpu->sp = getd16(cpu);
	cpu->pc += 3;
	cpu->cycle += 12;
}
void op32(struct gboy* cpu) {
	printf("%02x : 32 : LD $(HL-)(%x),A(%x)\n", cpu->pc, cpu->hl, cpu->a);
	cpu->ram[cpu->hl] = cpu->a;
	cpu->hl -= 1;
	cpu->pc += 1;
	cpu->cycle += 8;
}
void op3C(struct gboy* cpu) {
	printf("%02x : 3C : INC A(%x)\n", cpu->pc, cpu->a+1);
	cpu->a += 1;
	cpu->pc += 1;
	cycle(cpu, 4);
}
void op3E(struct gboy* cpu) {
	printf("%02x : 3E : LD A,d8 : %x\n", cpu->pc, getd8(cpu));
	cpu->a = getd8(cpu);
	cpu->pc += 2;
	cycle(cpu, 8);
}
//4x
void op4F(struct gboy* cpu) {
	printf("%02x : 4F : LD C,A(%x)(%x)\n", cpu->pc, cpu->c, cpu->a);
	cpu->c = cpu->a;
	//no flags
	cpu->pc += 1;
	cycle(cpu, 4);
}
//7x
void op77(struct gboy* cpu) {
	printf("%02x : 77 : LD (HL),A(%x)(%x)\n", cpu->pc, cpu->hl, cpu->a);
	cpu->ram[cpu->hl] = cpu->a;
	cpu->pc += 1;
	cycle(cpu, 8);
}
void op78(struct gboy* cpu) {
	printf("%02x : 78 : LD A, B(%x)\n", cpu->pc, cpu->b);
	cpu->a = cpu->b;
	cpu->pc += 1;
	cycle(cpu, 4);
}
void op7C(struct gboy* cpu) {
	printf("%02x : 7C : LD A,H(%x)\n", cpu->pc, cpu->h);
	cpu->a = cpu->h;
	cpu->pc += 1;
	cpu->cycle += 4;
}
void op7D(struct gboy* cpu) {
	printf("%02x : 7D : LD A,L(%x)\n", cpu->pc, cpu->l);
	cpu->a = cpu->l;
	cpu->pc += 1;
	cpu->cycle += 4;
}
//Ax
void opAF(struct gboy* cpu) {
	printf("AF : XOR A : %x\n", cpu->pc);
	
	cpu->a ^= cpu->a;

	cpu->pc += 1;
	if (cpu->a == 0) {
		cpu->fz = 1;
	}
	else {
		cpu->fz = 0;
	}
	cpu->cycle += 4;
}
//Bx
void opB1(struct gboy* cpu) {
	printf("%02x : B1 : OR C(%x)(%x)\n", cpu->pc, cpu->c, cpu->a);
	cpu->a = cpu->c | cpu->a;
	cpu->pc += 1;
	cycle(cpu, 4);
}
void opB8(struct gboy* cpu) {
	printf("%02x : B8 : CP B(%x)(%x)\n", cpu->pc, cpu->b, cpu->a);
	unsigned short out = cpu->a - cpu->b;
	//zero flag
	cpu->fz = (out==0?1:0);
	//subtract flag
	cpu->fn = 1;
	//check carry
	cpu->fc = (cpu->c>cpu->b?1:0);
	//check half-carry(dont know how yet)
	cpu->pc += 1;
	cycle(cpu, 4);
}
void opB9(struct gboy* cpu) {
	printf("%02x : B9 : CP C(%x)(%x)\n", cpu->pc, cpu->c, cpu->a);
	unsigned short out = cpu->a - cpu->c;
	//zero flag
	cpu->fz = (out==0?1:0);
	//subtract flag
	cpu->fn = 1;
	//check carry
	cpu->fc = (cpu->c>cpu->a?1:0);
	//check half-carry(dont know how yet)
	cpu->pc += 1;
	cycle(cpu, 4);
}
//Cx
void opC0(struct gboy* cpu) {
	printf("%02x : C0 : RET NZ(%x)\n", cpu->pc, cpu->fz);
	if(cpu->fz == 0) {
		//return, 			pcl (SP), pch (SP+1), SPSP+2
		cpu->pcl = cpu->sp;
		cpu->pch = cpu->sp+1;
		cpu->sp += 2;
		cycle(cpu, 20);
	} else {
		cpu->pc += 1;
		cycle(cpu, 8);
	}
}
void opC1(struct gboy* cpu) {
	printf("%02x : C1 : POP BC(%x)\n", cpu->pc, popd16(cpu));
	cpu->bc = popd16(cpu);
	cpu->pc += 1;
	cycle(cpu, 12);
}
void opC2(struct gboy* cpu) {
	printf("%02x : C2 : JP NZ,a16(%x)\n", cpu->pc, getd16(cpu));
	if (cpu->fz == 0) {
		cpu->pc = getd16(cpu);
		cpu->cycle += 16;
	}
	else {
		cpu->pc += 3;
		cpu->cycle += 12;
	}
}
void opC3(struct gboy* cpu) {
	printf("%02x : C3 : JP a16(%x)\n", cpu->pc, getd16(cpu));
	cpu->pc = getd16(cpu);
	cpu->cycle += 16;
}
void opC5(struct gboy* cpu) {
	printf("%02x : C5 : PUSH BC(%x)\n", cpu->pc, cpu->bc);
	pushd16(cpu, cpu->b, cpu->c);
	cpu->sp -= 2;
	cpu->pc += 1;
	cpu->cycle += 16;
}
void opC9(struct gboy* cpu) {
	printf("%02x : C9 : RET Z", cpu->pc);
	unsigned char pch, pcl;
	pcl = cpu->ram[cpu->sp];
	pch = cpu->ram[cpu->sp + 1];
	cpu->pc = (unsigned short)(pch << 8) | (unsigned short)pcl;
	printf("(%04x)\n", cpu->pc);
}
void opCD(struct gboy* cpu) {
	printf("%02x : CD : CALL a16(%x)", cpu->pc, getd16(cpu));
	unsigned short pc = cpu->pc + 3;
	unsigned char pch, pcl;
	pch = (pc >> 8);
	pcl = pc;
	printf(" pc(%02x%02x)\n", pch, pcl);
	cpu->ram[cpu->sp - 1] = pch;
	cpu->ram[cpu->sp - 2] = pcl;
	cpu->sp -= 2;
	cpu->pc = getd16(cpu);
	cpu->cycle += 24;
}
//Ex
void opE0(struct gboy* cpu) {
	printf("%02x : E0 : LDH (a8)(%x),A\n", cpu->pc, 0xff00 + getd8(cpu));
	cpu->ram[0xff00 + getd8(cpu)] = cpu->a;
	cpu->pc += 2;
	cpu->cycle += 12;
}
void opE1(struct gboy* cpu) {
	printf("%02x : E1 : POP HL (%x)\n", cpu->pc, popd16(cpu));
	cpu->hl = popd16(cpu);
	cpu->sp += 2;
	cpu->pc += 1;
	cpu->cycle += 12;
}
void opE5(struct gboy* cpu) {
	printf("%02x : E5 : PUSH HL(%x)\n", cpu->pc, cpu->hl);
	cpu->ram[cpu->sp - 1] = cpu->h;
	cpu->ram[cpu->sp - 2] = cpu->l;
	cpu->sp -= 2;
	cpu->pc += 1;
	cpu->cycle += 16;
}
void opEA(struct gboy* cpu) {
	printf("%02x : EA : LD (a16)(%x),A(%x)\n", cpu->pc, getd16(cpu), cpu->a);
	cpu->ram[getd16(cpu)] = cpu->a;
	cpu->pc += 3;
	cpu->cycle += 16;
}
//Fx
void opF0(struct gboy* cpu) {
	printf("%02x : F0 : LDH A, (a8)(%x)\n", cpu->pc, 0xff00 + getd8(cpu));
	cpu->a = cpu->ram[0xff00 + getd8(cpu)];
	cpu->pc += 2;
	cycle(cpu, 12);
}
void opF1(struct gboy* cpu) {
	printf("%02x : F1 : POP AF(%x)\n", cpu->pc, popd16(cpu));
	cpu->af = popd16(cpu);
	cpu->sp += 2;
	cpu->pc += 1;
	cycle(cpu, 16);
}
void opF3(struct gboy* cpu) {
	printf("%02x : F3 : DI\n", cpu->pc);
	cpu->interrupts = 0;
	cpu->pc += 1;
	cycle(cpu, 4);
}
void opF5(struct gboy* cpu) {
	printf("%02x : F5 : PUSH AF (%x)\n", cpu->pc, cpu->af);
	pushd16(cpu, cpu->a, cpu->f);
	cpu->sp -= 2;
	cpu->pc += 1;
	cycle(cpu, 16);
}
void opFE(struct gboy* cpu) {
	printf("%02x : FE : CP d8(%x - %x)\n", cpu->pc, cpu->a, getd8(cpu));
	if (cpu->a - getd8(cpu)) {
		cpu->fz = 1;
	}
	else {
		cpu->fz = 0;
	}
	cpu->fn = 1;
	//dont know how to set H
	if (cpu->a < getd8(cpu)) {
		cpu->fc = 1;
	}
	else {
		cpu->fc = 0;
	}
	cpu->pc += 2;
	cycle(cpu, 8);
}

