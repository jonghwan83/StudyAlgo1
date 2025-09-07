#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // freopen("input.txt", "r", stdin);

    int n, K;
    std::cin >> n >> K;

    std::vector<int> seq(n);
    
    for(int i = 0; i < n; i++)
    {
        int u;
        std::cin >> u;
        seq[i] = u;
        if(seq[i] >= K)
        {
            std::cout << 1 << "\n";
            return 0;
        }
    }

    int start = 0;
    int end = 1;

    int num = seq[start] + seq[end];
    int minlength = n + 1;

    while(end < n)
    {
        if(num < K)
        {
            end++;
            if(end >= n) break;
            num += seq[end];
        }
        else
        {
            if(end - start + 1 < minlength)
            {
                minlength = end - start + 1;
            }
            num -= seq[start];
            start++;
        }
    }
    if(minlength == n + 1)
    {
        std::cout << 0 << "\n";
    }
    else
    {
        std::cout << minlength << "\n";
    }
    
}