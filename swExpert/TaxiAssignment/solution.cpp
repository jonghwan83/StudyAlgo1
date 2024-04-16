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


Heap pQueue[MAX_DRIVER_COUNT];

bool finished[MAX_PASSENGER_COUNT];


int passengerSize[MAX_DRIVER_COUNT];

int passengerIDs[MAX_DRIVER_COUNT][MAX_PASSENGER_COUNT];

Coordinate driverLocation[MAX_DRIVER_COUNT];

int score[MAX_DRIVER_COUNT];

void run(int N, int M, Coordinate mDriver[], int K, Passenger mPassenger[])
{
    
    
//    for (int driver = 0; driver < M; driver++)
//    {
//        passengerSize[driver] = 0;
//        
//        driverLocation[driver] = mDriver[driver];
//        
//        score[driver] = 0;
//        
//        pQueue[driver].init();
//    }
    
    
    
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
            int wait_passenger = pQueue[driver].arr[0].id;
            
            int travel = getDistance(mPassenger[wait_passenger].departure, mPassenger[wait_passenger].arrival);
            
            if (pQueue[driver].arr[0].distance + score[driver] + travel < min_dist)
            {
                min_dist = pQueue[driver].arr[0].distance + score[driver] + travel;
                curr_driver = driver;
            }
        }
        
        
        Node curr = pQueue[curr_driver].pop();
        
        int passenger = curr.id;
        
        while (finished[passenger])
        {
            curr = pQueue[curr_driver].pop();
            
            passenger = curr.id;
        }
        
        passengerIDs[curr_driver][passengerSize[curr_driver]++] = passenger;
        
        score[curr_driver] += (getDistance(mPassenger[passenger].departure, mPassenger[passenger].arrival)
                                + getDistance(driverLocation[curr_driver], mPassenger[passenger].departure));
        
        driverLocation[curr_driver] = mPassenger[passenger].arrival;
        
        finished[passenger] = 1;
        
        cnt++;
        
        pQueue[curr_driver].init();
        
        for (int p = 0; p < K; p++)
        {
            if (!finished[p])
            {
                int dist = getDistance(driverLocation[curr_driver], mPassenger[p].departure);
                
                pQueue[curr_driver].push(dist, p);
            }
        }
        
    }
    
    
//    for (int passenger = 0; passenger < K; passenger++)
//    {
//        int min_dist = 4 * N;
//        
//        int close_driver = -1;
//        
//        int min_score = 4 * N * 100;
//        
//        for (int driver = 0; driver < M; driver++)
//        {
//            int dist = getDistance(mPassenger[passenger].departure, driverLocation[driver]);
//            
//            if (dist < min_dist && score[driver] == 0)
//            {
//                min_dist = dist;
//                close_driver = driver;
//            }
//            else if (dist < min_dist && score[driver] < min_score && score[driver] > 0)
//            {
//                min_dist = dist;
//                close_driver = driver;
//                min_score = score[driver];
//            }
//            
//        }
//        
//        passengerIDs[close_driver][passengerSize[close_driver]++] = passenger;
//        
//        score[close_driver] += (getDistance(mPassenger[passenger].departure, mPassenger[passenger].arrival)
//                                + getDistance(driverLocation[close_driver], mPassenger[passenger].departure));
//        
//        driverLocation[close_driver] = mPassenger[passenger].arrival;
//        
//    }
    
    
    int tc_score = 0;
    
    for (int driver = 0; driver < M; driver++)
    {
        assign_driver(driver, passengerSize[driver], passengerIDs[driver]);
        
        tc_score = max(tc_score, score[driver]);
    }
    
    printf("tc score: %d\n", tc_score);
    
    return;
}

