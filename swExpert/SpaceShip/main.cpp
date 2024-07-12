
#ifndef _CRT_SECURE_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS

#endif

#include <stdio.h>

#include <time.h>

static unsigned long long seed = 5;

static int pseudo_rand(void)

{

    seed = seed * 25214903917ULL + 11ULL;

    return (seed >> 16) & 0x3fffffff;
}

/* These constant variables will NOT be changed */

static const long long PENALTY = 0LL;

static const int MAX_TC = 10;

static const int MAP_SIZE = 1'000;

static const int SPACESHIP_NUM = 10'000;

static int map_org[MAP_SIZE][MAP_SIZE];

static int map_bak[MAP_SIZE][MAP_SIZE];

static bool occupied[MAP_SIZE][MAP_SIZE];

struct Spaceship
{

    int height;

    int width;
};

static struct Spaceship spaceship_org[SPACESHIP_NUM];

static struct Spaceship spaceship_bak[SPACESHIP_NUM];

static int rows[SPACESHIP_NUM];

static int cols[SPACESHIP_NUM];

static int dirs[SPACESHIP_NUM];

static long long SCORE = 0;

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

//////////////////////////

static void make_tc()

{

    for (register int y = 0; y < MAP_SIZE; y++)

    {

        for (register int x = 0; x < MAP_SIZE; x++)

        {

            map_org[y][x] = map_bak[y][x] = pseudo_rand() % 125;

            occupied[y][x] = false;
        }
    }

    for (int i = 0; i < SPACESHIP_NUM; i++)

    {

        spaceship_org[i].height = spaceship_bak[i].height = pseudo_rand() % 4 + 2;

        spaceship_org[i].width = spaceship_bak[i].width = pseudo_rand() % 4 + 2;

        rows[i] = cols[i] = dirs[i] = -1;
    }
}

static bool verify()

{

    for (int i = 0; i < SPACESHIP_NUM; i++)

    {

        if (rows[i] == -1 || cols[i] == -1 || dirs[i] == -1)

            continue;

        int y1 = rows[i], x1 = cols[i], y2, x2;

        if (dirs[i] == 0)

        {

            y2 = y1 + spaceship_org[i].height - 1;

            x2 = x1 + spaceship_org[i].width - 1;
        }

        else

        {

            y2 = y1 + spaceship_org[i].width - 1;

            x2 = x1 + spaceship_org[i].height - 1;
        }

        if (y1 < 0 || x1 < 0 || y2 >= MAP_SIZE || x2 >= MAP_SIZE)

            return false;

        int lo = MIN(MIN(MIN(map_org[y1][x1], map_org[y1][x2]), map_org[y2][x1]), map_org[y2][x2]);

        int hi = MAX(MAX(MAX(map_org[y1][x1], map_org[y1][x2]), map_org[y2][x1]), map_org[y2][x2]);

        if (hi - lo > 6)

            return false;

        for (int y = y1; y <= y2; y++)

        {

            for (int x = x1; x <= x2; x++)

            {

                if (occupied[y][x] == true)

                    return false;

                occupied[y][x] = true;
            }
        }

        SCORE += (spaceship_org[i].height * spaceship_org[i].width * MIN(spaceship_org[i].height, spaceship_org[i].width));
    }

    return true;
}

extern void init(int[][MAP_SIZE], struct Spaceship[]);

extern void process(int[], int[], int[]);

int main()

{
    clock_t start = clock();

    setbuf(stdout, NULL);

    SCORE = 0;

    for (int tc = 0; tc < MAX_TC; ++tc)
    {

        make_tc();

        init(map_bak, spaceship_bak);

        process(rows, cols, dirs);

        if (verify() == false)
        {

            SCORE = PENALTY;

            break;
        }
    }

    printf("SCORE: %lld\n", SCORE);

    clock_t end = clock() - start;

    printf("elapsed: %f\n", (float) end / CLOCKS_PER_SEC);

    return 0;
}
