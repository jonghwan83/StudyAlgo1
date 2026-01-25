#include <iostream>
#include <algorithm>
#include <vector>
#include <time.h>

int n, m;
std::vector<int> arr;
std::vector<int> subset;

void dfs(int st)
{
    if(subset.size() == m)
    {
        for(auto num: subset)
        {
            std::cout << num << " ";
        }
        std::cout << "\n";
        return;
    }

    int last = 0;

    for(int i = st; i < arr.size(); i++)
    {
        if(arr[i] != last)
        {
            subset.push_back(arr[i]);
            last = arr[i];
            dfs(i);
            subset.pop_back();
        }
        
    }
}

int main()
{
    clock_t start = clock();

    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // freopen("sample_input.txt", "r", stdin);

    std::cin >> n >> m;

    for(int i = 0; i < n; i++)
    {
        int k;
        std::cin >> k;

        arr.push_back(k);
    }

    std::sort(arr.begin(), arr.end());

    dfs(0);

    // std::cout << (float) (clock() - start) / CLOCKS_PER_SEC << "\n";
}