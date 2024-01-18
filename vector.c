#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "vector.h"

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
	char* ptr = S->c_str+S->memsize;
	S->memsize += size;
	memset(ptr, 0, size);
}

void string_shrink(string* S, size_t size) {
	S->c_str = _realloc2(S->c_str, S->memsize-size);
	S->memsize -= size;
	S->c_str[S->memsize-1] = 0;
}

void string_raise(string_exception ex, char* data) {
	printf("--- Libvector: String Error ---\n%s\n\t", data);
	switch (ex) {
		case NULL_STR:
			puts("NULL_STR: Expected char* but passed a NULL pointer.");
			break;
		case NULL_OBJ:
			puts("NULL_OBJ: Expected string* object but passed a NULL pointer.");
			break;
		default:
			;
	}
}

string string_init(char* str) {
	size_t str_size = str ? strlen(str) : 0;
	char* _str = str_size ? strdup(str) : NULL;
	string S = {str, str_size, str_size ? str_size+1 : 0};
	return S;
}

void string_append(string* S, char* str) {
	const char* fmt = "\tIn %s,\n\t\tS = %p\n\t\t\t.c_str = %s\n\t\t\t.size = %d\n\tstr = %s";
	if (!str) {
		string data = string_init(NULL);
		string_raise(NULL_STR, data.c_str);
	}
	size_t size = strlen(str);
	if (!size) return;
	if (!S->c_str) {
		S->c_str = strdup(str); S->size=size; S->memsize=size+1;
		return;
	}
	string_grow(S, size);
	S->size += size;
	strcat(S->c_str, str);
}

int string_count(string *S, int c) {
	int count=0;
	if (!S) {
		string data = string_init(NULL);
		string_raise(NULL_OBJ, data.c_str);
	}
	for (int i=0; i<S->size; i++) {
		if (S->c_str[i] == c) count++;
	}
	return count;
}

void string_shift(char* ptr, int n) {
	for (int i=0; i<n; i++) {
		char* end = ptr+strlen(ptr);
		while (end != ptr-1) {
			*(end+1) = *end;
			end--;
		}
	}
}

void string_shift_left(char* ptr, int n) {
	for (int i=0; i<n; i++) {
		char* e = ptr+i;
		while (*e != 0) {
			*(e-1) = *e;
			e++;
		}
	}
}

void string_append_fmt(string* S, const char* fmt, ...) {
	va_list argv;
	va_start(argv, fmt);
	string_append(S, (char*)fmt);
	int count=string_count(S, '%');
	for (int i=0; i<count; i++) {
		char* ptr = strchr(S->c_str, '%');
		switch (*(ptr+1)) {
			case 's':
				{
				char* str = va_arg(argv, char*);
				size_t str_size = strlen(str);

				if (str_size >= 2) {
					size_t S_tempsize = S->size;
					string_realloc(S, S->size + 1 + (str_size-2));
					string_print_raw(S);
					char* tmpzero = S->c_str+S_tempsize;
					memset(tmpzero, 0, str_size+1-2);
				}

				//move
				string_print_raw(S);
				ptr = strchr(S->c_str, '%');
				if (str_size >= 2) string_shift(ptr+2, str_size-2);
				else {
					string_shift_left(ptr+2, 2-str_size);
					string_print_raw(S);
					string_realloc(S, S->memsize-(2-str_size));
					S->c_str[S->memsize-1] = 0;
					puts("left");
				}
				string_print_raw(S);

				//fill
				for (int i=0; i<str_size; i++) {ptr[i] = str[i];}
				string_print_raw(S);puts("");
				S->size += str_size-2;
				}
				break;
			case 'd':
				break;
			case 'p':
				break;
		}
	}
	va_end(argv);
}

void string_append_char(string* S, char c) {
	string_realloc(S, S->size + 2);
	S->size += 1;
	S->c_str[S->size-1] = c;
}

void string_append_int(string* S, int n) {
	if (n < 0) {
		char* data=NULL;
		string_raise(NEG_INT, data);
	}
	int digits = floor(log10(n))+1;
	string_grow(S, digits);
	printf("digits: %d\n", digits);
	S->size += digits;
	string_print_raw(S);
	for (int i=0; i<digits; i++) {
		S->c_str[S->size-i-1] = (n%10)+'0';
		n /= 10;
	}
}

void string_append_pointer(string* S, void* p) {
	char* address = calloc(19, 1);
	snprintf(address, 18, "0x%lx", (size_t)p);
	string_append(S, address);
}

size_t string_size(string* S) {return S->size;}

void string_print_raw(string *S) {
	printf("print,size:%lu,mem:%lu\n", S->size, S->memsize);
	fputs("\"", stdout);
	for (int i=0; i<S->memsize; i++) {
		S->c_str[i] ? printf("%c", S->c_str[i]) : printf("\\0");
	}
	fputs("\"\n", stdout);
}