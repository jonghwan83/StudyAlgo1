#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdio.h>
#include <time.h>


extern void save(int money[8]);
extern void withdraw(int account[1000], int result[1000][8]);


static int money[8];
static int _money[8];
static int account[1000];
static int _account[1000];
static int result[1000][8];


static inline int rand(void)
{
    static unsigned long long seed = 55; // This variable can be modified.

    seed = seed * 25214903917ULL + 11ULL;

    return (seed >> 16) & 0x7fffffff;
}


int main()
{

    long long SCORE = 0;
    clock_t start = clock();

    for (int J = 0; J < 100; J++) {

    for (int c = 0; c < 8; c++) {
        _money[c] = money[c] = 500 + rand() % 1500;
    }


    save(money);

    for (int c = 0; c < 1000; c++) {
        _account[c] = account[c] = (rand() % 10000 + 1) * 10;
    }

    withdraw(account, result);

    for (int c = 0; c < 1000; c++) {
    if (result[c][0] < 0 ||
        result[c][1] < 0 ||
        result[c][2] < 0 ||
        result[c][3] < 0 ||
        result[c][4] < 0 ||
        result[c][5] < 0 ||
        result[c][6] < 0 ||
        result[c][7] < 0) {
        SCORE += 1000;
        continue;
    }

    if (_account[c] != result[c][0] * 50000 +
                        result[c][1] * 10000 +
                        result[c][2] * 5000 +
                        result[c][3] * 1000 +
                        result[c][4] * 500 +
                        result[c][5] * 100 +
                        result[c][6] * 50 +
                        result[c][7] * 10) {
                        SCORE += 1000;
                        continue;
    }

    for (int d = 0; d < 8; d++) _money[d] -= result[c][d];
    }

    for (int c = 0; c < 8; c++) {
        if (_money[c] < 0) {
            SCORE += 100000000;
        }
    }
    }

    SCORE += (clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf("SCORE: %lld\n", SCORE);

    return 0;
} 
