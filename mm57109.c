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
	}
}

