#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

#define MAXL (11) 

#define CMD_INIT   (100)
#define CMD_ADD    (200)
#define CMD_GET_DISTANCE (300)
#define CMD_GET_COUNT  (400)

extern void init(char mRootSpecies[MAXL]);
extern void add(char mSpecies[MAXL], char mParentSpecies[MAXL]);
extern int getDistance(char mSpecies1[MAXL], char mSpecies2[MAXL]);
extern int getCount(char mSpecies[MAXL], int mDistance);

static bool run()
{
    int Q;
    char mRootSpecies[MAXL], mSpecies[MAXL], mParentSpecies[MAXL], mSpecies1[MAXL], mSpecies2[MAXL];
    int mDistance;

    int ret = -1, ans;

    scanf("%d", &Q);

    bool okay = false;

    for (int q = 0; q < Q; ++q)
    {
        int cmd;
        scanf("%d", &cmd);

        switch (cmd)
        {
        case CMD_INIT:
            scanf("%s", &mRootSpecies);
            init(mRootSpecies);
            okay = true;
            break;
        case CMD_ADD:
            scanf("%s %s", mSpecies, mParentSpecies);
            add(mSpecies, mParentSpecies);
            break;
        case CMD_GET_DISTANCE:
            scanf("%s %s", mSpecies1, mSpecies2);
            ret = getDistance(mSpecies1, mSpecies2);
            scanf("%d", &ans);
            if (ret != ans)
                okay = false;
            break;
        case CMD_GET_COUNT:
            scanf("%s %d", mSpecies, &mDistance);
            ret = getCount(mSpecies, mDistance);
            scanf("%d", &ans);
            if (ret != ans)
                okay = false;
            break;
        default:
            okay = false;
            break;
        }
    }

    return okay;
}

int main()
{
    clock_t start, end;
    start = clock();

    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int TC, MARK;

    scanf("%d %d", &TC, &MARK);
    for (int tc = 1; tc <= TC; ++tc)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    end = clock() - start;
    printf("elapsed: %f\n", (float)end / CLOCKS_PER_SEC);

    return 0;
}
