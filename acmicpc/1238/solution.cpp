#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct compare
{
    bool operator()(std::pair<int, int> &a, std::pair<int, int> &b)
    {
        if(a.second > b.second) return true;
        return false;
    }
};

std::vector<int> getMinTime(int &n, std::vector<std::vector<std::pair<int, int>>> &path, int &st)
{
    std::vector<int> minTime(n+1, 1e9);

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, compare> pq;
    pq.push(std::make_pair(st, 0));
    minTime[st] = 0;

    while(!pq.empty())
    {
        std::pair<int, int> curr = pq.top();
        pq.pop();

        for(std::pair<int, int> p: path[curr.first])
        {
            int to = p.first;
            int time = curr.second + p.second;
            
            if(minTime[to] < time) continue;
            minTime[to] = time;
            pq.push(std::make_pair(to, time));
        }  
    }

    return minTime;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // freopen("input.txt", "r", stdin);

    int N, M, X;
    std::cin >> N >> M >> X;

    std::vector<std::vector<std::pair<int, int>>> roads(M+1);

    for(int i = 0; i < M; i++)
    {
        int A, B, T;
        std::cin >> A >> B >> T;
        
        roads[A].push_back(std::make_pair(B, T));
    }

    int maxRoundTrip = 0;
    std::vector<int> timeToHome = getMinTime(N, roads, X);

    for(int i = 1; i <= N; i++)
    {
        std::vector<int> res = getMinTime(N, roads, i);
        maxRoundTrip = std::max(timeToHome[i]+res[X], maxRoundTrip);
    }
    
    std::cout << maxRoundTrip << "\n";
}