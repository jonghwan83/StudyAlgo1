#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // freopen("sample_input.txt", "r", stdin);

    int n, m;

    std::cin >> n >> m;

    std::vector<std::vector<int>> dp(1025, std::vector<int>(1025, 0));

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            int k;
            std::cin >> k;
            
            if(i == 1 && j == 1)
            {
                dp[i][j] = k;
            }
            else if(i == 1 && j > 1)
            {
                dp[i][j] = dp[i][j-1] + k;
            }
            else if(j == 1)
            {
                dp[i][j] = dp[i-1][j] + k;
            }
            else
            {
                dp[i][j] = dp[i-1][j] + dp[i][j-1] + k - dp[i-1][j-1];
            }
        }
    }

    for(int i = 0; i < m; i++)
    {
        int r1, r2, c1, c2;
        std::cin >> r1 >> c1 >> r2 >> c2;

        int res = dp[r2][c2] - dp[r2][c1-1] 
                - dp[r1-1][c2] + dp[r1-1][c1-1];

        std::cout << res << "\n";
    }
}