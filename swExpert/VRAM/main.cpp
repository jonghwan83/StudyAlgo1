
#ifndef _CRT_SECURE_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS

#endif



#include <stdio.h>

#include <string.h>

#include <time.h>



extern void fill_rect(int x1, int y1, int x2, int y2);

extern void clear_rect(int x1, int y1, int x2, int y2);

extern void reverse_rect(int x1, int y1, int x2, int y2);



extern unsigned char* get_vram(void);



static unsigned long long RSEED;

static unsigned char      VRAM[1024 * 8192] = { 0 };



static inline int pseudo_rand(void)

{

	RSEED = RSEED * 25214903917ULL + 11ULL;

	return (RSEED >> 16) & 0x7fffffff;

}

static void _set_pixel(int x1, int y1)

{

	int xbase = x1 / 8;

	int xoffset = x1 % 8;

	int y = (y1 % 4) * 4 + (y1 / 4);

	VRAM[y * xbase] |= 0x80 >> xoffset;

}

static void _reset_pixel(int x1, int y1)

{

	int xbase = x1 / 8;

	int xoffset = x1 % 8;

	int y = (y1 % 4) * 4 + (y1 / 4);



	VRAM[y * xbase] &= ~(0x80 >> xoffset);

}

static int _get_pixel(int x1, int y1)

{

	int xbase = x1 / 8;

	int xoffset = x1 % 8;

	int y = (y1 % 4) * 4 + (y1 / 4);

	return VRAM[y * xbase] & (0x80 >> xoffset);

}

static void _fill_rect(int x1, int y1, int x2, int y2)

{

	for (int y = y1; y <= y2; y++) {

		for (int x = x1; x <= x2; x++) {

			_set_pixel(x, y);

		}

	}

}



static void _clear_rect(int x1, int y1, int x2, int y2)

{

	for (int y = y1; y <= y2; y++) {

		for (int x = x1; x <= x2; x++) {

			_reset_pixel(x, y);

		}

	}

}



static void _reverse_rect(int x1, int y1, int x2, int y2)

{

	for (int y = y1; y <= y2; y++) {

		for (int x = x1; x <= x2; x++) {

			if (_get_pixel(x, y) == 0) {

				_set_pixel(x, y);

			}

			else {

				_reset_pixel(x, y);

			}

		}

	}

}



int main()

{

	const int TRY = 256; // will be 100000

	RSEED = 55L; // will be changed

	int _rseed = RSEED;

	int SCORE = clock() / (CLOCKS_PER_SEC / 1000);



	for (int J = 0; J < TRY; J++) {

		int r = pseudo_rand() % 3;

		int x1 = pseudo_rand() % 8192;

		int y1 = pseudo_rand() % 8192;

		int x2 = pseudo_rand() % 8192;

		int y2 = pseudo_rand() % 8192;



		if ((x1 > x2) || (y1 > y2)) {

			continue;

		}

		if (r == 0) {

			fill_rect(x1, y1, x2, y2);

		}

		else if (r == 1) {

			clear_rect(x1, y1, x2, y2);

		}

		else {

			reverse_rect(x1, y1, x2, y2);

		}

	}

	unsigned char* RESULT = get_vram();

	SCORE = clock() / (CLOCKS_PER_SEC / 1000) - SCORE;

	RSEED = _rseed;

	for (int J = 0; J < TRY; J++) {

		int r = pseudo_rand() % 3;

		int x1 = pseudo_rand() % 8192;

		int y1 = pseudo_rand() % 8192;

		int x2 = pseudo_rand() % 8192;

		int y2 = pseudo_rand() % 8192;

		if ((x1 > x2) || (y1 > y2)) {

			continue;

		}

		if (r == 0) {

			_fill_rect(x1, y1, x2, y2);

		}

		else if (r == 1) {

			_clear_rect(x1, y1, x2, y2);

		}

		else {

			_reverse_rect(x1, y1, x2, y2);

		}

	}

	if (memcmp(VRAM, RESULT, 1024 * 8192) != 0) {

		SCORE += 100000000;

	}



	printf("SCORE: %d\n", SCORE);

	return 0;

}
