#define MAX_MAP_SIZE 2000

struct Point
{
    int r;
    int c;
};

#define MAXK 50
#define MAXNK 60

class HeapNode {
public:
    int height;
    int row;
    int col;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXK * MAXK];

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].height < arr[child].height) { return true; }
        if (arr[parent].height == arr[child].height && arr[parent].row > arr[child].row) { return true; }
        if (arr[parent].height == arr[child].height && arr[parent].row == arr[child].row &&
            arr[parent].col > arr[child].col) {
            return true;
        }
        return false;
    }

    void push(int h, int r, int c) {
        HeapNode last;
        last.height = h; last.row = r; last.col = c;

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    HeapNode pop() {
        HeapNode ans = arr[0];
        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;
        while (idx * 2 + 1 < length) {
            left = idx * 2 + 1;
            right = idx * 2 + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                HeapNode temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }

        return ans;
    }
};

class Grid {
public:
    int subGrid[MAXK][MAXK];
    int height;
    Point top5[5];
    int length;

    void init() {
        height = 0;
        length = 0;
    }

    void push_loc(int r, int c) {
        top5[length].r = r;
        top5[length].c = c;
        length++;
    }
};

class Location {
public:
    int row, col, subRow, subCol;
};

int interval, totalSize;
Grid grid[MAXNK][MAXNK];
int versions[MAX_MAP_SIZE][MAX_MAP_SIZE];
Heap tops[MAXNK][MAXNK];

Location coor2loc(int row, int col) {
    Location ans;

    ans.row = row / interval;
    ans.col = col / interval;
    ans.subRow = row % interval;
    ans.subCol = col % interval;

    return ans;
}

void init(int N, int K, int mHeight[][MAX_MAP_SIZE])
{
    totalSize = N;
    interval = K;

    for (int i = 0; i < N / K; i++) {
        for (int j = 0; j < N / K; j++) {
            grid[i][j].init();
            tops[i][j].init();
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Location temp = coor2loc(i, j);
            grid[temp.row][temp.col].subGrid[temp.subRow][temp.subCol] = mHeight[i][j];
            tops[temp.row][temp.col].push(mHeight[i][j], i, j);
        }
    }

    for (int i = 0; i < N / K; i++) {
        for (int j = 0; j < N / K; j++) {
            while (grid[i][j].length < 5 && tops[i][j].length > 0) {
                HeapNode curr = tops[i][j].pop();
                grid[i][j].push_loc(curr.row, curr.col);
            }
        }
    }
}

void destroy()
{

}

void query(Point mA, Point mB, int mCount, Point mTop[])
{
    mA.r--; mA.c--;
    mB.r--; mB.c--;

    Location a = coor2loc(mA.r, mA.c);
    Location b = coor2loc(mB.r, mB.c);

    Heap pQueue;
    pQueue.init();

    for (int i = a.row; i <= b.row; i++) {
        for (int j = a.col; j <= b.col; j++) {

            for (int k = 0; k < grid[i][j].length; k++) {
                Location temp = coor2loc(grid[i][j].top5[k].r, grid[i][j].top5[k].c);
                int height = grid[i][j].height + grid[i][j].subGrid[temp.subRow][temp.subCol];
                pQueue.push(height, grid[i][j].top5[k].r, grid[i][j].top5[k].c);
            }

        }
    }

    int k = 0;
    while (pQueue.length > 0 && k < mCount) {
        HeapNode curr = pQueue.pop();

        mTop[k].r = curr.row + 1;
        mTop[k].c = curr.col + 1;
        k++;
    }
}

int getHeight(Point mP)
{
    mP.r--; mP.c--;

    Location loc = coor2loc(mP.r, mP.c);

    int ans = grid[loc.row][loc.col].subGrid[loc.subRow][loc.subCol] + grid[loc.row][loc.col].height;
    return ans;
}

void work(Point mA, Point mB, int mH)
{
    mA.r--; mA.c--;
    mB.r--; mB.c--;

    Location a = coor2loc(mA.r, mA.c);
    Location b = coor2loc(mB.r, mB.c);

    for (int i = a.row; i <= b.row; i++) {
        for (int j = a.col; j <= b.col; j++) {
            grid[i][j].height += mH;
        }
    }
}