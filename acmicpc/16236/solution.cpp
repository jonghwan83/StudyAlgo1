#include <iostream>
#include <vector>
#include <queue>

#define INF 100000

std::vector<int> getMinCost(const int &n, std::vector<std::vector<int>> &graph,int row, int col, int bulk)
{
    std::vector<std::vector<int>> minCost(n, std::vector<int>(n, INF));

    std::queue<std::pair<int, int>> queue;
    queue.push(std::make_pair(row, col));
    minCost[row][col] = 0;

    int drows[4] = { -1, 0, 1, 0 };
    int dcols[4] = {0, -1, 0, 1 };

    int minValue = INF;
    std::pair<int, int> loc = std::make_pair(INF, INF);

    while(!queue.empty())
    {
        auto curr = queue.front();
        queue.pop();

        for(int i = 0; i < 4; i++)
        {
            int dr = drows[i] + curr.first;
            int dc = dcols[i] + curr.second;

            if(dr < 0 || dr >= n) continue;
            if(dc < 0 || dc >= n) continue;

            if(graph[dr][dc] > bulk) continue;

            int dist = minCost[curr.first][curr.second] + 1;
            if(dist < minCost[dr][dc])
            {
                minCost[dr][dc] = dist;
                queue.push(std::make_pair(dr, dc));

                if(graph[dr][dc] == bulk) continue;
                if(graph[dr][dc] == 0) continue;

                if(dist < minValue)
                {
                    minValue = dist;
                    loc = std::make_pair(dr, dc);
                }
                else if(dist == minValue && dr < loc.first)
                {
                    minValue = dist;
                    loc = std::make_pair(dr, dc);
                }
                else if(dist == minValue && dr == loc.first && dc < loc.second)
                {
                    minValue = dist;
                    loc = std::make_pair(dr, dc);
                }
            }
        }
    }

    return { minValue, loc.first, loc.second };
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // freopen("input.txt", "r", stdin);

    int totalsec = 0;

    int n;
    std::cin >> n;

    std::vector<std::vector<int>> graph(n, std::vector<int>(n, 0));

    std::pair<int, int> pos;
    int bulk = 2;
    int eaten = 0;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            int u;
            std::cin >> u;
            if(0 < u && u < 9)
            {
                graph[i][j] = u;
            }
            else if(u == 9)
            {
                pos = std::make_pair(i, j);
            }
        }
    }

    while(true)
    {
        auto target = getMinCost(n, graph, pos.first, pos.second, bulk);
        if(target[1] == INF) break;

        graph[target[1]][target[2]] = 0;
        totalsec += target[0];
        pos = std::make_pair(target[1], target[2]);
        eaten++;

        if(bulk == eaten)
        {
            bulk++;
            eaten = 0;
        }
    }

    std::cout << totalsec << "\n";
}