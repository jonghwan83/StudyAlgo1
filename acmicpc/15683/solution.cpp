#include <iostream>
#include <vector>
#include <queue>

std::vector<int> drow = {0, 1, 0, -1};
std::vector<int> dcol = {-1, 0, 1, 0};

std::vector<std::vector<int>> cctv;

int N, M;
std::vector<std::vector<int>> map(8, std::vector<int>(8, 0));

int answer = 1e9;

void check(int row, int col, int dir)
{
    int nrow = row;
    int ncol = col;

    while(true)
    {
        nrow += drow[dir];
        ncol += dcol[dir];

        if(nrow < 0 || nrow >= N || ncol < 0 || ncol >= M)
        {
            break;
        }

        if(map[nrow][ncol] == 6)
        {
            break;
        }

        if(map[nrow][ncol] == 0)
        {
            map[nrow][ncol] = -1; // 감시됨
        }
    }
}

void dfs(int idx)
{
    if(idx == cctv.size())
    {
        int count = 0;
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < M; j++)
            {
                if(map[i][j] == 0)
                {
                    count++;
                }
            }
        }
        answer = std::min(answer, count);
        return;
    }

    int row = cctv[idx][0];
    int col = cctv[idx][1];
    int type = cctv[idx][2];

    std::vector<std::vector<int>> temp_map = map;

    for(int dir = 0; dir < 4; dir++)
    {
        if(type == 1)
        {
            // 방향 하나
            check(row, col, dir);
        }
        else if(type == 2)
        {
            // 방향 두개
            check(row, col, dir);
            check(row, col, (dir + 2) % 4);
        }
        else if(type == 3)
        {
            // 방향 두개 직각
            check(row, col, dir);
            check(row, col, (dir + 1) % 4);
        }
        else if(type == 4)
        {
            // 방향 세개
            check(row, col, dir);
            check(row, col, (dir + 1) % 4);
            check(row, col, (dir + 2) % 4);
        }
        else if(type == 5)
        {
            // 방향 네개
            check(row, col, dir);
            check(row, col, (dir + 1) % 4);
            check(row, col, (dir + 2) % 4);
            check(row, col, (dir + 3) % 4);
        }

        dfs(idx + 1);
        map = temp_map;
    }

}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    
    // freopen("sample_input.txt", "r", stdin);

    std::cin >> N >> M;

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            std::cin >> map[i][j];
            if(map[i][j] >= 1 && map[i][j] <= 5)
            {
                cctv.push_back({i, j, map[i][j]});
            }
        }
    }

    dfs(0);

    std::cout << answer << "\n";
    return 0;
}