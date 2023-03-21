#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

#define MAXN 8000

extern void compose(int N, int mTone[MAXN][2][4], int mBeat[MAXN][2][4], int mAns[MAXN]);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int mTone[MAXN][2][4];
static int mBeat[MAXN][2][4];
static int mAns[MAXN];

static int run(int score)
{
    int N;

    scanf("%d", &N);

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 4; ++k)
                scanf("%d", &mTone[i][j][k]);

            for (int k = 0; k < 4; ++k)
                scanf("%d", &mBeat[i][j][k]);
        }

    compose(N, mTone, mBeat, mAns);

    for (int i = 0; i < N; ++i)
    {
        int ans;
        scanf("%d", &ans);
        if (mAns[i] != ans)
            score = 0;
    }

    return score;
}

int main()
{
    clock_t start, end;
    start = clock();

    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        printf("#%d %d\n", tc, run(MARK));
    }

    end = clock() - start;
    printf("elapsed: %f\n", (float) end / CLOCKS_PER_SEC);

    return 0;
}