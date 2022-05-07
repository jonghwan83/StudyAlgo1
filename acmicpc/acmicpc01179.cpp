#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

vector<int> parent;

int find(int x){
    if (parent[x] == x){
        return x;
    } else {
        return find(parent[x]);
    }
}


int main(){
    // freopen("sample.txt", "r", stdin);

    int v, e;
    cin >> v >> e;
    vector< vector<int> > graph;

    parent.push_back(0);
    for (int i=1; i < (v + 1); i++){
        parent.push_back(i);
    }

    for (int i=0; i < e; i++){
        vector<int> temp;
        int node, nxt, w;
        cin >> node >> nxt >> w;
        temp.push_back(w);
        temp.push_back(node);
        temp.push_back(nxt);
        graph.push_back(temp);
    }
    
    sort(graph.begin(), graph.end());

    int answer = 0;
    for (int i=0; i < e; i++){
        int x, y;
        x = find(graph[i][1]);
        y = find(graph[i][2]);
        if (x != y){
            parent[y] = x;
            answer = answer + graph[i][0];
        }
    }
    
    cout << answer << "\n";
    
}