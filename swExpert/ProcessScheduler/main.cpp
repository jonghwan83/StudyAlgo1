#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

extern void init(int mQuantum);
extern int addProcess(int tStamp, int pId, int mTime, int mPriority, int completedIds[]);
extern int checkProcess(int tStamp, int completedIds[]);

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_CHECK 3

static bool run() {
    int q;
    scanf("%d", &q);

    static int completedIds[50000];
    int quantum, tstamp, burst, priority;
    int cmd, ans, ret, pid = 1;
    bool okay = false;

    for (int i = 0; i < q; ++i) {
        scanf("%d", &cmd);
        switch (cmd) {
        case CMD_INIT:
            scanf("%d", &quantum);
            init(quantum);
            okay = true;
            break;
        case CMD_ADD:
            scanf("%d %d %d %d", &tstamp, &burst, &priority, &ans);
            ret = addProcess(tstamp, pid++, burst, priority, completedIds);
            if (ans != ret)

                okay = false;
            break;
        case CMD_CHECK:
            scanf("%d %d", &tstamp, &ans);
            ret = checkProcess(tstamp, completedIds);
            if (ans != ret)
                okay = false;
            break;
        default:
            okay = false;
            break;                                           
        }
    }
                      
    scanf("%d", &q);
    for (int i = 0; i < q; ++i) {
        scanf("%d", &ans);
        if (completedIds[i] != ans)
            okay = false;
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