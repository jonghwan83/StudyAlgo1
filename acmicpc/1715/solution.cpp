#include <iostream>
#include <vector>
#include <queue>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // freopen("input.txt", "r", stdin);

    int n;

    std::cin >> n;

    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;

    for(int i = 0; i < n; i++)
    {
        int u;
        std::cin >> u;
        pq.push(u);
    }

    if(n == 1)
    {
        std::cout << 0 << "\n";
        return 0;
    }

    int cnt = 0;

    while(pq.size() > 1)
    {
        int first = pq.top();
        pq.pop();

        int second = pq.top();
        pq.pop();

        int sum = first + second;

        pq.push(sum);
        cnt += sum;
    }

    std::cout << cnt << "\n";

}