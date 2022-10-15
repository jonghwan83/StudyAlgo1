#include <vector>
#include <string>
#include <deque>
#include <iostream>

using namespace std;


int n, m;
vector<string> board;
vector<int> drows = { -1, 1, 0, 0 };
vector<int> dcols = { 0, 0, -1, 1 };

void bfs(pair<int, int> red, pair<int, int> blue) {
    deque<pair<int, int> > q;
    vector< vector<int> > visited;

    
}

int main() {
    freopen("sample_input.txt", "r", stdin);

    cin >> n >> m;
    board.resize(n);
    char t;
    pair<int, int> red, blue;
    for (int i=0; i < n; i++) {
        for (int j=0; j < m; j++) {
            cin >> t;
            board[i] += t;
            if (t == 'R') { red = make_pair(i, j); }
            if (t == 'B') { blue = make_pair(i, j); }
        }
    }
}