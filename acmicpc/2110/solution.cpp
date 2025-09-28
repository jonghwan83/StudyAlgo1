#include <iostream>
#include <vector>
#include <algorithm>

bool isPossible(std::vector<int> &xi, int target, int min_dist)
{
    int last_idx = 0;
    int cnt = 1;
    for(int i = 1; i < xi.size(); i++)
    {
        int dist = xi[i] - xi[last_idx];
        if(dist >= min_dist)
        {
            cnt++;
            last_idx = i;
        }

        if(cnt >= target) return true;
    }

    if(cnt >= target) return true;
    return false;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // freopen("input.txt", "r", stdin);

    int N, C;
    std::cin >> N >> C;
    
    std::vector<int> xi(N);
    for(int i = 0; i < N; i++)
    {
        int u;
        std::cin >> u;
        xi[i] = u;
    }

    std::sort(xi.begin(), xi.end());

    if(C == 2)
    {
        std::cout << xi[N-1] - xi[0] << "\n";
        return 0;
    }

    int left = 0;
    int right = xi[N-1];
    int mid = -1;
    int answer = -1;

    while(right >= left)
    {
        mid = (left + right) / 2;
        bool res = isPossible(xi, C, mid);

        if(!res)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1; 
            answer = std::max(answer, mid);
        }
    }

    std::cout << answer << "\n";
}