#define MAX_N 100

class Data {
public:
    int energy;
    int row;
    int col;

    Data() {}
    Data(int e, int r, int c) {
        energy = e;
        row = r;
        col = c;
    }
};

class Heap {
public:
    Data arr[MAX_N * MAX_N];
    int length;

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].energy < arr[child].energy) { return true; }
        if (arr[parent].energy == arr[child].energy && arr[parent].row > arr[child].row) { return true; }
        if (arr[parent].energy == arr[child].energy && arr[parent].row == arr[child].row && 
                arr[parent].col > arr[child].col) { return true; }
        return false;
    }

    void push(Data last) {
        arr[length] = last;
        int idx = length;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Data temp = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = arr[idx];
            arr[idx] = temp;
            idx = (idx - 1) / 2;
        }
        length++;
    }

    Data pop() {
        Data res = arr[0];
        length--;
        arr[0] = arr[length];

        int left, right, child;
        int idx = 0;
        while (2 * idx + 1 < length) {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (2 * idx + 2 < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                Data temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }
        return res;
    }
};

int drows[4] = { -1, 1, 0, 0 };
int dcols[4] = { 0, 0, -1, 1 };

Heap pQueue;
int totalN;
int grid[MAX_N + 1][MAX_N + 1];
int bacterias[MAX_N + 1][MAX_N + 1];
int nBacterias[3];
bool visited[MAX_N + 1][MAX_N + 1];


void initVisited() {
    for (int i = 0; i < totalN; i++) {
        for (int j = 0; j < totalN; j++) {
            visited[i][j] = false;
        }
    }
}

void dfsDrop(int mTarget, int mRow, int mCol) {
    if (bacterias[mRow][mCol] != mTarget || visited[mRow][mCol]) {
        return;
    }

    visited[mRow][mCol] = true;

    int dr, dc;
    for (int i = 0; i < 4; i++) {
        dr = mRow + drows[i];
        dc = mCol + dcols[i];

        if (0 < dr && dr < totalN && 0 < dc && dc < totalN) {
            if (bacterias[dr][dc] == 0 && !visited[dr][dc]) {
                Data d = Data(grid[dr][dc], dr, dc);
                pQueue.push(d);
            }
            else if (bacterias[dr][dc] == mTarget && !visited[dr][dc]) {
                dfsDrop(mTarget, dr, dc);
            }
        }
    }
}

void bfsDrop(int mTarget, int mRow, int mCol, int mEnergy) {
    if (bacterias[mRow][mCol] == mTarget) {
        dfsDrop(mTarget, mRow, mCol);
    }
    else if (bacterias[mRow][mCol] == 0) {
        Data d = Data(grid[mRow][mCol], mRow, mCol);
        pQueue.push(d);
        visited[mRow][mCol] = true;
    }

    while (pQueue.length > 0 && mEnergy > 0) {
        Data curr = pQueue.pop();

        if (bacterias[curr.row][curr.col] == 0) {
            bacterias[curr.row][curr.col] = mTarget;
            mEnergy -= grid[curr.row][curr.col];
            nBacterias[mTarget]++;
        }

        int dr, dc;
        for (int i = 0; i < 4; i++) {
            dr = curr.row + drows[i];
            dc = curr.col + dcols[i];

            if (0 < dr && dr < totalN && 0 < dc && dc < totalN) {
                if (bacterias[dr][dc] == mTarget && !visited[dr][dc]) {
                    dfsDrop(mTarget, dr, dc);
                    visited[dr][dc] = true;
                }
                else if (bacterias[dr][dc] == 0 && !visited[dr][dc]) {
                    Data d = Data(grid[dr][dc], dr, dc);
                    pQueue.push(d);
                    visited[dr][dc] = true;
                }
            }
        }
    }
}

void dfsClean(int mTarget, int mRow, int mCol) {
    if (bacterias[mRow][mCol] != mTarget) {
        return;
    }

    if (bacterias[mRow][mCol] == mTarget) {
        bacterias[mRow][mCol] = 0;
        nBacterias[mTarget]--;
    }
    
    int dr, dc;
    for (int i = 0; i < 4; i++) {
        dr = mRow + drows[i];
        dc = mCol + dcols[i];

        if (0 < dr && dr < totalN && 0 < dc && dc < totalN) {
            if (bacterias[dr][dc] == mTarget) {
                dfsClean(mTarget, dr, dc);
            }
        }
    }
}


void init(int N, int mDish[MAX_N][MAX_N])
{
    for (int i = 0; i < 3; i++) {
        nBacterias[i] = 0;
    }

    totalN = N + 1;
    for (int i = 1; i < totalN; i++) {
        for (int j = 1; j < totalN; j++) {
            grid[i][j] = mDish[i-1][j-1];
            bacterias[i][j] = 0;
        }
    }
}

int dropMedicine(int mTarget, int mRow, int mCol, int mEnergy)
{
    pQueue.init();
    initVisited();

    bfsDrop(mTarget, mRow, mCol, mEnergy);
    return nBacterias[mTarget];
}

int cleanBacteria(int mRow, int mCol)
{
    if (bacterias[mRow][mCol] == 0) {
        return -1;
    }

    int target = bacterias[mRow][mCol];
    dfsClean(target, mRow, mCol);
    return nBacterias[target];
}