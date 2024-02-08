#include <stddef.h>
typedef struct {
	char* c_str;
	size_t memsize;
} string;
typedef enum {
	NULL_STR,
	NULL_OBJ,
	OUT_OF_BOUNDS,
} string_exception;
void* _realloc(void* ptr, size_t size);
void* _realloc2(void* ptr, size_t size);
string string_init(char* str);
void string_raise(char* function, string_exception exception);
char string_get_at(string* S, int n);
void string_asign_at(string* S, int n, char ch);
char* string_get_c_str(string* S);
int string_count(string* S, char ch);
void string_append(string* S, char* str);
void string_append_int(string* S, int num);
void string_append_char(string* S, char ch);
int string_find(string* S, char ch);
int string_rfind(string* S, char ch);
void string_append_fmt(string* S, char* fmt, ...);