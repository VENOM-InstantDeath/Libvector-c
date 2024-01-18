#include <stdio.h>
#include <string.h>
#include "vector.h"
int main() {
	int x = 555;
	string S = string_init(NULL);
	string_append_fmt(&S, "Nombre: %s, Edad: %s. XD: %s", "Muhammad", "2", "efectivamente");
	puts(S.c_str);
}
