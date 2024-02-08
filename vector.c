#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "vector.h"

char* _error_messages[] = {
	"NULL_STR: Se esperaba un c-string pero se obtuvo un puntero nulo",
	"NULL_OBJ: Se esperaba un objeto string pero se obtuvo un puntero nulo",
	"OUT_OF_BOUNDS: La posición solicitada está fuera de los límites del string",
};

void* _realloc(void* ptr, size_t size) {
	return !(ptr) ? malloc(size) : realloc(ptr, size);
}

void* _realloc2(void* ptr, size_t size) {
	return !(ptr) ? calloc(size, 1) : realloc(ptr, size);
}

void string_realloc(string* S, size_t size) {
	S->c_str = _realloc2(S->c_str, size);
	S->memsize = size;
	S->c_str[size-1] = 0;
}

void string_grow(string* S, size_t size) {
	S->c_str = _realloc2(S->c_str, S->memsize+size);
	//char* ptr = S->c_str+S->memsize;
	S->memsize += size;
	//memset(ptr, 0, size);
}

void string_shrink(string* S, size_t size) {
	S->c_str = _realloc2(S->c_str, S->memsize-size);
	S->memsize -= size;
}

string string_init(char* str) {
	size_t str_size = str ? strlen(str) : 0;
	char* _str = str_size ? strdup(str) : NULL;
	if (_str) _str = realloc(_str, str_size);
	string S = {_str, str_size ? str_size : 0};
	return S;
}

void string_raise(char* function, string_exception exception) {
	printf("libvector: error in %s: %s\n", function, _error_messages[exception]);
	exit(1);
}

char string_get_at(string* S, int n) {
	if (n < 0) {return S->c_str[S->memsize+n];}
	if (n >= S->memsize) string_raise("string_get_at", OUT_OF_BOUNDS);
	return S->c_str[n];
}

void string_asign_at(string* S, int n, char ch) {
	if (n < 0) {S->c_str[S->memsize+n] = ch;return;}
	if (n >= S->memsize) string_raise("string_asign_at", OUT_OF_BOUNDS);
	S->c_str[n] = ch;
}

char* string_get_c_str(string* S) {
	char* ptr = calloc(S->memsize+1, 1);
	for (int i=0; i<S->memsize; i++) {
		ptr[i] = S->c_str[i];
	}
	return ptr;
}

int string_count(string* S, char ch) {
	int c = 0;
	for (int i=0; i<S->memsize; i++) {
		if (string_get_at(S, i) == ch) c++;
	}
	return c;
}

void string_append(string* S, char* str) {
	if (!str) string_raise("string_append", NULL_STR);
	size_t str_size = strlen(str);
	if (!str_size) return;
	else {
		size_t start = S->memsize;
		string_grow(S, str_size);
		for (int i=0; i<str_size; i++) {string_asign_at(S, start+i, str[i]);}
	}
}

void string_append_int(string* S, int num) {
	if (num < 0) {
		string_grow(S, 1);
		string_asign_at(S, -1, '-');
	}
	int digits = floor(log10(num))+1;
	string_grow(S, digits);
	for (int i=0; i<digits; i++) {
		string_asign_at(S, (i+1)*-1, num%10+'0');
		num/=10;
	}
}

void string_append_char(string* S, char ch) {
	string_grow(S, 1);
	string_asign_at(S, -1, ch);
}

void string_shift(string* S, int start, int end, int steps) {
	if (!steps) return;
	for (int i=end-1; i>=start; i--) {
		string_asign_at(S, i+steps, string_get_at(S, i));
	}
}

void string_shift_left(string* S, int start, int end, int steps) {
	if (!steps) return;
	for (int i=start; i<end; i++) {
		string_asign_at(S, i-steps, string_get_at(S, i));
	}
}

int string_find(string* S, char ch) {
	for (int i=0; i<S->memsize; i++) {
		if (string_get_at(S, i) == ch) return i;
	}
	return -1;
}

int string_rfind(string* S, char ch) {
	for (int i=S->memsize; i>=0; i--) {
		if (string_get_at(S, i) == ch) return i;
	}
	return -1;
}

void string_append_fmt(string* S, char* fmt, ...) {
	va_list argv;
	va_start(argv, fmt);
	string Fmt = string_init(fmt);
	int coincidences = string_count(&Fmt, '%');
	for (int i=0; i<coincidences; i++) {
		int start = string_find(&Fmt, '%');
		switch (string_get_at(&Fmt, start+1)) {
			case 's':
				{
					char* str = va_arg(argv, char*);
					size_t str_size = strlen(str);
					if (str_size >= 2) {
						size_t end = Fmt.memsize;
						string_grow(&Fmt, str_size-2);
						string_shift(&Fmt, start+2, end, str_size-2);
					}
					else {
						size_t end = Fmt.memsize;
						string_shift_left(&Fmt, start+2, Fmt.memsize, str_size);
						string_shrink(&Fmt, str_size);
					}
					for (int i=0; i<str_size; i++) {string_asign_at(&Fmt, start+i, str[i]);}
				}
				break;
			case 'd':
				{
					int num = va_arg(argv, int);
					int digits = floor(log10(num))+1;
					if (num < 0) {digits++;}
					if (digits >= 2) {
						size_t end = Fmt.memsize;
						string_grow(&Fmt, digits-2);
						string_shift(&Fmt, start+2, end, digits-2);
					}
					else {
						string_shift_left(&Fmt, start+2, Fmt.memsize, digits);
						string_shrink(&Fmt, digits);
					}
					if (num < 0) {
						string_asign_at(&Fmt, start, '-');
						start++;
					}
					for (int i=0; i<digits; i++) {
						string_asign_at(&Fmt, start+digits-1-i, num%10+'0');
						num/=10;
					}
				}
				break;
			case 'c':
				{
					char ch = va_arg(argv, int);
					string_shift_left(&Fmt, start+2, Fmt.memsize, 1);
					string_shrink(&Fmt, 1);
					string_asign_at(&Fmt, start, ch);
				}
				break;
		}
	}
	string_append(S, string_get_c_str(&Fmt));
	va_end(argv);
}