#define MAX_CITY 500
#define MAX_ROAD 7000
#define INF 1'000'000'000
#define MAX_B 301

int ceiling(float a)
{
    int b = (int) a;
    if (a - b > 0)
    {
        return b + 1;
    }
    return  b;;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

struct Node
{
    int id;
    int idx;
    Node* next;
    
    Node(int mId, int i)
    {
        id = mId; idx = i;
        next = nullptr;
    }
};

struct LinkedList
{
    int length;
    Node* head;
    
    void init()
    {
        length = 0;
        head = nullptr;
    }
    
    void insert(int mId, int i)
    {
        Node* node = new Node(mId, i);
        
        node->next = head;
        head = node;
        
        length++;
    }
    
    int findByID(int mId)
    {
        Node* curr = head;
        while(curr)
        {
            if(curr->id == mId)
            {
                return curr->idx;
            }
            curr = curr->next;
        }
        return -1;
    }
};

struct ToCities
{
    int k;
    int to[MAX_CITY];
    int idx[MAX_CITY];
    
    void init()
    {
        k = 0;
    }
    
    void add(int next, int i)
    {
        to[k] = next;
        idx[k] = i;
        k++;
    }
    
};

struct Road
{
    int time;
    int power;
    bool isRemoved;
    
    Road() {}
    Road(int mTime, int mPower)
    {
        time = mTime;
        power = mPower;
        isRemoved = false;
    }
};

struct HeapNode
{
    int time, city, battery;
    
    HeapNode() {}
    HeapNode(int mTime, int mCity, int mBattery)
    {
        time = mTime; city = mCity; battery = mBattery;
    }
};

struct priorityQueue
{
    int length;
    HeapNode arr[MAX_CITY * MAX_B * 2];
    
    void init()
    {
        length = 0;
    }
    
    bool compare(int parent, int child)
    {
        if(arr[parent].time > arr[child].time) return true;
        return false;
    }
    
    void push(int mTime, int mCity, int mBattery)
    {
        HeapNode node = HeapNode(mTime, mCity, mBattery);
        
        int idx = length;
        arr[length++] = node;
        
        while((idx-1)/2 >= 0 && compare((idx-1)/2, idx))
        {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx-1)/2];
            arr[(idx-1)/2] = temp;
            idx = (idx-1)/2;
        }
    }
    
    HeapNode pop()
    {
        HeapNode top = arr[0];
        
        arr[0] = arr[--length];
        
        int idx = 0;
        int left, right, child;
        
        while(2*idx + 1 < length)
        {
            left = 2*idx + 1;
            right = 2*idx + 2;
            
            if(right < length)
                if(compare(left, right)) child = right;
                else child = left;
            else child = left;
            
            if(compare(idx, child))
            {
                HeapNode temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else
            {
                break;
            }
        }
        
        return top;
    }
    
};


int nCity;
LinkedList hashID;
ToCities roadByCity[MAX_CITY];
Road roads[MAX_ROAD];
int chargerInCity[MAX_CITY];
int nRoad;

priorityQueue pq;
int minTime[MAX_CITY];
int minCost[MAX_CITY][MAX_B];


void waveEpidemic(int sCity, int mTime)
{
    pq.init();
    pq.push(mTime, sCity, 0);
    minTime[sCity] = min(mTime, minTime[sCity]);
    
    while(pq.length > 0)
    {
        HeapNode curr = pq.pop();
        
        for(int i = 0; i < roadByCity[curr.city].k; i++)
        {
            if(roads[roadByCity[curr.city].idx[i]].isRemoved) continue;
            int to = roadByCity[curr.city].to[i];
            int time = roads[roadByCity[curr.city].idx[i]].time;
            
            int cumtime = curr.time + time;
            if(minTime[to] <= cumtime) continue;
            
            minTime[to] = cumtime;
            pq.push(cumtime, to, 0);
        }
    }
}

void getMinCost(int sCity, int eCity, int battery)
{
    pq.init();
    pq.push(0, sCity, battery);
    for(int i = 0; i <= battery; i++)
    {
        minCost[sCity][i] = 0;
    }
    
    while(pq.length > 0)
    {
        HeapNode curr = pq.pop();
        
        if(curr.city == eCity) return;
        
        for(int i = 0; i < roadByCity[curr.city].k; i++)
        {
            if(roads[roadByCity[curr.city].idx[i]].isRemoved) continue;
            int to = roadByCity[curr.city].to[i];
            int time = roads[roadByCity[curr.city].idx[i]].time;
            int power = roads[roadByCity[curr.city].idx[i]].power;
            
            int cumtime = curr.time + time;
            if(cumtime >= minTime[to]) continue;
            
            int cntToCharge = ceiling((battery - curr.battery) / (float) chargerInCity[curr.city]);
            int minToCharge = ceiling((power - curr.battery) / (float) chargerInCity[curr.city]);
            if(minToCharge < 0) minToCharge = 0;
            
            for(int k = minToCharge; k <= cntToCharge; k++)
            {
                int charged = curr.battery + k * chargerInCity[curr.city];
                charged = min(charged, battery);
                
                int timeWithCharge = cumtime + k;
                int remain = charged - power;
                
                if(remain < 0) continue;
                if(minTime[to] <= timeWithCharge) continue;
                if(minCost[to][remain] <= timeWithCharge) continue;
                
                minCost[to][remain] = timeWithCharge;
                pq.push(timeWithCharge, to, remain);
            }
        }
    }
}

void add(int mId, int sCity, int eCity, int mTime, int mPower) {
    roadByCity[sCity].add(eCity, nRoad);
    hashID.insert(mId, nRoad);
    roads[nRoad] = Road(mTime, mPower);
    
    nRoad++;
    return;
}

void init(int N, int mCharge[], int K, int mId[], int sCity[], int eCity[], int mTime[], int mPower[]) {
    hashID.init();
    nRoad = 0;
    nCity = N;
    
    for(int i = 0; i < N; i++)
    {
        roadByCity[i].init();
        chargerInCity[i] = mCharge[i];
    }
    
    for(int i = 0; i < K; i++)
    {
        add(mId[i], sCity[i], eCity[i], mTime[i], mPower[i]);
    }
    
    return;
}

void remove(int mId) {
    int idx = hashID.findByID(mId);
    
    roads[idx].isRemoved = true;
    return;
}



int cost(int B, int sCity, int eCity, int M, int mCity[], int mTime[]) {
    for(int i = 0; i < nCity; i++)
    {
        minTime[i] = INF;
        for(int j = 0; j <= B; j++)
        {
            minCost[i][j] = INF;
        }
    }
    
    for(int i = 0; i < M; i++)
    {
        waveEpidemic(mCity[i], mTime[i]);
    }
    
    getMinCost(sCity, eCity, B);
    
    int res = INF;
    for(int i = 0; i <= B; i++)
    {
        if(minCost[eCity][i] < res)
        {
            res = minCost[eCity][i];
        }
    }
    if(res == INF)
        return -1;
    
    return res;
}
