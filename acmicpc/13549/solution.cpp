#include <iostream>
#include <queue>
#include <vector>

#define MAX_N 100001

struct compare
{
    bool operator()(std::pair<int, int> &a, std::pair<int, int> &b)
    {
        if(a.second > b.second)
            return true;
        return false;
    }
};

int bfs(int st, int ed)
{
    int cnt = 0;

    std::vector<int> dx = { -1, 1 };

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, compare> q;
    std::vector<bool> visited(MAX_N, false);

    q.push({ st, cnt });
    visited[st] = true;

    while(!q.empty())
    {
        std::pair<int, int> curr = q.top();
        q.pop();

        int x = curr.first;
        visited[x] = true;

        if(x == ed)
            return curr.second;

        for(int i = 0; i < 2; i++)
        {
            int next = x + dx[i];

            if(next < 0 || next > 100000) continue;
            if(!visited[next])
            {
                q.push({ next, curr.second + 1 });
            }
        }

        int next = x * 2;
        if(next < 0 || next > 100000) continue;
        if(!visited[next])
        {
            q.push({ next, curr.second });
        }
    }
    
    return cnt;
}

int main()
{
    // freopen("sample_input.txt", "r", stdin);

    int n, k;
    std::cin >> n >> k;

    int res = bfs(n, k);

    std::cout << res << "\n";
}