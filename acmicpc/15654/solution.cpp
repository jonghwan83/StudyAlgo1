#include<iostream>
#include<vector>
#include<algorithm>

int n, m;
std::vector<int> result;
std::vector<int> arr;

void dfs()
{
    if(result.size() == m)
    {
        for(auto p: result)
        {
            std::cout << p << " ";
        }
        std::cout << "\n";
        return;
    }

    for(int i = 0; i < n; i++)
    {
        if(std::find(result.begin(), result.end(), arr[i]) == result.end())
        {
            result.push_back(arr[i]);
            dfs();
            result.pop_back();
        }
    }
}

int main()
{
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
}