#ifndef __UTILS__H
#define __UTILS__H

void limit (int& i, int min, int max) {
	if (i > max) i = max;
	else if (i < min) i = min;
};

void limit (float& i, float min, float max) {
	if (i > max) i = max;
	else if (i < min) i = min;
};

#endif
