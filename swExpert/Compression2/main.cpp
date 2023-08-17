#include <stdio.h>
#include <time.h>


void comp(char map[10000][10000], char data[150000]);
void decomp(char map[10000][10000], char data[150000]);


static char map[10000][10000];
static char map_org[10000][10000];
static char result[10000][10000];
static char data[150000];


static unsigned short random(void)
{
    static unsigned long long seed = 5;

    return ((unsigned short)((seed = seed * 25214903917ULL + 11ULL) >> 16));
}

static void build(void)
{
    for (register int y = 0; y <= 9999; y++)
        for (register int x = 0; x <= 9999; x++)
            map_org[y][x] = map[y][x] = 0;

    for (int c = 0; c < 10000; c++)
    {
        int sx = random() % 10000;
        int sy = random() % 10000;
        int ex = random() % 10000;
        int ey = random() % 10000;

        if ((sx >= ex) || (sy >= ey))
        {
            c--;
            continue;
        }

        for (register int x = sx; x <= ex; x++)
        {
            map_org[sy][x] = map[sy][x] = 1;
            map_org[ey][x] = map[ey][x] = 1;
        }

        for (register int y = sy + 1; y <= ey - 1; y++)
        {
            map_org[y][sx] = map[y][sx] = 1;
            map_org[y][ex] = map[y][ex] = 1;
        }
    }
}


static bool verify(void)
{
    for (register int y = 0; y <= 9999; y++)
        for (register int x = 0; x <= 9999; x++)
            if (map_org[y][x] != result[y][x]) return false;

    return true;
}


int main()
{
    build();

    time_t SCORE = clock();
    comp(map, data);
    decomp(result, data);
    SCORE = (clock() - SCORE) / (CLOCKS_PER_SEC / 1000);

    if (verify() == false) SCORE = 100000000;

    printf("SCORE: %d\n", SCORE);

    return 0;
}
