#include "mm57109.h"

void mm57109_init(struct MM57109* mm) {
	mm->state = halt;
	mm57109_set_register(&mm->x, 0);
	mm57109_set_register(&mm->y, 0);
	mm57109_set_register(&mm->t, 0);
	mm57109_set_register(&mm->m, 0);
}

void mm57109_set_register(struct MM57109_register* reg, float value) {
	reg->value = value;
}

void mm57109_op(struct MM57109* mm, int op) {
	switch(op) {
	case OP_0: case OP_1: case OP_2: case OP_3: case OP_4: case OP_5: case OP_6: case OP_7: case OP_8: case OP_9:
		switch(mm->state) {
		case integer_entry:
			mm57109_set_register(&mm->x, mm57109_get_register(&mm->x) * 10 + op);
		break;
		case decimal_entry:
			//TODO track divisor and add to X
		break;
		default:
			mm57109_set_register(&mm->x, op);
			mm->state = integer_entry;
		}
	break;
	}
}

