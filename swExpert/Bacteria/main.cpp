
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

#define MAX_BCNT 100
#define MAX_NAME 10

extern void init(int N, char bNameList[MAX_BCNT][MAX_NAME], int mHalfTime[MAX_BCNT]);
extern void addBacteria(int tStamp, char bName[MAX_NAME], int mLife, int mCnt);
extern int takeOut(int tStamp, int mCnt);
extern int checkBacteria(int tStamp, char bName[MAX_NAME]);

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 0
#define CMD_ADD  1
#define CMD_OUT  2
#define CMD_CHECK 3

static bool run() {
    int q;
    scanf("%d", &q);

    int time, life, cnt, halftime[MAX_BCNT];
    char bname[MAX_BCNT][MAX_NAME];
    int cmd, ans, ret;
    bool okay = false;

    for (int i = 1; i < q + 1; ++i) {
        scanf("%d", &cmd);
        switch (cmd) {
        case CMD_INIT:
            scanf("%d", &cnt);
            for (int k = 0; k < cnt; k++) {
                scanf("%s %d", bname[k], &halftime[k]);
            }
            init(cnt, bname, halftime);
            okay = true;
            break;
        case CMD_ADD:
            scanf("%d %s %d %d", &time, bname[0], &life, &cnt);
            addBacteria(time, bname[0], life, cnt);
            break;
        case CMD_OUT:
            scanf("%d %d %d", &time, &cnt, &ans);
            ret = takeOut(time, cnt);
            if (ans != ret)
                okay = false;
            break;
        case CMD_CHECK: {
            scanf("%d %s %d", &time, bname[0], &ans);
            ret = checkBacteria(time, bname[0]);
            if (ans != ret)
                okay = false;
            break;
        }
        default:
            okay = false;
            break;
        }
    }
    return okay;
}

int main() {
    clock_t start, end;
    start = clock();

    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++) {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }

    end = clock() - start;
    printf("elapsed: %f\n", (float)end / CLOCKS_PER_SEC);

    return 0;
}

