#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

void makeAlliance(std::vector<std::vector<int>> &grid, const int &row, const int &col, const int &L, const int &R,
                    std::unordered_map<int, int> &hashAlliance, std::vector<std::vector<int>> &visited, int nVisited)
{
    int n = grid.size();

    int key = row * n + col;
    if(hashAlliance.find(key) != hashAlliance.end()) return;

    std::vector<int> drows = { -1, 0, 1, 0 };
    std::vector<int> dcols = { 0, -1, 0, 1 };

    std::queue<std::pair<int, int>> queue;

    std::vector<std::pair<int, int>> posToUpdate;
    posToUpdate.push_back(std::make_pair(row, col));

    queue.push(std::make_pair(row, col));
    visited[row][col] = nVisited;

    int cnt = 1;
    int sum = grid[row][col];

    while(!queue.empty())
    {
        std::pair<int, int> curr = queue.front();
        queue.pop();

        for(int i = 0; i < 4; i++)
        {
            int dr = curr.first + drows[i];
            int dc = curr.second + dcols[i];

            if(dr < 0 || dr > n-1) continue;
            if(dc < 0 || dc > n-1) continue;

            if(visited[dr][dc] >= nVisited) continue;
            int diff = std::abs(grid[curr.first][curr.second] - grid[dr][dc]);
            if(diff < L || diff > R) continue;
            
            visited[dr][dc] = nVisited;
            queue.push(std::make_pair(dr, dc));
            posToUpdate.push_back(std::make_pair(dr, dc));

            cnt++;
            sum += grid[dr][dc];
        }
    }

    if(posToUpdate.size() <= 1) return;

    int avg = sum / cnt;

    for(std::pair<int, int> pos : posToUpdate)
    {
        hashAlliance.insert(std::make_pair(pos.first * n + pos.second, avg));
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    
    // freopen("input.txt", "r", stdin);

    int N, L, R;
    std::cin >> N >> L >> R;

    std::vector<std::vector<int>> grid(N, std::vector<int>(N));
    std::vector<std::vector<int>> visited(N, std::vector<int>(N));
    std::unordered_map<int, int> hashAlliance;

    int cnt = 0;

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            int u;
            std::cin >> u;
            grid[i][j] = u;
        }
    }

    while(true)
    {
        hashAlliance.clear();
        
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                makeAlliance(grid, i, j, L, R, hashAlliance, visited, cnt+1);
            }
        }
        
        if(hashAlliance.size() == 0) break;
        for(std::pair<int, int> p : hashAlliance)
        {
            int avg = p.second;
            int row = p.first / N;
            int col = p.first % N;
            grid[row][col] = avg;
        }

        cnt++;
    }

    std::cout << cnt << "\n";
}