#include <vector>
#include <unordered_map>
#include <queue>
#include <cmath>

#define MAX_CITY 500
#define MAX_ROAD 7000
#define INF 1'000'000'000

struct Road
{
    int to, time, power, id;
    bool isRemoved;
    
    Road() {}
    Road(int mId, int eCity, int mTime, int mPower)
    {
        id = mId; to = eCity; time = mTime; power = mPower;
        isRemoved = false;
    }
};

struct Node
{
    int cost, city, time, battery;
    
    Node() {}
    Node(int c, int mCity, int mTime, int mBattery)
    {
        cost = c; city = mCity; time = mTime; battery = mBattery;
    }
};

struct compare
{
    bool operator()(Node &a, Node &b)
    {
        if(a.cost > b.cost) return true;
        return false;
    }
};


std::unordered_map<int, int> hashID;

int nCity;
int nRoad;
std::vector<Road> roads(MAX_ROAD);
std::vector<std::vector<int>> adj(MAX_CITY);
std::vector<int> minTime(MAX_CITY);
std::vector<std::vector<int>> minCost(MAX_CITY);

std::vector<int> chargingTime(MAX_CITY);

void waveEpidemic(int time, int st)
{
    std::priority_queue<Node, std::vector<Node>, compare> pq;
    
    pq.push(Node(time, st, time, 0));
    minTime[st] = time;
    
    while(!pq.empty())
    {
        Node curr = pq.top();
        pq.pop();
        
        for(int iRoad: adj[curr.city])
        {
            if(roads[iRoad].isRemoved) continue;
            int cumtime = curr.cost + roads[iRoad].time;
            if(cumtime >= minTime[roads[iRoad].to]) continue;
            
            minTime[roads[iRoad].to] = cumtime;
            pq.push(Node(cumtime, roads[iRoad].to, cumtime, 0));
        }
    }
}

void getMinCost(int st, int ed, int battery)
{
    std::priority_queue<Node, std::vector<Node>, compare> pq;
    
    pq.push(Node(0, st, 0, battery));
    for(int i = 0; i <= battery; i++)
    {
        minCost[st][i] = 0;
    }
    
    while(!pq.empty())
    {
        Node curr = pq.top();
        pq.pop();
        
        if(curr.city == ed) return;
        
        for(int iRoad: adj[curr.city])
        {
            if(roads[iRoad].isRemoved) continue;
            int to = roads[iRoad].to;
            
            int cumtime = curr.time + roads[iRoad].time;
            if(cumtime >= minTime[to]) continue;
            
            int cntToCharge = std::ceil((battery - curr.battery) / (float)chargingTime[curr.city]);
            int minToCharge = std::ceil((roads[iRoad].power-curr.battery)/ (float)chargingTime[curr.city]);
            if(minToCharge < 0) minToCharge = 0;
            for(int i = minToCharge; i <= cntToCharge; i++)
            {
                int charged = curr.battery + i * chargingTime[curr.city];
                charged = std::min(battery, charged);
                
                int time = cumtime + i;
                int remained = charged - roads[iRoad].power;
                
                if(remained < 0) continue;
                if(time >= minTime[to]) continue;
                if(minCost[to][remained] <= time) continue;
                
                minCost[to][remained] = time;
                pq.push(Node(time, to, time, remained));
            }
            
        }
    }
    return;
}

void add(int mId, int sCity, int eCity, int mTime, int mPower) {
    adj[sCity].push_back(nRoad);
    roads[nRoad] = Road(mId, eCity, mTime, mPower);
    hashID.insert(std::make_pair(mId, nRoad));
    nRoad++;
    return;
}

void init(int N, int mCharge[], int K, int mId[], int sCity[], int eCity[], int mTime[], int mPower[]) {
    nCity = N;
    
    hashID.clear();
    for(int i = 0; i < N; i++)
    {
        adj[i].clear();
        chargingTime[i] = mCharge[i];
    }
    
    nRoad = 0;
    
    for(int i = 0; i < K; i++)
    {
        add(mId[i], sCity[i], eCity[i], mTime[i], mPower[i]);
    }
    return;
}

void remove(int mId) {
    int idx = hashID[mId];
    roads[idx].isRemoved = true;
    return;
}



int cost(int B, int sCity, int eCity, int M, int mCity[], int mTime[]) {
    for(int i = 0; i < nCity; i++)
    {
        minTime[i] = INF;
        minCost[i] = std::vector<int>(B+1, INF);
    }
    
    for(int i = 0; i < M; i++)
    {
        waveEpidemic(mTime[i], mCity[i]);
    }
    
    getMinCost(sCity, eCity, B);
    int res = INF;
    
    for(int i = 0; i < B+1; i++)
    {
        if(minCost[eCity][i] < res)
        {
            res = minCost[eCity][i];
        }
    }
    
    if(res == INF) return -1;
    return res;
}
