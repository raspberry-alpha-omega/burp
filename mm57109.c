#include <math.h>
#include <stdio.h> //TODO remove after testing

#include "mm57109.h"

void mm57109_init(struct MM57109* mm, uint8_t* ram) {
	mm->ram = ram;
	mm->state = normal;
	mm57109_set_register(&mm->x, 0);
	mm57109_set_register(&mm->y, 0);
	mm57109_set_register(&mm->t, 0);
	mm57109_set_register(&mm->m, 0);
	mm->dp = 1;
	mm->pc = 0;
}

void mm57109_set_register(struct MM57109_register* reg, float value) {
	reg->value = value;
}

float mm57109_get_register(struct MM57109_register* reg) {
	return reg->value;
}

void set_pc(struct MM57109* mm, uint8_t addr) {
	mm->pc = addr;
}

void mm57109_push(struct MM57109* mm, float value) {
	mm->t = mm->z;
	mm->z = mm->y;
	mm->y = mm->x;
	mm57109_set_register(&mm->x, value);
}

float mm57109_pop(struct MM57109* mm) {
	float ret = mm57109_get_register(&mm->x);
	mm->x = mm->y;
	mm->y = mm->z;
	mm->z = mm->t;
	mm57109_set_register(&mm->t, 0);
	return ret;
}

int is_two(uint8_t op) {
	switch (op) {
	case OP_SMDC: case OP_IN: case OP_OUT:
	case OP_JMP: case OP_TJC: case OP_TERR: case OP_TX0: case OP_TXF: case OP_TXLT0: case OP_IBNZ: case OP_DBNZ:
		return 1;
	default: return 0;
	}
}

void mm57109_op(struct MM57109* mm, uint8_t op) {
	uint8_t arg = op;
	if (mm->state == two) {
		op = mm->prev;
	} else if (is_two(op)) {
		mm->prev = op;
		mm->state = two;
		return;
	}

	switch(op) {

	case OP_0: case OP_1: case OP_2: case OP_3: case OP_4: case OP_5: case OP_6: case OP_7: case OP_8: case OP_9:
		switch(mm->state) {
		case integer_entry:
			mm57109_set_register(&mm->x, (mm57109_get_register(&mm->x) * 10) + op);
		break;
		case decimal_entry:
			mm->dp /= 10;
			mm57109_set_register(&mm->x, mm57109_get_register(&mm->x) + (op * mm->dp));
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
		mm->state = normal;
	break;
	case OP_PI:
		mm57109_set_register(&mm->x, M_PI);
		mm->state = normal;
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
		mm->pc = arg;
		mm->state = normal;
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
	case OP_XEM: {
		struct MM57109_register tmp;
		tmp = mm->x;
		mm->x = mm->m;
		mm->m = tmp;
		mm->state = normal;
	} break;
	case OP_MS:
		mm->m = mm->x;
		mm->state = normal;
	break;
	case OP_MR:
		mm57109_push(mm, mm57109_get_register(&mm->m));
		mm->state = normal;
	break;
	case OP_LSH:
		//TODO
	break;
	case OP_RSH:
		//TODO
	break;

	case OP_INV:
		mm->state = (mm->state == invert) ? normal : invert;
	break;
	case OP_EN:
		mm57109_push(mm, 0);
		mm->state = normal;
	break;
	case OP_TOGM:
		//TODO
	break;
	case OP_ROLL: {
		struct MM57109_register tmp;
		tmp = mm->t;
		mm->t = mm->x;
		mm->x = mm->y;
		mm->y = mm->z;
		mm->z = tmp;
		mm->state = normal;
	} break;
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
		mm->x = mm->y;
		mm->y = mm->z;
		mm->z = mm->t;
		mm57109_set_register(&mm->t, 0);
		mm->state = normal;
	break;
	case OP_MCLR:
		mm57109_set_register(&mm->x, 0);
		mm57109_set_register(&mm->y, 0);
		mm57109_set_register(&mm->z, 0);
		mm57109_set_register(&mm->t, 0);
		mm57109_set_register(&mm->m, 0);
		mm->flagbyte = 0;
		mm->state = normal;
	break;

	case OP_XEY:{
		struct MM57109_register tmp;
		tmp = mm->x;
		mm->x = mm->y;
		mm->y = tmp;
		mm->state = normal;
	} break;
	case OP_EX: {
		float x = mm57109_pop(mm);
		mm57109_push(mm, exp(x));
	} break;
	case OP_10X:{
		float x = mm57109_pop(mm);
		mm57109_push(mm, pow(10,x));
	} break;
	case OP_SQ: {
		float x = mm57109_pop(mm);
		mm57109_push(mm, x * x);
	} break;
	case OP_SQRT: {
		float x = mm57109_pop(mm);
		mm57109_push(mm, sqrt(x));
	} break;
	case OP_LN: {
		float x = mm57109_pop(mm);
		mm57109_push(mm, log(x));
	} break;
	case OP_LOG: {
		float x = mm57109_pop(mm);
		mm57109_push(mm, log10(x));
	} break;
	case OP_1_X: {
		float x = mm57109_pop(mm);
		mm57109_push(mm, 1/x);
	} break;
	case OP_YX: {
		float x = mm57109_pop(mm);
		float y = mm57109_pop(mm);
		mm57109_push(mm, pow(y,x));
	} break;
	case OP_PLUS:
		if (mm->state == invert) {
			float x = mm57109_pop(mm);
			float m = mm57109_get_register(&mm->m);
			mm57109_set_register(&mm->m, m + x);
		} else {
			float x = mm57109_pop(mm);
			float y = mm57109_pop(mm);
			mm57109_push(mm, x + y);
		}
		mm->state = normal;
	break;
	case OP_MINUS:
		if (mm->state == invert) {
			float x = mm57109_pop(mm);
			float m = mm57109_get_register(&mm->m);
			mm57109_set_register(&mm->m, m - x);
		} else {
			float x = mm57109_pop(mm);
			float y = mm57109_pop(mm);
			mm57109_push(mm, x - y);
		}
		mm->state = normal;
	break;
	case OP_MUL:
		if (mm->state == invert) {
			float x = mm57109_pop(mm);
			float m = mm57109_get_register(&mm->m);
			mm57109_set_register(&mm->m, m * x);
		} else {
			float x = mm57109_pop(mm);
			float y = mm57109_pop(mm);
			mm57109_push(mm, x * y);
		}
		mm->state = normal;
	break;
	case OP_DIV:
		if (mm->state == invert) {
			float x = mm57109_pop(mm);
			float m = mm57109_get_register(&mm->m);
			mm57109_set_register(&mm->m, m / x);
		} else {
			float x = mm57109_pop(mm);
			float y = mm57109_pop(mm);
			mm57109_push(mm, x / y);
		}
		mm->state = normal;
	break;
	case OP_PRW1:
		//TODO
	break;
	case OP_PRW2:
		//TODO
	break;
	case OP_NOP:
		// it's a NOP, so do nothing!
	break;

	}
}

