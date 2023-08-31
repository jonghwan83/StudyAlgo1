
extern void scan(int floorState[3][3]);

extern int move(void);

extern void turn(int mCommand);

#define MAXN 128

int drows[4] = { -1, 0, 1, 0 };
int dcols[4] = { 0, -1, 0, 1 };


struct Robot {
    int row;
    int col;
    int dir;

    void init() {
        row = MAXN / 2;
        col = MAXN / 2;
        dir = 0;
    }
};


class Queue {
public:
    int st, ed, length;
    Robot arr[MAXN * MAXN];

    void init() {
        st = 0; ed = 0; length = 0;
    }

    void push(int row, int col, int dir) {
        arr[ed].row = row; arr[ed].col = col; arr[ed].dir = dir;
        ed++; length++;
    }

    Robot pop() {
        length--;
        return arr[st++];
    }
};

int unclean = 9;
int cleaned = 2;
int wall = 1;



int map[MAXN][MAXN];
int visited[MAXN][MAXN];

int nVisited;

Robot robotInfo;
Queue queue;

void update(int a[3][3]) {

    int rev[3] = { 2, 1, 0 };

    int temp[3][3];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (robotInfo.dir == 1) {
                temp[i][j] = a[j][rev[i]];
            }
            else if (robotInfo.dir == 2) {
                temp[i][j] = a[rev[i]][rev[j]];
            }
            else if (robotInfo.dir == 3) {
                temp[i][j] = a[rev[j]][i];
            }
            else {
                temp[i][j] = a[i][j];
            }
        }
    }

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {

            if (temp[i + 1][j + 1] == 1) {
                map[i + robotInfo.row][j + robotInfo.col] = wall;
            }

        }
    }
}

void scan_norm() {

    int around[3][3];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            int dr = robotInfo.row + i - 1;
            int dc = robotInfo.col + j - 1;

            if (map[dr][dc] == unclean) {
                scan(around);
                update(around);
                return;
            }

        }
    }

}

void turn_norm(int dest) {

    int k = dest - robotInfo.dir;
    if (k < 0) { k += 4; }
    if (k >= 4) { k -= 4; }

    if (k == 0) { return; }

    turn(k);
    robotInfo.dir = dest;
}


int move_norm(bool force) {
    int dr = robotInfo.row + drows[robotInfo.dir];
    int dc = robotInfo.col + dcols[robotInfo.dir];

    if (map[dr][dc] == wall) { return 0; }
    if (map[dr][dc] == cleaned && !force) { return 0; }

    int res = move();

    if (!res) {
        map[dr][dc] = wall;
        return 0;
    }

    map[dr][dc] = cleaned;
    robotInfo.row = dr; robotInfo.col = dc;
    return 1;
}

void go() {
    while (true) {
        int res = move_norm(false);
        if (!res) { break; }
    }
}

int bfs() {
    nVisited++;

    queue.init();

    queue.push(robotInfo.row, robotInfo.col, -1);
    visited[robotInfo.row][robotInfo.col] = nVisited;

    while (queue.length > 0) {
        Robot curr = queue.pop();

        for (int i = 0; i < 4; i++) {
            int dr = curr.row + drows[i];
            int dc = curr.col + dcols[i];

            if (map[dr][dc] == wall) { continue; }

            if (visited[dr][dc] >= nVisited) { continue; }

            if (map[dr][dc] == unclean) {
                if (curr.dir == -1) { return i; }
                else { return curr.dir; }
            }

            if (curr.dir == -1) {
                queue.push(dr, dc, i);
            }
            else {
                queue.push(dr, dc, curr.dir);
            }

            visited[dr][dc] = nVisited;
        }
    }

    return -1;
}

void init()
{
    nVisited = 0;

    for (int r = 0; r < MAXN; r++) {
        for (int c = 0; c < MAXN; c++) {
            visited[r][c] = 0;
        }
    }

}



void cleanHouse()
{

    for (int r = 0; r < MAXN; r++) {
        for (int c = 0; c < MAXN; c++) {

            map[r][c] = unclean;
        }
    }

    robotInfo.init();
    map[robotInfo.row][robotInfo.col] = cleaned;

    while (true) {
        int dr = robotInfo.row + drows[robotInfo.dir];
        int dc = robotInfo.col + dcols[robotInfo.dir];

        if (map[dr][dc] == unclean) {
            go();
        }
        else if (map[dr][dc] == wall) {
            scan_norm();
        }

        int res = bfs();
        if (res == -1) { break; }
        turn_norm(res);
        move_norm(true);
    }

}