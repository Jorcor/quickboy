#ifndef cpu_header
#define cpu_header

//structs//
typedef struct gboy {
	union {
		unsigned short af;
		struct {
			unsigned char a;
			unsigned char f;
		};
	};
	union {
		unsigned short bc;
		struct {
			unsigned char b;
			unsigned char c;
		};
	};
	union {
		unsigned short de;
		struct {
			unsigned char d;
			unsigned char e;
		};
	};
	union {
		unsigned short hl;
		struct {
			unsigned char h;
			unsigned char l;
		};
	};
	unsigned short sp;
	union {
		unsigned short pc;
		struct {
			unsigned char pch;
			unsigned char pcl;
		};
	};
	union {
		struct {
			unsigned fz : 1;
			unsigned fn : 1;
			unsigned fh : 1;
			unsigned fc : 1;
			unsigned fr : 4;
		};
		unsigned char ff;
	};
	unsigned char ram[0xffff + 1];

	int cycle;
	int interrupts;
} gboy;
/////////////

//functions//
void initCPU(struct gboy* cpu);
void status(struct gboy* cpu);
void emuExit(struct gboy* cpu);
void setJumps();
unsigned char getd8(struct gboy* cpu);
unsigned short getd16(struct gboy* cpu);
char getr8(struct gboy* cpu);
void cycle(struct gboy* cpu, int num);
/////////////

//ops//
//0x
void op00(struct gboy* cpu);
void op01(struct gboy* cpu);
void op02(struct gboy* cpu);
void op03(struct gboy* cpu);
void op04(struct gboy* cpu);
void op05(struct gboy* cpu);
void op06(struct gboy* cpu);
void op07(struct gboy* cpu);
void op08(struct gboy* cpu);
void op09(struct gboy* cpu);
void op0A(struct gboy* cpu);
void op0B(struct gboy* cpu);
void op0C(struct gboy* cpu);
void op0D(struct gboy* cpu);
void op0E(struct gboy* cpu);
void op0F(struct gboy* cpu);
//1X
void op18(struct gboy* cpu);
//2x
void op20(struct gboy* cpu);
void op21(struct gboy* cpu);
void op23(struct gboy* cpu);
void op28(struct gboy* cpu);
void op2A(struct gboy* cpu);
//3x
void op30(struct gboy* cpu);
void op31(struct gboy* cpu);
void op32(struct gboy* cpu);
void op3C(struct gboy* cpu);
void op3E(struct gboy* cpu);
//4x
void op4F(struct gboy* cpu);
//7x
void op77(struct gboy* cpu);
void op78(struct gboy* cpu);
void op7C(struct gboy* cpu);
void op7D(struct gboy* cpu);
//Ax
void opAF(struct gboy* cpu);
//Bx
void opB1(struct gboy* cpu);
void opB8(struct gboy* cpu);
void opB9(struct gboy* cpu);
//Cx
void opC0(struct gboy* cpu);
void opC1(struct gboy* cpu);
void opC2(struct gboy* cpu);
void opC3(struct gboy* cpu);
void opC5(struct gboy* cpu);
void opC9(struct gboy* cpu);
void opCD(struct gboy* cpu);
//Ex
void opE0(struct gboy* cpu);
void opE1(struct gboy* cpu);
void opE5(struct gboy* cpu);
void opEA(struct gboy* cpu);
//Fx
void opF0(struct gboy* cpu);
void opF1(struct gboy* cpu);
void opF3(struct gboy* cpu);
void opF5(struct gboy* cpu);
void opFE(struct gboy* cpu);
///////

typedef void (*Handler)(struct gboy* cpu);
Handler jump[0xff];

#endif

