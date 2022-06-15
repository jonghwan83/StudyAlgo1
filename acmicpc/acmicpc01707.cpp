#include <iostream>
#include <vector>
#define MAX 20001
using namespace std;

int X[MAX], Y[MAX];
vector<int> graph[MAX];
int v, e;
bool result;

void dfs(int s){
    for (int i=0; i < graph[s].size(); i++){
        int k = graph[s][i];
        if (s != k){
            if (X[k] + Y[k] == 0) {
                if (X[s] == 1 && Y[k] == 0){
                    Y[k] = 1;
                    dfs(k);
                } else if (Y[s] == 1 && X[k] == 0) {
                    X[k] = 1;
                    dfs(k);
                }
            } else if (X[k] + Y[k] == 1) {
                if (X[s] + X[k] == 2 || Y[s] + Y[k] == 2) {
                    result = false;
                }
            }
            
        }
    }
}

void solve(){
    int p1, p2;

    cin >> v >> e;

    // initialize
    result = true;
    for (int i=0; i <= v; i++) {
        X[i] = 0;
        Y[i] = 0;
        graph[i].clear();
    }

    for (int i=0; i < e; i++) {
        cin >> p1 >> p2;
        graph[p1].push_back(p2);
        graph[p2].push_back(p1);
    }

    for (int i = 1; i <= v; i++) {
        if (X[i] == 0 && Y[i] == 0) {
            X[i] = 1;
            dfs(i);
        }
    }   
}

int main(){
    // freopen("sample.txt", "r", stdin);

    int k;
    cin >> k;

    for (int i=0; i < k; i++) {
        solve();

        if (result == 1){
            cout << "YES" << "\n";
        } else {
            cout << "NO" << "\n";
        }
    }

    return 0;
}