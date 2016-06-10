#ifndef MM57109_H
#define MM57109_H

#include <stdint.h>

/* use native types for now, implement wacky MM57109 BCD later */
struct MM57109_register {
	double value;
};

enum MM57109_state { normal, integer_entry, decimal_entry, exponent_entry, invert };

struct MM57109 {
	struct MM57109_register x;
	struct MM57109_register y;
	struct MM57109_register z;
	struct MM57109_register t;
	struct MM57109_register m;
	enum MM57109_state state;
	float dp;
	union {
		struct {
			unsigned int f1 : 1;
			unsigned int f2 : 1;
			unsigned int rw1 : 1;
			unsigned int rw2 : 1;
			unsigned int mode : 1;
			unsigned int err : 1;
		} flags;
		uint8_t flagbyte;
	};
};


#define mm57109_set_flag(flag) { flag = 1; }
#define mm57109_clear_flag(flag) { flag = 0; }

// generic: NOP, HALT, MCLR, ECLR
// digit entry: 0,1,2,3,4,5,6,7,8,9,DP,EE,CS,PI
// stack: ROLL,EN,POP,XEY,XEM,MS,MR,MSH,LSH
// math(X,Y): +,-,*,/
// math(X,M): inv+, inv-, inv*,inv/
// math(X): 1/x,sqrt,sq,10x,ex,ln,log
// trig(X): sin,cos,tan,invsin.invcos,invtan,DTR,RTD
// flow: JMP,TJC,TERR,TX0,TXF,TXLT0,IBNZ,DBNZ
// io: IN,OUT,AIN
// flags: SF1,PF1,SF2,PF2,PRW1,PRW2
// mode: TOGM,SMDC,INV

/* opcode table from datasheet:
      00      01      10      11
0000  0       TJC     INV     XEY
0001  1       TX0     EN      EX
0010  2       TXLT0   TOGM    10X
0011  3       TXF     ROLL    SQ
0100  4       TERR    SIN (-) SQRT
0101  5       JMP     COS (-) LN
0110  6       OUT     TAN (-) LOG
0111  7       IN      SF1     1/X
1000  8       SMDC    PF1     YX
1001  9       IBNZ    SF2     + (M+)
1010  DP      DBNZ    PF2     - (M-)
1011  EE      XEM     ECLR    * (M*)
1100  CS      MS      RTD     / (M/)
1101  PI      MR      DTR     PRW1
1110  AIN     LSH     POP     PRW2
1111  HALT    RSH     MCLR    NOP
*/

#define OP_0     0b000000
#define OP_1     0b000001
#define OP_2     0b000010
#define OP_3     0b000011
#define OP_4     0b000100
#define OP_5     0b000101
#define OP_6     0b000110
#define OP_7     0b000111
#define OP_8     0b001000
#define OP_9     0b001001
#define OP_DP    0b001010
#define OP_EE    0b001011
#define OP_CS    0b001100
#define OP_PI    0b001101
#define OP_AIN   0b001110
#define OP_HALT  0b001111

#define OP_TJC   0b010000
#define OP_TX0   0b010001
#define OP_TXLT0 0b010010
#define OP_TXF   0b010011
#define OP_TERR  0b010100
#define OP_JMP   0b010101
#define OP_OUT   0b010110
#define OP_IN    0b010111
#define OP_SMDC  0b011000
#define OP_IBNZ  0b011001
#define OP_DBNZ  0b011010
#define OP_XEM   0b011011
#define OP_MS    0b011100
#define OP_MR    0b011101
#define OP_LSH   0b011110
#define OP_RSH   0b011111

#define OP_INV   0b100000
#define OP_EN    0b100001
#define OP_TOGM  0b100010
#define OP_ROLL  0b100011
#define OP_SIN   0b100100
#define OP_COS   0b100101
#define OP_TAN   0b100110
#define OP_SF1   0b100111
#define OP_PF1   0b101000
#define OP_SF2   0b101001
#define OP_PF2   0b101010
#define OP_ECLR  0b101011
#define OP_RTD   0b101100
#define OP_DTR   0b101101
#define OP_POP   0b101110
#define OP_MCLR  0b101111

#define OP_XEY   0b110000
#define OP_EX    0b110001
#define OP_10X   0b110010
#define OP_SQ    0b110011
#define OP_SQRT  0b110100
#define OP_LN    0b110101
#define OP_LOG   0b110110
#define OP_1_X   0b110111
#define OP_YX    0b111000
#define OP_PLUS  0b111001
#define OP_MINUS 0b111010
#define OP_MUL   0b111011
#define OP_DIV   0b111100
#define OP_PRW1  0b111101
#define OP_PRW2  0b111110
#define OP_NOP   0b111111

void mm57109_init(struct MM57109* mm);
void mm57109_set_register(struct MM57109_register* reg, float value);
float mm57109_get_register(struct MM57109_register* reg);

void mm57109_op(struct MM57109* mm, uint8_t op);

#endif
