#include <string>
#include <vector>
#include <unordered_map>

#define MAXSIZE 1000

using namespace std;

class Data {
public:
    int cnt;
    int row;
    int col;
    int dir;

    Data() {
        cnt = 0;
        row = -1;
        col = -1;
        dir = -1;
    }
    Data(int r, int c, int d) {
        cnt = 0;
        row = r;
        col = c;
        dir = d;
    }
};

class Heap {
public:
    Data arr[MAXSIZE];
    int length;

    void init() { length = 0; }
    bool compare(int parent, int child) {
        if (arr[parent].cnt < arr[child].cnt) { return true;  }
        if (arr[parent].cnt == arr[child].cnt && arr[parent].row > arr[child].row) { return true; }
        if (arr[parent].cnt == arr[child].cnt && arr[parent].row == arr[child].row && arr[parent].col > arr[child].col) { return true; }
        return false;
    }

    void push(int count, int r, int c, int d) {
        Data last = Data();
        last.cnt = count;  last.row = r; last.col = c; last.dir = d;
        arr[length] = last;

        int idx = length;
        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Data temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
        length++;
    }

    Data pop() {
        Data ans = arr[0];
        length--;
        arr[0] = arr[length];

        int idx = 0;
        int left, right, child;
        while (2 * idx + 1 < length) {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length) {
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
        return ans;
    }
};


int drows[4] = { -1, 0, 1, 0 };
int dcols[4] = { 0, 1, 0, -1 };

int* rotate(int mPiece[4], int nRotate) {
    static int ans[4];

    for (int i = 0; i < 4; i++) {
        ans[(i + nRotate) % 4] = mPiece[i];
    }

    return ans;
}

// 0: UP, 1: RIGHT, 2: DOWN, 3: LEFT
int pieceSize, boardSize;
int board[MAXSIZE][MAXSIZE][4];
bool onBoard[MAXSIZE][MAXSIZE];
unordered_map<int, vector<Data> > possiblePiece;

int getReverse(int p) {
    string s = to_string(p);
    string k;
    for (int i = s.size() - 1; i > -1; i--) {
        k += s[i];
    }
    return stoi(k);
}

int getCounterPiece(int p) {
    string param;
    for (int i = 0; i < pieceSize; i++) {
        param += '1';
    }
    param += '0';

    int ans = stoi(param);
    return ans - p;
}

int checkFit(int mPiece[4], int r, int c) {
    int cnt = 0;
    for (int i = 0; i < 4; i++) {
        if (board[r][c][i] > 0 && board[r][c][i] == mPiece[i]) {
            cnt++;
        }
        else if (board[r][c][i] > 0 && board[r][c][i] != mPiece[i]) {
            return 0;
        }
    }
    return cnt;
}

void init(int N, int M, int mU[][4], int mR[][4], int mB[][4], int mL[][4]) {
    boardSize = N;
    pieceSize = M;

    possiblePiece.clear();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            onBoard[i][j] = false;

            board[i][j][0] = 0;
            board[i][j][1] = 0;
            board[i][j][2] = 0;
            board[i][j][3] = 0;
            if (i == 0) {
                board[i][j][0] = getCounterPiece(getReverse(mU[j][2]));
                Data temp = Data(i, j, 0);
                possiblePiece[board[i][j][0]].push_back(temp);
            }
            else if (i == (boardSize - 1)) {
                board[i][j][2] = getReverse(getCounterPiece(mB[j][0]));
                Data temp = Data(i, j, 2);
                possiblePiece[board[i][j][2]].push_back(temp);
            }
            if (j == 0) {
                board[i][j][3] = getReverse(getCounterPiece(mL[i][1]));
                Data temp = Data(i, j, 3);
                possiblePiece[board[i][j][3]].push_back(temp);
            }
            else if (j == (boardSize - 1)) {
                board[i][j][1] = getCounterPiece(getReverse(mR[i][3]));
                Data temp = Data(i, j, 1);
                possiblePiece[board[i][j][1]].push_back(temp);
            }
        }
    }
}
void destroy() {

}

void placePiece(int r, int c, int mPiece[4]) {
    onBoard[r][c] = true;
    int dr, dc;
    for (int i = 0; i < 4; i++) {
        board[r][c][i] = mPiece[i];

        dr = r + drows[i];
        dc = c + dcols[i];
        if (0 <= dr && dr < boardSize && 0 <= dc && dc < boardSize) {
            if (i == 0) {
                board[dr][dc][2] = getReverse(getCounterPiece(mPiece[i]));
                Data temp = Data(dr, dc, 2);
                possiblePiece[board[dr][dc][2]].push_back(temp);
            }
            else if (i == 1) {
                board[dr][dc][3] = getReverse(getCounterPiece(mPiece[i]));
                Data temp = Data(dr, dc, 3);
                possiblePiece[board[dr][dc][3]].push_back(temp);
            }
            else if (i == 2) {
                board[dr][dc][0] = getCounterPiece(getReverse(mPiece[i]));
                Data temp = Data(dr, dc, 0);
                possiblePiece[board[dr][dc][0]].push_back(temp);
            }
            else if (i == 3) {
                board[dr][dc][1] = getCounterPiece(getReverse(mPiece[i]));
                Data temp = Data(dr, dc, 1);
                possiblePiece[board[dr][dc][1]].push_back(temp);
            }
        }
    }
}

int put(int mPiece[4]) {
    unordered_map<int, vector<Data> >::iterator itr;
    Heap pQueue;
    pQueue.init();

    for (int i = 0; i < 4; i++) {
        itr = possiblePiece.find(mPiece[i]);
        if (itr == possiblePiece.end()) {
            continue;
        }

        for (Data loc : possiblePiece[mPiece[i]]) {
            int nRotate = loc.dir - i;
            if (nRotate < 0) { nRotate += 4; }

            int* piece = rotate(mPiece, nRotate);
            int k = checkFit(piece, loc.row, loc.col);
            if (k > 0 && !onBoard[loc.row][loc.col]) {
                pQueue.push(k, loc.row, loc.col, nRotate);
            }
        }
    }

    if (pQueue.length > 0) {
        int* piece = rotate(mPiece, pQueue.arr[0].dir);
        placePiece(pQueue.arr[0].row, pQueue.arr[0].col, piece);

        return pQueue.arr[0].row + pQueue.arr[0].col + 2;
    }

    return -1;
}
