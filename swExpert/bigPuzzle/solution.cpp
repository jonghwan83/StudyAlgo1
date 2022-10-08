// max board: 1000 * 1000
// dfs: 1,000,000 * 500,000 = 5,000,000,000,000 -> about 5000s
// 매번 퍼즐 조각을 grid에서 찾아가면 시간 부족

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

class locInfo{
public:
    int row;
    int column;
    int dir;

    locInfo() {
        row = -1;
        column = -1;
        dir = -1;
    }
    // direction 0, 1, 2, 3 -> upper, right, botton, left
    void getlocation(int r, int c, int d) {
        row = r;
        column = c;
        dir = d;
    }
};

class Piece{
public:
    int upper;
    int bottom;
    int left;
    int right;

    Piece() {
        upper = 0;
        bottom = 0;
        left = 0;
        right = 0;
    }
};

int nBoard;
int nPiece;
vector< vector<Piece> > grid;
vector< vector<bool> > isPiece;

// puzzle piece - location
unordered_map<int, locInfo > possilbeMap;  // key: possible Number, value: row, column, direction

vector<int> drows = { -1, 0, 1, 0 };
vector<int> dcols = { 0, 1, 0, -1 };

// left, lower side: reverse number
int reverseNum(int piece) {
    string k = to_string(piece);
    reverse(k.begin(), k.end());
    int result = stoi(k);
    return result;
}

int getOppsite(int piece) {
    string k = to_string(piece);
    string opposite = "";
    int a;
    for (int i=0; i < k.size(); i++) {
        a = (int)k[i] - 48;
        opposite += (char)(10 - a + 48);
    }
    return stoi(opposite);
}

void updateGrid(int r, int c, int mPiece[4]) {
    // (r, c) 에 조각을 놓았을 경우, 놓은 위치의 상하좌우의 grid를 업데이트 한다.
    // 방향 판단 필요함
    // 맞닿는 면의 개수를 세어야 함
    isPiece[r][c] = true;

    int dr, dc;
    for (int i=0; i < 4; i++) {
        dr = r + drows[i];
        dc = c + dcols[i];

        if (0 <= dr && dr < nBoard && 0 <= dc && dc < nBoard) {
            if (i == 0) {
                grid[dr][dc].bottom = reverseNum(getOppsite(mPiece[0]));
                possilbeMap[reverseNum(getOppsite(mPiece[0]))].getlocation(dr, dc, 2);
            }
            else if (i == 1) {
                grid[dr][dc].left = reverseNum(getOppsite(mPiece[1]));
                possilbeMap[reverseNum(getOppsite(mPiece[1]))].getlocation(dr, dc, 3);
            }
            else if (i == 2) {
                grid[dr][dc].upper = getOppsite(reverseNum(mPiece[2]));
                possilbeMap[getOppsite(reverseNum(mPiece[2]))].getlocation(dr, dc, 0);
            }
            else {
                grid[dr][dc].right = getOppsite(reverseNum(mPiece[3]));
                possilbeMap[getOppsite(reverseNum(mPiece[3]))].getlocation(dr, dc, 1);
            }
        }
    }
    return;
}

void init(int N, int M, int mU[][4], int mR[][4], int mB[][4], int mL[][4]) {
    nBoard = N;
    nPiece = M;

    grid.resize(N);
    isPiece.resize(N);
    for (int i=0; i < N; i++) {
        grid[i].resize(N);
        isPiece[i].resize(N);
    }

    // possible numbers in grid
    for (int i=0; i < N; i++) {
        grid[0][i].upper = getOppsite(reverseNum(mU[i][2]));
        possilbeMap[getOppsite(reverseNum(mU[i][2]))].getlocation(0, i, 0);

        grid[N-1][i].bottom = reverseNum(getOppsite(mB[i][0]));
        possilbeMap[reverseNum(getOppsite(mB[i][0]))].getlocation(N-1, i, 2);

        grid[i][0].left = reverseNum(getOppsite(mL[i][1]));
        possilbeMap[reverseNum(getOppsite(mL[i][1]))].getlocation(i, 0, 3);

        grid[i][N-1].right = getOppsite(reverseNum(mR[i][3]));
        possilbeMap[getOppsite(reverseNum(mR[i][3]))].getlocation(i, N-1, 1);
    }
}

void destroy() {

}

int put(int mPiece[4]) {
    // 각각의 r, c를 저장한 후, 가장 왼쪽 상단에 있는 것을 선택 한다.
    // 여기서 맞닿는 면의 개수도 같이 저장 해야 함

    printf("-------\n");
    for (int i=0; i < 4; i++) {
        if (possilbeMap[mPiece[i]].dir != -1) {
            if (!isPiece[possilbeMap[mPiece[i]].row][possilbeMap[mPiece[i]].column]) {
                printf("%d %d %d %d\n", mPiece[i], possilbeMap[mPiece[i]].row, possilbeMap[mPiece[i]].column,
                       possilbeMap[mPiece[i]].dir);
                updateGrid(possilbeMap[mPiece[i]].row, possilbeMap[mPiece[i]].column, mPiece);
            }
        }
    }

    return -1;
}
