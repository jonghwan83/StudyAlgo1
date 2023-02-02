#include <unordered_map>
#include <deque>

#define MAXGRID 200
#define MAXBAR 300
#define MAXREGION 15000

using namespace std;

int drows[4] = { -1, 1, 0, 0 };
int dcols[4] = { 0, 0, -1, 1 };

class Bar {
public:
    bool isRemoved;
    int id;
    int length;
    int row;
    int col;
    int dir;
};

class Region {
public:
    bool isExist;
    int length;

    Region() {
        length = 0;
        isExist = false;
    }
};

unordered_map<int, int> hashBar;
int nGrid, barIdx, checkIdx, regionIdx, regionCnt;
int checkMap[MAXGRID][MAXGRID];
int regionMap[MAXGRID][MAXGRID];
int grid[MAXGRID][MAXGRID];

Bar bars[MAXBAR];
deque< pair<int, int> > queue;
Region regions[MAXREGION];

bool incr;

void init(int N) {
    hashBar.clear();
    barIdx = 0;
    nGrid = N;
    checkIdx = 1;
    regionIdx = 0;
    regions[regionIdx].length = 0;
    regionCnt = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = 0;
            checkMap[i][j] = 0;
            regionMap[i][j] = 0;
        }
    }
}

void paintRegion(int idx, bool isRemoving) {
    for (int i = 0; i < bars[idx].length; i++) {
        int row = bars[idx].row + i * drows[bars[idx].dir];
        int col = bars[idx].col + i * dcols[bars[idx].dir];

        if (isRemoving) {
            grid[row][col]--;
            if (grid[row][col] == 0 && checkMap[row][col] <= checkIdx) {
                queue.push_back(make_pair(row, col));
                checkMap[row][col] = checkIdx;
            }
            continue;
        }

        int dr, dc;
        for (int k = 0; k < 4; k++) {
            dr = row + drows[k];
            dc = col + dcols[k];

            if (0 <= dr && dr < nGrid && 0 <= dc && dc < nGrid) {
                if (grid[dr][dc] == 0 && checkMap[dr][dc] <= checkIdx) {
                    queue.push_back(make_pair(dr, dc));
                    checkMap[dr][dc] = checkIdx;
                    if (regions[regionMap[dr][dc]].isExist) {
                        regions[regionMap[dr][dc]].isExist = false;
                        regionCnt--;
                    }
                }
            }
        }
    }
}

void dfs(int row, int col, bool isRemoving) {
    if (checkMap[row][col] == checkIdx) {
        incr = false;
        return;
    }

    if (isRemoving) {
        if (regionMap[row][col] != regionIdx && regions[regionMap[row][col]].isExist) {
            regions[regionMap[row][col]].isExist = false;
            regionCnt--;
        }
    }
    checkMap[row][col] = checkIdx;
    regionMap[row][col] = regionIdx;
    regions[regionIdx].length++;
    
    int dr, dc;
    for (int i = 0; i < 4; i++) {
        dr = row + drows[i];
        dc = col + dcols[i];

        if (0 <= dr && dr < nGrid && 0 <= dc && dc < nGrid) {
            if (grid[dr][dc] == 0 && checkMap[dr][dc] < checkIdx) {
                dfs(dr, dc, isRemoving);
            }
        }
    }
    incr = true;
}

int addBar(int mID, int mLength, int mRow, int mCol, int mDir) {
    mRow--;
    mCol--;

    queue.clear();

    hashBar[mID] = barIdx;
    bars[barIdx].id = mID; bars[barIdx].isRemoved = false;
    bars[barIdx].row = mRow; bars[barIdx].col = mCol;
    bars[barIdx].dir = mDir; bars[barIdx].length = mLength;

    for (int i = 0; i < mLength; i++) {
        int dr, dc;
        dr = mRow + drows[mDir] * i;
        dc = mCol + dcols[mDir] * i;

        if (regions[regionMap[dr][dc]].isExist) {
            regions[regionMap[dr][dc]].length--;
            if (regions[regionMap[dr][dc]].length < 1) {
                regions[regionMap[dr][dc]].isExist = false;
                regionCnt--;
            }
        }
        grid[dr][dc]++;
    }

    paintRegion(barIdx, false);
    checkIdx++;

    incr = false;
    while (!queue.empty()) {
        dfs(queue.front().first, queue.front().second, false);
        queue.pop_front();
        if (incr) { 
            regions[regionIdx++].isExist = true;
            regions[regionIdx].length = 0;
            regionCnt++;
        }
    }

    barIdx++;
    return regionCnt;
}

int removeBar(int mID) {
    queue.clear();

    int idx = hashBar[mID];

    paintRegion(idx, true);
    checkIdx++;

    incr = false;
    while (!queue.empty()) {
        dfs(queue.front().first, queue.front().second, true);
        queue.pop_front();
        if (incr) {
            regions[regionIdx++].isExist = true;
            regions[regionIdx].length = 0;
            regionCnt++;
        }
    }

    return regionCnt;
}