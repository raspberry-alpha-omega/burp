#include <math.h>

#include "mm57109.h"

void mm57109_init(struct MM57109* mm) {
	mm->state = normal;
	mm57109_set_register(&mm->x, 0);
	mm57109_set_register(&mm->y, 0);
	mm57109_set_register(&mm->t, 0);
	mm57109_set_register(&mm->m, 0);
}

void mm57109_set_register(struct MM57109_register* reg, float value) {
	reg->value = value;
}

float mm57109_get_register(struct MM57109_register* reg) {
	return reg->value;
}

void mm57109_op(struct MM57109* mm, uint8_t op) {
	switch(op) {

	case OP_0: case OP_1: case OP_2: case OP_3: case OP_4: case OP_5: case OP_6: case OP_7: case OP_8: case OP_9:
		switch(mm->state) {
		case integer_entry:
			mm57109_set_register(&mm->x, mm57109_get_register(&mm->x) * 10 + op);
		break;
		case decimal_entry:
			//TODO track divisor and add to X
		break;
		case exponent_entry:
			//TODO track exponent and scale value
		break;
		default:
			mm57109_set_register(&mm->x, op);
			mm->state = integer_entry;
		}
	break;
	case OP_DP:
		if (mm->state != integer_entry) {
			mm57109_set_register(&mm->x, 0);
		}
		mm->state = decimal_entry;
	break;
	case OP_EE:
		if (mm->state != integer_entry && mm->state != decimal_entry) {
			mm57109_set_register(&mm->x, 0);
		}
		mm->state = exponent_entry;
	break;
	case OP_CS:
		mm57109_set_register(&mm->x, -mm57109_get_register(&mm->x));
	break;
	case OP_PI:
		mm57109_set_register(&mm->x, M_PI);
	break;
	case OP_AIN:
		//TODO not quite sure what this is about
	break;
	case OP_HALT:
		//TODO AFAICT this doesn't actually change any states or values
	break;

	case OP_TJC:
		//TODO
	break;
	case OP_TX0:
		//TODO
	break;
	case OP_TXLT0:
		//TODO
	break;
	case OP_TXF:
		//TODO
	break;
	case OP_TERR:
		//TODO
	break;
	case OP_JMP:
		//TODO
	break;
	case OP_OUT:
		//TODO
	break;
	case OP_IN:
		//TODO
	break;
	case OP_SMDC:
		//TODO
	break;
	case OP_IBNZ:
		//TODO
	break;
	case OP_DBNZ:
		//TODO
	break;
	case OP_XEM:
		//TODO
	break;
	case OP_MS:
		//TODO
	break;
	case OP_MR:
		//TODO
	break;
	case OP_LSH:
		//TODO
	break;
	case OP_RSH:
		//TODO
	break;

	case OP_INV:
		//TODO
	break;
	case OP_EM:
		//TODO
	break;
	case OP_TOGM:
		//TODO
	break;
	case OP_ROLL:
		//TODO
	break;
	case OP_SIN:
		//TODO
	break;
	case OP_COS:
		//TODO
	break;
	case OP_TAN:
		//TODO
	break;
	case OP_SF1:
		//TODO
	break;
	case OP_PF1:
		//TODO
	break;
	case OP_SF2:
		//TODO
	break;
	case OP_PF2:
		//TODO
	break;
	case OP_ECLR:
		//TODO
	break;
	case OP_RTD:
		//TODO
	break;
	case OP_DTR:
		//TODO
	break;
	case OP_POP:
		//TODO
	break;
	case OP_MCLR:
		//TODO
	break;

	case OP_XEY:
		//TODO
	break;
	case OP_EX:
		//TODO
	break;
	case OP_10X:
		//TODO
	break;
	case OP_SQ:
		//TODO
	break;
	case OP_SQRT:
		//TODO
	break;
	case OP_LN:
		//TODO
	break;
	case OP_LOG:
		//TODO
	break;
	case OP_1_X:
		//TODO
	break;
	case OP_YX:
		//TODO
	break;
	case OP_PLUS:
		//TODO
	break;
	case OP_MINUS:
		//TODO
	break;
	case OP_MUL:
		//TODO
	break;
	case OP_DIV:
		//TODO
	break;
	case OP_PRW1:
		//TODO
	break;
	case OP_PRW2:
		//TODO
	break;
	case OP_NOP:
		//TODO
	break;

	}
}

