#include <stdio.h>
#include "burp.h"

struct MM57109 mm;

int main(int argc, char* argv) {
	mm57109_init(&mm);
	burp_init(&mm);
	puts("initialised");
}
