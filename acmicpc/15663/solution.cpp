#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <time.h>

int n, m;
std::vector<int> arr;
std::vector<int> subset;
std::vector<bool> hashtable(8, false);


void dfs()
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

    for(int i = 0; i < arr.size(); i++)
    {
        if(hashtable[i] || arr[i] == last)
            continue;
        subset.push_back(arr[i]);
        hashtable[i] = true;
        last = arr[i];
        dfs();
        subset.pop_back();
        hashtable[i] = false;
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

    dfs();

    // std::cout << (float) (clock() - start) / CLOCKS_PER_SEC << "\n";
}