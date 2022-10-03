#include <iostream>
#include <vector>
#include <string>

using namespace std;

int n;
vector<string> graph;
vector< vector<int> > visited;
vector<int> temp;

vector<int> drows = { -1, 1, 0, 0 };
vector<int> dcols = { 0, 0, -1, 1 };

int dr, dc;
char color_graph;
void dfs(int r, int c, char color) {
    color_graph = graph[r][c];
    if (visited[r][c] || color_graph != color) {
        return;
    }
    else {
        visited[r][c] = 1;
    }

    for (int i=0; i < n; i++) {
        dr = r + drows[i];
        dc = c + dcols[i];

        if (0 <= dr && dr < n && 0 <= dc && dc < n) { 
            if (!visited[dr][dc]) {
                dfs(dr, dc, color);
            }
        }
    }

}

int main() {
    // freopen("sample_input.txt", "r", stdin);

    int human, cow;
    human = 0;
    cow = 0;

    cin >> n;

    string s;
    for (int i=0; i < n; i++) {
        cin >> s;
        graph.push_back(s);
        temp.clear();
        for (int j=0; j < n; j++) {
            temp.push_back(0);
        }
        visited.push_back(temp);
    }

    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
            if (!visited[i][j]) {
                dfs(i, j, graph[i][j]);
                human++;
            }
        }
    }
    char c;
    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
            visited[i][j] = 0;
            c = graph[i][j];
            if (c == 'R') { graph[i][j] = 'G'; }
        }
    }

    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++) {
            if (!visited[i][j]) {
                dfs(i, j, graph[i][j]);
                cow++;
            }
        }
    }

    cout << human << " " << cow << "\n";
}