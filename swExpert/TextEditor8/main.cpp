
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdio.h>
#include <string.h>
#include <time.h>


extern void append(char* string);
extern void insert(int line, char* string);
extern void erase(int line);
extern char* linestring(int line);


static inline int psuedo_rand(void)
{
    static unsigned long long seed = 55;

    seed = seed * 25214903917ULL + 11ULL;

    return (seed >> 16) & 0x7fffffff;
}


static char* rndstr(void)
{
    static char str[1024] = { 0 };

    if (str[0] == 0) {
        for (int c = 0; c < 1023; c++) {
            str[c] = 'A' + psuedo_rand() % 26;
        }
    }

    char c = 'A' + psuedo_rand() % 26;
    str[psuedo_rand() % 1023] = c;
    str[512 + psuedo_rand() % 512] = 0;

    return str;
}


int main()
{
    static const int MAX = 100000;

    int SCORE;
    int size;

    SCORE = clock() / (CLOCKS_PER_SEC / 1000);

    for (int c = 0; c < 1000; c++) {
        append(rndstr());
    }
    size = 1000;

    while (size < MAX) {
        if (psuedo_rand() % 10 == 0) {
            append(rndstr());
            size++;
        }

        if (psuedo_rand() % 2 == 0) {
            insert(psuedo_rand() % size, linestring(psuedo_rand() % size));
            size++;
        }
        else
        {
            erase(psuedo_rand() % size);
            size--;
        }
    }

    SCORE = clock() / (CLOCKS_PER_SEC / 1000) - SCORE;
    if (strcmp(linestring(psuedo_rand() % MAX), "YCASELNZOBUPSTVOXYKSGQRIOQDWSOBGZRQWIWMZZOFYYTDBZFYXOTKPWILNKDMQSNAYELAMAJVOIOODQDIVWKYOGDPUCMULUJNUTSBGRAPZCDDQQNAXAKPAERAHIWARBXMSCJOCLIBAENAGLFISHSMPIJJFTCNEOWBPMYQGOECHUCWGZUNVZXUONDVRAXNURNBFAPMUGZBYGCDXVQKSYIBYGFGPAPUKIJZYPEQMSRVDKCAFOBSXTUQNHVNVOFLOXPPKLYFHDPNPXGVLVOCCSINCWVOOVFQZKBAUHPGBUUFQOAJVONLWBPDHBOHVTQIARYKPIROVWONZQVMNGIMLKPTLVSXXFXGTGMTXNNPCJFCDSBFIHEOJSWLHWPOGOMKJZVDTKDPHUBFBDSAZYAYGHHBBUEOPKSKFOWWQQRTJEZJFEKUZWXVGLAVYYGVRAUCQRQVNJWDQGSDWQERQKSQCRICBTLPKAZEKNXLCTEZHJQBJFBBJBSHOETZETHQAKNSMUB") != 0) {
        SCORE += 100000000;
    }

    printf("SCORE: %d\n", SCORE);

    return 0;
}
