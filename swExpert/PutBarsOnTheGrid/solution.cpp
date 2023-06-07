#define MAXID 1000000
#define MAXN 200
#define MAXSTICK 300
#define MAXREGION 15000

int drows[4] = { -1, 1, 0, 0 };
int dcols[4] = { 0, 0, -1, 1 };

class Node {
public:
    int row;
    int col;
};

class Queue {
public:
    Node arr[MAXN * MAXN];
    int length, st, ed;

    void init() {
        length = 0; st = 0; ed = 0;
    }

    void push(int r, int c) {
        length++;
        arr[ed].row = r; arr[ed].col = c;
        ed++;
    }

    Node pop() {
        length--;
        return arr[st++];
    }
};


class Stick {
public:
    int id;
    int row;
    int col;
    int dir;
    int length;
};

class Region {
public:
    int length;
    bool isExist;

    void init() {
        length = 0;
        isExist = true;
    }
};


int boardSize;
int board[MAXN][MAXN];
int regionMap[MAXN][MAXN];
int bfsVisited[MAXN][MAXN];

Region regions[MAXREGION];
int regionIdx;
int nBfs;

int sIdx;
Stick sticks[MAXSTICK];
int hashStick[MAXID];

int regionCnt;
bool isIncr;

Queue queue;
Queue q;


void init(int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = 0;
            bfsVisited[i][j] = 0;
            regionMap[i][j] = 0;
        }
    }

    boardSize = N;
    sIdx = 0;
    regionIdx = 1;
    nBfs = 0;
    regionCnt = 0;
}

void paintRegion(int idx, bool isRemoving) {
    for (int i = 0; i < sticks[idx].length; i++) {
        int r = sticks[idx].row + drows[sticks[idx].dir] * i;
        int c = sticks[idx].col + dcols[sticks[idx].dir] * i;

        
        if (isRemoving) {
            board[r][c]--;
            if (board[r][c] == 0 && bfsVisited[r][c] < nBfs) {
                queue.push(r, c);
                bfsVisited[r][c] = nBfs;
            }
            continue;
        }
        
        for (int k = 0; k < 4; k++) {
            int dr = r + drows[k];
            int dc = c + dcols[k];

            if (0 <= dr && dr < boardSize && 0 <= dc && dc < boardSize) {
                if (board[dr][dc] == 0 && bfsVisited[dr][dc] < nBfs) {
                    queue.push(dr, dc);
                    bfsVisited[dr][dc] = nBfs;

                    if (regions[regionMap[dr][dc]].isExist) {
                        regions[regionMap[dr][dc]].isExist = false;
                        regionCnt--;
                    }
                }
            }

        }
    }
}


void bfs(int r, int c, bool isRemoving) {
    q.init();

    if (bfsVisited[r][c] < nBfs) {
        q.push(r, c);
        bfsVisited[r][c] = nBfs;

        regions[regionIdx].init();
    }

    else { return; }

    while (q.length > 0) {
        Node curr = q.pop();

        if (isRemoving) {
            if (regions[regionMap[curr.row][curr.col]].isExist && regionMap[curr.row][curr.col] != regionIdx) {
                regions[regionMap[curr.row][curr.col]].isExist = false;
                regionCnt--;
            }
        }

        regionMap[curr.row][curr.col] = regionIdx;
        regions[regionIdx].length++;

        for (int i = 0; i < 4; i++) {
            int dr = curr.row + drows[i];
            int dc = curr.col + dcols[i];

            if (0 <= dr && dr < boardSize && 0 <= dc && dc < boardSize) {
                if (board[dr][dc] == 0 && bfsVisited[dr][dc] < nBfs) {
                    q.push(dr, dc);
                    bfsVisited[dr][dc] = nBfs;
                }
            }
        }
    }
    
    regionIdx++;
    regionCnt++;
}


int addBar(int mID, int mLength, int mRow, int mCol, int mDir)
{
    mRow--; mCol--;

    hashStick[mID] = sIdx;

    sticks[sIdx].id = mID;
    sticks[sIdx].row = mRow;
    sticks[sIdx].col = mCol;
    sticks[sIdx].length = mLength;
    sticks[sIdx].dir = mDir;

    for (int i = 0; i < mLength; i++) {
        int r = mRow + drows[mDir] * i;
        int c = mCol + dcols[mDir] * i;

        board[r][c]++;

        if (regions[regionMap[r][c]].isExist) {
            regions[regionMap[r][c]].length--;
            if (regions[regionMap[r][c]].length == 0) {
                regions[regionMap[r][c]].isExist = false;
                regionCnt--;
            }
        }
    }

    queue.init();

    nBfs++;
    paintRegion(sIdx, false);
    nBfs++;

    while (queue.length > 0) {
        Node curr = queue.pop();

        bfs(curr.row, curr.col, false);
    }
                                              
    sIdx++;
    
    return regionCnt;
}

int removeBar(int mID)
{

    int idx = hashStick[mID];

    queue.init();

    paintRegion(idx, true);
    nBfs++;

    while (queue.length > 0) {
        Node curr = queue.pop();

        bfs(curr.row, curr.col, true);
    }

    return regionCnt;
}