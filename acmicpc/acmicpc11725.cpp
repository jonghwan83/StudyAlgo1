#include <iostream>
#include <vector>
#include <deque>

#define VMAX 100000

using namespace std;

int n;
vector< vector<int> > graph;
vector<int> hashTable;

void bfs(int start) {
    deque<int> dQueue;
    vector<bool> visited(n + 1);

    dQueue.push_back(start);
    visited[start] = true;

    int c;
    while (dQueue.size() > 0) {
        c = dQueue.front();
        dQueue.pop_front();

        for (int i=0; i < graph[c].size(); i++) {
            if (!visited[graph[c][i]]) {
                hashTable[graph[c][i]] = c;
                dQueue.push_back(graph[c][i]);
                visited[graph[c][i]] = true;
            }
        }
    }
}

int main() {
    // freopen("sample_input.txt", "r", stdin);

    cin >> n;
    
    graph.resize(n + 1);
    hashTable.resize(n + 1);

    int x, y;
    for (int i=0; i < n; i++) {
        cin >> x >> y;
        graph[x].push_back(y);
        graph[y].push_back(x);
    }

    bfs(1);
    for (int i=2; i <= n; i++) {
        cout << hashTable[i] << "\n";
    }

    return 0;
}