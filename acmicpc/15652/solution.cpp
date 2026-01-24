#include <iostream>
#include <vector>

int n, m;
std::vector<int> arr;

void dfs(int st)
{
    if(arr.size() == m)
    {
        for(auto p: arr)
        {
            std::cout << p << " ";
        }
        std::cout << "\n";
        return;
    }

    for(int i = st; i < n + 1; i++)
    {
        arr.push_back(i);
        dfs(i);
        arr.pop_back();
    }
}

int main()
{
    // freopen("sample_input.txt", "r", stdin);

    std::cin >> n >> m;

    dfs(1);
}