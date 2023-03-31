#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

extern void init(int N, int mUID1[], int mUID2[], int mBandwidth[]);
extern int share(int mUID, char mFilename[], int mFilesize);
extern int request(int mUID, char mFilename[], int mTID);
extern int complete(int mTID);

#define INIT (100)
#define SHARE (200)
#define REQUEST (300)
#define COMPLETE (400)


static bool run()
{
    int query_num;
    scanf("%d", &query_num);

    int n = 0;
    bool ok = false;

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);

        if (query == INIT)
        {
            static int uid1[200];
            static int uid2[200];
            static int bandwidth[200];
            scanf("%d", &n);
            for (int i = 0; i < n - 1; i++)
                scanf("%d", &uid1[i]);
            for (int i = 0; i < n - 1; i++)
                scanf("%d", &uid2[i]);
            for (int i = 0; i < n - 1; i++)
                scanf("%d", &bandwidth[i]);
            init(n, uid1, uid2, bandwidth);

            ok = true;
        }
        else if (query == SHARE)
        {
            int uid, filesize;
            static char filename[10];
            scanf("%d %s %d", &uid, filename, &filesize);
            int ret = share(uid, filename, filesize);
            int ans;
            scanf("%d", &ans);
            if (ret != ans)
            {
                ok = false;
            }
        }
        else if (query == REQUEST)
        {
            int uid, tid;
            char filename[10];
            scanf("%d %s %d", &uid, filename, &tid);
            int ret = request(uid, filename, tid);
            int ans;
            scanf("%d", &ans);
            if (ret != ans)
            {
                ok = false;
            }
        }
        else if (query == COMPLETE)
        {
            int tid;
            scanf("%d", &tid);
            int ret = complete(tid);
            int ans;
            scanf("%d", &ans);
            if (ret != ans)
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
    scanf("%d%d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    end = clock() - start;
    printf("elapsed: %f\n", (float) end / CLOCKS_PER_SEC);

    return 0;
}