#include <stddef.h>
typedef struct {
	char* c_str;
	size_t size;
	size_t memsize;
} string;
typedef enum {
	NULL_STR,
	NULL_OBJ,
	OUT_OF_BOUNDS,
	NEG_INT
} string_exception;
void* _realloc(void* ptr, size_t size);
void* _realloc2(void* ptr, size_t size);
string string_init(char* str);
void string_raise(string_exception ex, char* data);
void string_append(string* S, char* str);
int string_count(string* S, int c);
void string_shift(char* ptr, int n);
void string_append_fmt(string* S, const char* fmt, ...);
void string_append_char(string* S, char c);
void string_append_int(string* S, int n);
void string_append_pointer(string* S, void* p);
size_t string_size(string* S);
void string_print_raw(string *S);