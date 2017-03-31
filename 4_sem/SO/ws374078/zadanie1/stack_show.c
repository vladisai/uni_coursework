#include "stdio.h"

#define SHIFT 0

extern void *__libc_stack_end;
extern char __executable_start;
extern char __etext;

int is_function(void *ptr);

void *get_address(void *ptr) {
	int k = *(int*)(ptr - 4);
	//printf("aaa %p\n", (int*)(ptr - 4));
	//printf("bbb %p\n", (char*)(ptr - 4));
	return ptr + k;
}

void stack_show() {
	//printf("stack show %p\n", (void*)stack_show);
	int *p = 1; // the pointer is allocated somewhere on the stack (on its top)
	// the pointer's address would be the address on the stack, right?
	// to find pointer's address we need to do &p
	void *top = &p;
	while (top != __libc_stack_end) {
		// to find what the address is pointing to we need to do what?
		// to dereference the top
		// and cast it to void*?
		void *ptr = *(int**)top;
		if (is_function(ptr)) {
			//printf("A function was found!\n");
			//printf("top(%p) points to something here (%p)\n", top, ptr);
			printf("%016x\n", get_address(ptr));
		} else {
			//printf("                  not a function\n                top(%p) points to something here (%p)\n", top, ptr);
		}
		top += 1;
	}
}

int is_function(void *ptr) {
	if (ptr >= &__executable_start - 5 && ptr < &__etext + 5) {
		unsigned char prev = *((char*)ptr - 5);
		//printf("%p\n", (char*)ptr - 5);
				
		//printf("%x\n", prev);
		if (prev == 232) {
			return 1;
		} else {
			return 0;
		}
		return 1;
	} else {
		return 0;
	}
}
