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

int pSize;
vector< vector<Piece> > board;
vector< vector<bool> > onBoard;
unordered_map< int, vector<PieceLoc> > possiblePiece;

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

vector<int> checkNfit(vector<int> p) {
    vector<int> ans(3);
    ans[0] = 0;
    ans[1] = 1001;
    ans[2] = 1001;

    unordered_map <string, int> nMap;
    string key;
    vector< pair<int, int> > locations;
    for (int i=0; i < 4; i++) {
        if (!possiblePiece[p[i]].empty()) {
            for (PieceLoc loc: possiblePiece[p[i]]) {
                if (loc.dir == i) {
                    key = loc2key(loc.row, loc.col);
                    nMap[key]++; 
                }
            }
        }
    }

    pair<int, int> temp;
    for (auto loc: nMap) {
        temp = key2loc(loc.first);
        if (ans[0] < loc.second) {
            ans[0] = loc.second;
            ans[1] = temp.first;
            ans[2] = temp.second;
        }
        else if (ans[0] == loc.second) {
            if (temp.first < ans[1]) {
                ans[0] = loc.second;
                ans[1] = temp.first;
                ans[2] = temp.second;
            }
            else if( temp.first == ans[1] && temp.second < ans[2]) {
                ans[0] = loc.second;
                ans[1] = temp.first;
                ans[2] = temp.second;
            }
        }
    }
    // 0: 맞닿는 면 개수, 1: row, 2: col
    return ans;
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

    for (int i=0; i < 3; i++) {
        pq.push_back(checkNfit(p));
        push_heap(pq.begin(), pq.end());
        p = rotatePiece(p);
    }

    printf("%d %d %d %d\n", pq.front()[0], pq.front()[1], pq.front()[2]);

    return -1;
}
