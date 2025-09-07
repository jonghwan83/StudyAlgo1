#include <iostream>
#include <vector>
#include <queue>
#include <time.h>

#define INF 1e9

std::vector<int> getMinCost(int n, std::vector<std::vector<std::pair<int, int>>> graph, int st)
{
    std::queue<std::pair<int, int>> queue;
    std::vector<int> travel(n, INF);

    queue.push(std::make_pair(st, 0));
    travel[st] = 0;

    while(!queue.empty())
    {
        std::pair<int, int> curr = queue.front();
        queue.pop();

        for(auto g : graph[curr.first])
        {
            int distance = travel[curr.first] + g.second;
            if(distance < travel[g.first])
            {
                travel[g.first] = distance;
                queue.push(std::make_pair(g.first, distance));
            }  
        }
    }

    return travel;
}


int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // clock_t start = clock();

    freopen("input.txt", "r", stdin);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<std::pair<int, int>>> graph(n, std::vector<std::pair<int, int>>());

    for(int i = 0; i < m; i++)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        graph[u-1].push_back(std::make_pair(v-1, w));
    }

    for(int i = 0; i < n; i++)
    {
        std::vector<int> minCost = getMinCost(n, graph, i);
        for(int j = 0; j < n; j++)
        {
            if(minCost[j] == INF)
            {
                std::cout << 0 << " ";
            }
            else
            {
                std::cout << minCost[j] << " ";
            }
        }
        std::cout << "\n";
    }

    // std::cout << (float) (clock() - start) / CLOCKS_PER_SEC << "\n";
    
}