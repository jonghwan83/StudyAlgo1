#include <vector>
#include <deque>
#include <iostream>
#include <cmath>

using namespace std;


int n, m;
vector<string> board;
bool visited[11][11][11][11];
vector<int> drows = { -1, 1, 0, 0 };
vector<int> dcols = { 0, 0, -1, 1 };

void bfs(pair<int, int> red, pair<int, int> blue) {
    deque< vector<int> > queue;
    queue.push_back({ red.first, red.second, blue.first, blue.second, 1 });
    visited[red.first][red.second][blue.first][blue.second] = true;

    vector<int> loc;
    int rr, rc, br, bc, cnt;
    bool isRedHole, isBlueHole;

    while (!queue.empty()) {
        loc = queue.front();
        queue.pop_front();

        for (int i=0; i < 4; i++) {
            isRedHole = false;
            isBlueHole = false;

            rr = loc[0];
            rc = loc[1];
            br = loc[2];
            bc = loc[3];
            cnt = loc[4];

            while (board[rr + drows[i]][rc + dcols[i]] != '#') {
                rr += drows[i];
                rc += dcols[i];
                if (board[rr][rc] == 'O') {
                    isRedHole = true;
                    break;
                }
            }

            while (board[br + drows[i]][bc + dcols[i]] != '#') {
                br += drows[i];
                bc += dcols[i];
                if (board[br][bc] == 'O') {
                    isBlueHole = true;
                    break;
                }
            }

            if (rr == br && rc == bc) {
                int redTrail = abs(loc[0] - rr) + abs(loc[1] - rc);
                int blueTrain = abs(loc[2] - br) + abs(loc[3] - bc);
                if (redTrail > blueTrain) {
                    rr -= drows[i];
                    rc -= dcols[i];
                }
                else {
                    br -= drows[i];
                    bc -= dcols[i];
                }
            }

            if (isBlueHole) { continue; }
            if (isRedHole) {
                if (cnt > 10) {
                    cout << -1 << "\n";
                    exit(0);
                }
                cout << cnt <<"\n";
                exit(0);
            }

            if (!visited[rr][rc][br][bc]) {
                queue.push_back({ rr, rc, br, bc, cnt + 1 });
                visited[rr][rc][br][bc] = true;
            }
        }

    }

}

int main() {
//    freopen("../sample_input.txt", "r", stdin);
    pair<int, int> red, blue;

    cin >> n >> m;
    board.resize(n);

    char t;
    for (int i=0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> t;
            board[i] += t;
            if (t == 'R') { red = make_pair(i, j); }
            if (t == 'B') { blue = make_pair(i, j); }
        }
    }

    bfs(red, blue);

    cout << -1;
}