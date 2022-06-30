#include <iostream>

using namespace std;

int square[9][9];
int P[81][2];
int np;
bool isSolved;

bool verify(int k, int idx) {
    bool H = true;
    for (int i=0; i < 9; i++) {
        if (square[P[k][0]][i] == idx) {
            H = false;
            return H;
        }
    }

    bool V = true;
    for (int i=0; i < 9; i++) {
        if (square[i][P[k][1]] == idx) {
            V = false;
            return V;
        }
    }

    bool S = true;
    int r = 3 * (P[k][0] / 3);
    int c = 3 * (P[k][1] / 3);
    for (int i=0; i < 3; i++) {
        for (int j=0; j < 3; j++) {
            if (square[r+i][c+j] == idx) {
                S = false;
                return S;
            }
        }
    }

    bool result = H && V && S;
    return result;
}

void solve(int k) {
    if (k >= np) {
        for (int i=0; i < 9; i++) {
            for (int j=0; j < 9; j++) {
                cout << square[i][j] << " ";
            }
            cout << "\n";
        }
        isSolved = true;
    }

    for (int i=1; i < 10; i++) {
        if (isSolved) {
            return;
        }
        if (verify(k, i)) {
            square[P[k][0]][P[k][1]] = i;
            solve(k+1);
            square[P[k][0]][P[k][1]] = 0;
        }
    }
}

int main() {
    // freopen("sample.txt", "r", stdin);
    np = 0;
    isSolved = false;

    for (int i=0; i < 9; i++) {
        for (int j=0; j < 9; j++) {
            cin >> square[i][j];
            if (square[i][j] == 0) {
                P[np][0] = i;
                P[np][1] = j;
                np++;
            }
        }
    }

    solve(0);

    return 0;
}