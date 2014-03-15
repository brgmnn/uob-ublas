#include <utils.h>

void swap_ptr(void *x, void *y) {
	void *tmpx = x;
	x = y;
	y = tmpx;
}