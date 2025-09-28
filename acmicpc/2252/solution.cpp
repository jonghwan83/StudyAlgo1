#include <iostream>
#include <vector>
#include <cstdlib>

std::vector<int> res;

void dfs(int curr, std::vector<std::vector<int>> &graph, std::vector<bool> &visited)
{
    visited[curr] = 1;
    for(auto g : graph[curr])
    {
        if(visited[g] == 0)
        {
            dfs(g, graph, visited);
        }   
    }

    res.push_back(curr);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // freopen("input.txt", "r", stdin);

    int n, m;
    std::cin >> n >> m;

    std::vector<int> s(n+1, 0);
    std::vector<std::vector<int>> graph(n+1);
    std::vector<bool> visited(n+1, 0);

    for(int i = 0; i < m; i++)
    {
        int u, v;
        std::cin >> u >> v;
        graph[v].push_back(u);
        s[u]++;
    }

    for(int i = 1; i <= n; i++)
    {
        if(s[i] == 0)
        {
            dfs(i, graph, visited);
        }
        
    }

    for(auto i : res)
    {
        std::cout << i << " ";
    }
}