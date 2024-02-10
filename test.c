#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

string* new_string(char* str) {
    string* x = malloc(sizeof(string));
    *x = string_init(str);
    return x;
}

int main() {
    vector vec = vector_init(VGEN);
    vector_append((&vec), new_string("Hello world"));
    puts(string_get_c_str(vector_get_at((&vec), -1)));
}
