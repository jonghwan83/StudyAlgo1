#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

#define MAXBOARD 1001

using namespace std;

int boardSize;
int pieceSize;

int getReverse(int p) {
    string s = to_string(p);
    string k;
    for (int i=s.size() - 1; i > -1; i--) {
        k += s[i];
    }
    return stoi(k);
}

int getCounterPiece(int p) {
    string param;
    for (int i=0; i < pieceSize; i++) {
        param += '1';
    }
    param += '0';

    int ans = stoi(param);
    return ans - p;
}

// 0: up, 1: right, 2: down, 3: left
unordered_map<int, vector< vector<int> > > possiblePiece;
vector< vector< vector<int> > > board(MAXBOARD, vector< vector<int> >(MAXBOARD, vector<int> (4)));
vector< vector<bool> > onBoard(MAXBOARD, vector<bool> (MAXBOARD));

void printBoard() {
    for (int i=0; i < boardSize; i++) {
        for (int j=0; j < boardSize; j++) {
            printf("%d/%d/%d/%d ", board[i][j][0], board[i][j][1], board[i][j][2], board[i][j][3]);
        }
        printf("\n");
    }
}
void printHash() {
    for (auto boardInfo: possiblePiece) {
        for (auto info: boardInfo.second) {
            printf("%d: %d %d\n", boardInfo.first, info[0], info[1]);
        }
    }
}

vector<int> rotatePiece(vector<int> p) {
    vector<int> ans(4);
    int temp;
    temp = p[3];
    for (int i=1; i < 4; i++) {
        ans[i] = p[i - 1];
    }
    ans[0] = temp;
    return ans;
}

int checkShape(int r, int c, vector<int> piece) {
    int k = 0;
    for (int i=0; i < 4; i++) {
        if (board[r][c][i] == 0) {
            continue;
        }
        else if (board[r][c][i] == piece[i]) {
            k++;
            continue;
        }
        if (piece[i] != board[r][c][i]) { return 0; }
    }
    return k;
}

vector<int> getNfit(vector<int> piece) {
    vector< vector<int> > pq;
    for (int i=0; i < 4; i++) {
        for (auto boardInfo: possiblePiece[piece[i]]) {
            if (boardInfo[2] == i && !onBoard[boardInfo[0]][boardInfo[1]]) {
                int k = checkShape(boardInfo[0], boardInfo[1], piece);
                pq.push_back({ k, -boardInfo[0], -boardInfo[1] } );
                push_heap(pq.begin(), pq.end());
            }
        }
    }

    if (!pq.empty()) {
        return { pq.front()[0], pq.front()[1], pq.front()[2] };
    }
    return { 0, 1002, 1002 };
}

void placePiece(int r, int c, int nRotate, int mPiece[4]) {
    vector<int> piece(4);
    for (int i=0; i < 4; i++) {
        piece[(i + nRotate) % 4] = mPiece[i];
    }

    onBoard[r][c] = true;
    vector<int> drows = { -1, 0, 1, 0 };
    vector<int> dcols = { 0, 1, 0, -1 };

    int dr, dc;
    for (int i=0; i < 4; i++) {
        dr = r + drows[i];
        dc = c + dcols[i];
        if (0 <= dr && dr < boardSize && 0 <= dc && dc < boardSize) {
            if (i == 0){
                board[dr][dc][2] = getReverse(getCounterPiece(piece[i]));
                possiblePiece[board[dr][dc][2]].push_back( {dr, dc, 2} );
            }
            else if (i == 1) {
                board[dr][dc][3] = getReverse(getCounterPiece(piece[i]));
                possiblePiece[board[dr][dc][3]].push_back( {dr, dc, 3} );
            }
            else if (i == 2) {
                board[dr][dc][0] = getCounterPiece(getReverse(piece[i]));
                possiblePiece[board[dr][dc][0]].push_back( {dr, dc, 0} );
            }
            else if (i == 3) {
                board[dr][dc][1] = getCounterPiece(getReverse(piece[i]));
                possiblePiece[board[dr][dc][1]].push_back( {dr, dc, 1} );
            }
        }
    }
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
                possiblePiece[board[i][j][0]].push_back({i, j, 0});
            }
            else if(i == (boardSize - 1)) {
                board[i][j][2] = getReverse(getCounterPiece(mB[j][0]));
                possiblePiece[board[i][j][2]].push_back({i, j, 2});
            }
            if(j == 0) {
                board[i][j][3] = getReverse(getCounterPiece(mL[i][1]));
                possiblePiece[board[i][j][3]].push_back({i, j, 3});
            }
            else if (j == (boardSize - 1)) {
                board[i][j][1] = getCounterPiece(getReverse(mR[i][3]));
                possiblePiece[board[i][j][1]].push_back({i, j, 1});
            }
        }
    }
}

void destroy() {

}

int put(int mPiece[4]) {
    vector<int> piece(mPiece, mPiece + 4);
    vector< vector<int> > pq;

    vector<int> temp;
    for (int i=0; i < 4; i++) {
        temp = getNfit(piece);
        temp.push_back(i);

        pq.push_back(temp);
        push_heap(pq.begin(), pq.end());
        piece = rotatePiece(piece);
    }

    if (pq.front()[0] > 0) {
        placePiece(-pq.front()[1], -pq.front()[2], pq.front()[3], mPiece);
        return -(pq.front()[1] + pq.front()[2]) + 2;
    }

    return -1;
}
