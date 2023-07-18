#ifndef _CRT_SECURE_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS

#endif





#include <stdio.h>

#include <time.h>

#include <memory.h>





int  comp(unsigned char dest[10000000], unsigned char src[10000000]);

void decomp(unsigned char src[10000000], unsigned char dest[10000000]);





static inline int rand(void)

{

    static unsigned long long seed = 55;



    seed = seed * 25214903917ULL + 11ULL;



    return (seed >> 16) & 0x7fffffff;

}





int main()

{

    static const char* TBL = "JAGANJNJJGANJJAAJJEAJJOAJAJJGJJNJKJSNNJJJAJJNAAJAJAJJGAJJANJJJAJ";



    static unsigned char src[10000000];

    static unsigned char _src[10000000];

    static unsigned char dest[10000000];



    long long SCORE = 0LL;



    for (register int J = 0; J < 10; J++) {

        for (register int c = 0; c < 10000000; c++) {

            _src[c] = src[c] = TBL[rand() % 64];

        }



        int start = clock() / (CLOCKS_PER_SEC / 1000);



        int size = comp(dest, src);



        memset(dest + size, rand() % 256, 10000000 - size);

        memset(src, 0, 10000000);



        decomp(src, dest);



        SCORE += clock() / (CLOCKS_PER_SEC / 1000LL) - start;



        if (memcmp(src, _src, 10000000) != 0) {

            SCORE += 1000000000000LL;

        }

        else {

            SCORE += size * 100000LL;

        }

    }



    printf("SCORE: %lld\n", SCORE);



    return 0;

}
