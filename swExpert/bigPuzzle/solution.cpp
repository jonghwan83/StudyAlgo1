#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class Piece {
public:
    int up;
    int right;
    int down;
    int left;
};

vector< vector<Piece> > board;
unordered_map<int, int> possiblePiece;

int getReverse(int p) {
    string s = to_string(p);
    string k;
    for (int i=s.size() - 1; i > -1; i--) {
        k += s[i];
    }
    printf("%s\n", k.c_str());
    return stoi(k);
}


void init(int N, int M, int mU[][4], int mR[][4], int mB[][4], int mL[][4]) {
    board.clear();
    possiblePiece.clear();

    board.resize(N + 1);
    for (int i=0; i <= N; i++) {
        board[i].resize(N + 1);
    }

    for (int i=0; i < N; i++) {
        for (int j=0; j < 4; j++) {
            
        }
    }
}

void destroy() {

}

int put(int mPiece[4]) {
    return -1;
}

