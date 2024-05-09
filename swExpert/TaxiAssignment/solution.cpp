#include <iostream>

#define ABS(x) (((x) > 0) ? (x) : -(x))


struct Coordinate

{
    
    int y, x;
};

struct Passenger

{
    
    Coordinate departure;
    
    Coordinate arrival;
};


static const int MAX_PASSENGER_COUNT = 10'000;

static const int MAX_DRIVER_COUNT = 100;

static const int MAP_SIZE = 100;

int max(int a, int b) { return a > b? a : b; }


int getDistance(Coordinate a, Coordinate b)
{
    return ABS(a.x - b.x) + ABS(a.y - b.y);
}

extern bool assign_driver(int driverID, int passengerSize, int passengerIDs[]);


struct Node
{
    int distance;
    int id;
};

class Heap
{
public:
    int length;
    Node arr[MAX_PASSENGER_COUNT];
    
    void init() { length = 0; }
    
    bool compare(int parent, int child)
    {
        if (arr[parent].distance > arr[child].distance) { return true; }
        return false;
    }
    
    void push(int dist, int id)
    {
        Node node = { dist, id };
        
        int idx = length;
        arr[length++] = node;
        
        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx))
        {
            Node temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }
    
    Node pop()
    {
        Node ans = arr[0];
        
        arr[0] = arr[--length];
        
        int idx = 0;
        int left, right, child;
        
        while (2 * idx + 1 < length)
        {
            left = 2 * idx + 1;
            right = 2 * idx + 2;
            
            if (right < length)
                if (compare(left, right))
                    child = right;
                else
                    child = left;
                else
                    child = left;
            
            if (compare(idx, child))
            {
                Node temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }
        
        return ans;
    }
};


class Queue
{
public:
    Coordinate arr[MAP_SIZE * MAP_SIZE];
    int st, ed, length;
    
    void init()
    {
        st = ed = length = 0;
    }
    
    void push(int y, int x)
    {
        length++;
        arr[ed++] = { y, x };
    }
    
    Coordinate pop()
    {
        length--;
        return arr[st++];
    }
    
};



Heap pQueue[MAX_DRIVER_COUNT];

bool finished[MAX_PASSENGER_COUNT];


int passengerSize[MAX_DRIVER_COUNT];

int passengerIDs[MAX_DRIVER_COUNT][MAX_PASSENGER_COUNT];

Coordinate driverLocation[MAX_DRIVER_COUNT];

int score[MAX_DRIVER_COUNT];

int passengerOnMap[MAP_SIZE][MAP_SIZE][1000];
int cntOnMap[MAP_SIZE][MAP_SIZE];


Queue queue;
int visited[MAP_SIZE][MAP_SIZE];
int nVisited;

int subsize;

void bfs(int driver, Passenger mPassenger[])
{
    int drows[4] = { -1, 0, 1, 0 };
    int dcols[4] = { 0, -1, 0, 1 };
    
    nVisited++;
    
    queue.init();

    int y = driverLocation[driver].y / subsize;
    int x = driverLocation[driver].x / subsize;
    
    queue.push(y, x);
    visited[y][x] = nVisited;
    
    for (int p = 0; p < cntOnMap[y][x]; p++)
    {
        int psger = passengerOnMap[y][x][p];
        if (!finished[psger])
        {
            int dist = getDistance(driverLocation[driver], mPassenger[psger].departure);
            pQueue[driver].push(dist, psger);
        }
        
    }
    
    while (queue.length > 0)
    {
        Coordinate curr = queue.pop();
        
        for (int i = 0; i < 4; i++)
        {
            int dy = curr.y + drows[i];
            int dx = curr.x + dcols[i];
            
            if (dx < 0 || dy < 0) { continue; }
            if (dx >= MAP_SIZE || dy >= MAP_SIZE) { continue; }
            if (visited[dy][dx] >= nVisited) { continue; }
            
            for (int p = 0; p < cntOnMap[dy][dx]; p++)
            {
                int psger = passengerOnMap[dy][dx][p];
                if (!finished[psger])
                {
                    int dist = getDistance(driverLocation[driver], mPassenger[psger].departure);
                    pQueue[driver].push(dist, psger);
                }
                
            }
            
            queue.push(dy, dx);
            visited[dy][dx] = nVisited;
            
            if (pQueue[driver].length > 100)
            {
                return;
            }
            
        }
        
    }
    
}



void run(int N, int M, Coordinate mDriver[], int K, Passenger mPassenger[])
{
    nVisited = 0;
    
    subsize = N / MAP_SIZE;
    
    for (int row = 0; row < MAP_SIZE; row++)
        for (int col = 0; col < MAP_SIZE; col++)
        {
            cntOnMap[row][col] = 0;
            visited[row][col] = 0;
        }
    
    for (int passenger = 0; passenger < K; passenger++)
    {
        int row = mPassenger[passenger].departure.y / subsize;
        int col = mPassenger[passenger].departure.x / subsize;
        
        passengerOnMap[row][col][cntOnMap[row][col]++] = passenger;
    }
    
    
    for (int driver = 0; driver < M; driver++)
    {
        passengerSize[driver] = 0;
        
        driverLocation[driver] = mDriver[driver];
        
        score[driver] = 0;
        
        pQueue[driver].init();
        
        for (int passenger = 0; passenger < K; passenger++)
        {
            int dist = getDistance(mDriver[driver], mPassenger[passenger].departure);
            
            pQueue[driver].push(dist, passenger);
            
            if (driver == 0)
            {
                finished[passenger] = 0;
            }
        }
    }
    
    
    int cnt = 0;
    
    while (cnt < K)
    {
        
        int curr_driver = -1;
        
        int min_dist = 4 * N * 100;
        
        for (int driver = 0; driver < M; driver++)
        {
            
            if (pQueue[driver].arr[0].distance + score[driver] < min_dist)
            {
                min_dist = pQueue[driver].arr[0].distance + score[driver];
                curr_driver = driver;
            }
        }
        
        
        Node curr = pQueue[curr_driver].pop();
        
        int passenger = curr.id;
        
        passengerIDs[curr_driver][passengerSize[curr_driver]++] = passenger;
        
        score[curr_driver] += (getDistance(mPassenger[passenger].departure, mPassenger[passenger].arrival)
                               + getDistance(driverLocation[curr_driver], mPassenger[passenger].departure));
        
        driverLocation[curr_driver] = mPassenger[passenger].arrival;
        
        finished[passenger] = 1;
        
        cnt++;
        
        pQueue[curr_driver].init();
        
        bfs(curr_driver, mPassenger);
        
        for (int driver = 0; driver < M; driver++)
        {
            while (pQueue[driver].length > 0 && finished[pQueue[driver].arr[0].id])
            {
                pQueue[driver].pop();
            }
            
            if (pQueue[driver].length == 0)
            {
                bfs(driver, mPassenger);
            }
        }
        
    }
    
    
    int tc_score = 0;
    
    for (int driver = 0; driver < M; driver++)
    {
        assign_driver(driver, passengerSize[driver], passengerIDs[driver]);
        
        tc_score = max(tc_score, score[driver]);
    }
    
    printf("tc score: %d\n", tc_score);
    
    return;
}

