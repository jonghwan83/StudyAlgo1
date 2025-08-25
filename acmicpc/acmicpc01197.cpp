// 1 <= V <= 10'000
// 1 <= E <= 100'000
#include <iostream>
#include <queue>
#include <vector>

#define MAX_V 10001

using namespace std;

struct Edge 
{
    int from, to, weight;
    Edge(int f, int t, int w) : from(f), to(t), weight(w) {}
};

struct cmp
{
    bool operator()(const Edge& a, const Edge& b) {
        return a.weight > b.weight;
    }
};

int find(int a, int parent[MAX_V])
{
    if(parent[a] == a) return a;
    return find(parent[a], parent);
}

void _union(int a, int b, int parent[MAX_V], int size[MAX_V])
{
    int rootA = find(a, parent);
    int rootB = find(b, parent);
    if(rootA == rootB) {
        return;
    }

    if(size[rootA] < size[rootB]) 
    {
        parent[rootA] = rootB;
        size[rootB] += size[rootA];
    } 
    else 
    {
        parent[rootB] = rootA;
        size[rootA] += size[rootB];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    // freopen("input.txt", "r", stdin);

    int answer = 0;

    int V, E;
    priority_queue<Edge, vector<Edge>, cmp> pq;

    int parent[MAX_V];
    int size[MAX_V];
    
    cin >> V >> E;

    for(int i = 0; i <= V; i++)
    {
        parent[i] = i;
        size[i] = 1;
    }
        
    for(int i = 0; i < E; i++)
    {
        int from, to, weight;
        cin >> from >> to >> weight;
        pq.push(Edge(from, to, weight));
    }

    while(!pq.empty())
    {
        Edge curr = pq.top();
        pq.pop();

        if(size[curr.from] >= V || size[curr.to] >= V)
        {
            break;
        }

        if(find(curr.from, parent) != find(curr.to, parent))
        {
            _union(curr.from, curr.to, parent, size);
            answer += curr.weight;
        }
    }

    cout << answer << "\n";

    return 0;
}