#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

void init(int R, int C);
int dropBlocks(int mTimestamp, int mCol, int mLen);
int removeBlocks(int mTimestamp);

#define CMD_INIT 100
#define CMD_DROP 200
#define CMD_REMOVE 300

static bool run()
{
    int query_num;
    scanf("%d", &query_num);

    int ret, ans;
    bool ok = false;

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);

        if (query == CMD_INIT)
        {
            int R, C;
            scanf("%d %d", &R, &C);
            init(R, C);
            ok = true;
        }
        else if (query == CMD_DROP)
        {
            int mTimestamp, mCol, mLen;
            scanf("%d %d %d", &mTimestamp, &mCol, &mLen);
            ret = dropBlocks(mTimestamp, mCol, mLen);
            scanf("%d", &ans);
            if (ans != ret)
            {
                ok = false;
            }
        }
        else if (query == CMD_REMOVE)
        {
            int mTimestamp;
            scanf("%d", &mTimestamp);
            ret = removeBlocks(mTimestamp);
            scanf("%d", &ans);
            if (ans != ret)
            {
                ok = false;
            }
        }

    }
    return ok;
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
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    end = clock() - start;
    printf("elapsed: %f\n", (float) end / CLOCKS_PER_SEC);
    return 0;
}