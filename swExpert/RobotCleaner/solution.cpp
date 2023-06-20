extern void scanFromRobot(int ceilingState[3][3]);
extern int moveRobot(int mCommand);

#include <iostream>

#define MAXN 30

int visited[MAXN][MAXN];
int nVisited;

class Robot {
public:
    int row, col;
    int dir;

    void init() {
        row = 1; col = 1; dir = 0;
    }
};

class Queue {
public:
    Robot arr[MAXN * MAXN];
    int length, st, ed;

    void init() {
        length = 0; st = 0; ed = 0;
    }

    void push(int r, int c, int d) {
        length++;
        Robot p; p.row = r; p.col = c; p.dir = d;
        arr[ed++] = p;
    }

    Robot pop() {
        length--;
        return arr[st++];
    }
};


// ↑(0)  ←(1)  ↓(2)  →(3)

int drows[4] = { -1, 0, 1, 0 };
int dcols[4] = { 0, -1, 0, 1 };
int rev[3] = { 2, 1, 0 };




Robot robot;
int house[MAXN][MAXN];
int n;
int nextDir;

int convert(int a) {
    if (a < 0) { return a + n; }
    if (a >= n) { return a - n; }
    return a;
}

int rotate(int a) {
    if (a < 0) { return a + 4; }
    if (a > 3) { return a - 4; }
    return a;
}


void checkHouse() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (i == robot.row && j == robot.col) {
                printf("%c ", 'A');
            }
            else {
                if (house[i][j] == -1) { printf("%c ", 'X'); }
                else { printf("%d ", house[i][j]); }
            }

        }
        printf("\n");
    }
    printf("\n");
}


void init(int N, int subTaskCount)
{
    n = N;

    nVisited = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visited[i][j] = 0;
        }
    }
}

void scan() {
    int around[3][3];

    scanFromRobot(around);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            int r = convert(robot.row + i - 1);
            int c = convert(robot.col + j - 1);
            if (house[r][c] >= 0) { continue; }

            if (robot.dir == 0) {
                house[r][c] = around[i][j];
            }

            else if (robot.dir == 1) {
                house[r][c] = around[j][rev[i]];
            }
            else if (robot.dir == 2) {
                house[r][c] = around[rev[i]][rev[j]];
            }

            else {
                house[r][c] = around[rev[j]][i];
            }

        }
    }

}

int findUnclean() {
    nVisited++;
    nextDir = 100;

    int ans = 0;

    Queue queue; queue.init();
    queue.push(robot.row, robot.col, -1);
    visited[robot.row][robot.col] = nVisited;

    while (queue.length > 0) {
        Robot curr = queue.pop();

        if (ans > 0) { break; }

        for (int i = 0; i < 4; i++) {
            int dr = convert(curr.row + drows[i]);
            int dc = convert(curr.col + dcols[i]);

            if (house[dr][dc] == -1) { continue; }
            if (house[dr][dc] == 1) { continue; }
            if (visited[dr][dc] < nVisited) {
                if (house[dr][dc] == 0) {
                    ans++;
                    if (nextDir == 100) { nextDir = curr.dir; }
                }
                visited[dr][dc] = nVisited;

                if (curr.dir == -1) { queue.push(dr, dc, i); }
                else { queue.push(dr, dc, curr.dir); }

            }
        }
    }

    return ans;
}

void move(int dir) {
    int res = moveRobot(rotate(dir - robot.dir));
    robot.dir = dir;

    if (res == 0) { return; }

    robot.dir = dir;
    robot.row = convert(robot.row + drows[dir]);
    robot.col = convert(robot.col + dcols[dir]);

    house[robot.row][robot.col] = 2;
}

void cleanHouse(int mLimitMoveCount)
{
    robot.init();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            house[i][j] = -1;
        }
    }

    house[robot.row][robot.col] = 2;


    while (true) {
        scan();
        // checkHouse();

        bool flagNear = true;

        for (int i = 0; i < 4; i++) {
            int k = rotate(i + robot.dir);
            int dr = convert(robot.row + drows[k]);
            int dc = convert(robot.col + dcols[k]);

            if (house[dr][dc] == -1) { continue; }
            if (house[dr][dc] == 0) {
                move(k);
                flagNear = false;
                break;
            }
        }

        if (flagNear) {
            int k = findUnclean();
            if (k == 0) { return; }
            move(nextDir);
        }
    }
}