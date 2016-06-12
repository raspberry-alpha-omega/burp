#include <stdio.h>
#include "burp.h"

struct MM57109 mm;

int main(int argc, char* argv) {
	uint8_t mm_ram[256];
	mm57109_init(&mm, mm_ram);
	burp_init(&mm, mm_ram);
}
