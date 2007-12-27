#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <nds.h>

using namespace std;

typedef struct {
	const u16* bitmap;
	u16 width;
	u16 height;
} Bitmap;

#endif
