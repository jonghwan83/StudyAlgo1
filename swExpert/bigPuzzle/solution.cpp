#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Piece {
public:
    int up;
    int right;
    int down;
    int left;
    Piece() {
        up = 0;
        right = 0;
        down = 0;
        left = 0;
    }
};

class PieceLoc {
public:
    int row;
    int col;
    int dir;
    PieceLoc () {
        row = -1;
        col = -1;
        dir = -1;
    }
};

int pSize, boardSize;
vector< vector<Piece> > board;
vector< vector<bool> > onBoard;
unordered_map< int, vector<PieceLoc> > possiblePiece;

vector<int> drows = { -1, 0, 1, 0 };
vector<int> dcols = { 0, 1, 0, -1 };

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
    for (int i=0; i < pSize; i++) {
        param += '1';
    }
    param += '0';

    int iparam = stoi(param);
    return iparam - p;
}

void addPossible(int key, int r, int c, int d) {
    PieceLoc p;
    possiblePiece[key].push_back(p);
    int lastElement = possiblePiece[key].size() - 1;
    possiblePiece[key][lastElement].row = r;
    possiblePiece[key][lastElement].col = c;
    possiblePiece[key][lastElement].dir = d;
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

string loc2key(int r, int c) {
    string ans;
    ans = to_string(r) + ',' + to_string(c);
    return ans;
}

pair<int, int> key2loc(string key) {
    pair<string, string> temp;
    bool secondFlag = false;
    for (int i=0; i < key.size(); i++) {
        if (key[i] == ',') { secondFlag = true; continue; }
        if (secondFlag) {
            temp.second += key[i];
        }
        else {
            temp.first += key[i];
        }
    }
    return make_pair(stoi(temp.first), stoi(temp.second));
}

bool checkShape(vector<int> p1, Piece p2) {
    for (int i=0; i < 4; i++) {
        if (i == 0 && p2.up != 0 && p2.up != p1[i]) {
        return false;
        }
        else if (i == 1 && p2.right != 0 && p2.right != p1[i]) {
            return false;
        }
        else if (i == 2 && p2.down != 0 && p2.down != p1[i]) {
            return false;
        }
        else if (i == 3 && p2.left != 0 && p2.left != p1[i]) {
            return false;
        }
    }
    return true;
}

vector<int> checkNfit(vector<int> p) {
    unordered_map<string, int> nMap;
    for (int i=0; i < 4; i++) {
        if (!possiblePiece[p[i]].empty()) {
            for (auto pc: possiblePiece[p[i]]) {
                string key = loc2key(pc.row, pc.col);
                if (checkShape(p, board[pc.row][pc.col]) && !onBoard[pc.row][pc.col]) {
                    if (i == pc.dir) { nMap[key]++; }
                }
            }
        }
    }

    vector< vector<int> > pq;
    pair<int, int> temp;
    for (auto element: nMap) {
        temp = key2loc(element.first);
        pq.push_back({element.second, -temp.first, -temp.second});
        push_heap(pq.begin(), pq.end());
    }
    
    vector<int> ans(3);
    ans[0] = -1;
    if (!pq.empty()) {
        ans = pq.front();
    }
    return ans;
}

void placePiece(int r, int c, int nRotate, int mPiece[4]) {
    vector<int> p(4);
    for (int i=0; i < 4; i++) {
        p[(i + nRotate) % 4] = mPiece[i];
    }

    onBoard[r][c] = true;
    int dr, dc;
    for (int i=0; i < 4; i++) {
        dr = r + drows[i];
        dc = c + dcols[i];

        if (0 <= dr && dr < boardSize && 0 <= dc && dc < boardSize && !onBoard[dr][dc]) {
            if (i == 0) {
            board[dr][dc].down = getReverse(getCounterPiece(p[i]));
            addPossible(board[dr][dc].down, dr, dc, 2);
            }
            else if (i == 1) {
                board[dr][dc].left = getReverse(getCounterPiece(p[i]));
                addPossible(board[dr][dc].left, dr, dc, 3);
            }
            else if (i == 2) {
                board[dr][dc].up = getCounterPiece(getReverse(p[i]));
                addPossible(board[dr][dc].up, dr, dc, 0);
            }
            else {
                board[dr][dc].right = getCounterPiece(getReverse(p[i]));
                addPossible(board[dr][dc].right, dr, dc, 1);
            }
        }
    }
}

void printBoard(int k) {
    for (int i=0; i < k; i++) {
        for (Piece p: board[i]) {
            printf("%d/%d/%d/%d ", p.up, p.right, p.down, p.left);
        }
        printf("\n");
    }
}

// board에 가능한 퍼즐 조각의 정보를 넣는다.
// 퍼즐 정보가 들어오면 이것으로 위치를 빠르게 찾을 수 있는 좌표 위치를 해시테이블로 만든다.
void init(int N, int M, int mU[][4], int mR[][4], int mB[][4], int mL[][4]) {
    board.clear();
    possiblePiece.clear();
    onBoard.clear();

    pSize = M;
    boardSize = N;

    board.resize(N);
    onBoard.resize(N);
    for (int i=0; i < N; i++) {
        board[i].resize(N);
        onBoard[i].resize(N);
    }

    for (int i=0; i < N; i++) {
        board[0][i].up = getCounterPiece(getReverse(mU[i][2]));
        addPossible(board[0][i].up, 0, i, 0);
        board[i][N - 1].right = getCounterPiece(getReverse(mR[i][3]));
        addPossible(board[i][N - 1].right, i, N - 1, 1);
        board[N - 1][i].down = getReverse(getCounterPiece(mB[i][0]));
        addPossible(board[N - 1][i].down, N - 1, i, 2);
        board[i][0].left = getReverse(getCounterPiece(mL[i][1]));
        addPossible(board[i][0].left, i, 0, 3);
    }
}

void destroy() {

}

int put(int mPiece[4]) {
    vector<int> p(4);
    for (int i=0; i < 4; i++) {
        p[i] = mPiece[i];
    }

    vector< vector<int> > pq;
    vector<int> temp;
    for (int i=0; i < 4; i++) {
        temp = checkNfit(p);
        temp.push_back(i);

        pq.push_back(temp);
        push_heap(pq.begin(), pq.end());

        p = rotatePiece(p);
    }

    if (pq.front()[0] > 0) {
        placePiece(-pq.front()[1], -pq.front()[2], pq.front()[3], mPiece);
        return -(pq.front()[1] + pq.front()[2]) + 2;
    }
    return -1;
}
