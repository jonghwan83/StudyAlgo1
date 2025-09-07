#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // std::ignore = freopen("input.txt", "r", stdin);

    int n, x;
    std::cin >> n;
    std::vector<int> seq(n);

    for(int i = 0; i < n; i++)
    {
        int u;
        std::cin >> u;
        seq[i] = u;
    }
    std::cin >> x;

    std::sort(seq.begin(), seq.end());

    int start = 0;
    int end = seq.size()-1;

    int cnt = 0;
    while(start < end)
    {
        int res = seq[start] + seq[end];
        if (res > x)
        {
            end--;
        }
        else if (res < x)
        {
            start++;
        }
        else
        {
            cnt++;
            start++;
        }
    }

    std::cout << cnt << "\n";
}