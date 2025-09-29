#include <iostream>
#include <vector>

void dfs(int st, int depth, int &n, int &m, std::vector<int> &arr)
{
    if(depth == m)
    {
        for(int i = 0; i < m; i++)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << "\n";
        return;
    }

    for(int i = st; i <= n; i++)
    {
        arr[depth] = i;
        dfs(i + 1, depth + 1, n, m, arr);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // freopen("input.txt", "r", stdin);

    int N, M;

    std::cin >> N >> M;

    std::vector<int> arr(N);

    dfs(1, 0, N, M, arr);
}