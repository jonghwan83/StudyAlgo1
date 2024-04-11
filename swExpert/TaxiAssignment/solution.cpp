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




int passengerSize[MAX_DRIVER_COUNT];

int passengerIDs[MAX_DRIVER_COUNT][MAX_PASSENGER_COUNT];

Coordinate driverLocation[MAX_DRIVER_COUNT];

int score[MAX_DRIVER_COUNT];

void run(int N, int M, Coordinate mDriver[], int K, Passenger mPassenger[])
{
    
    for (int driver = 0; driver < M; driver++)
    {
        passengerSize[driver] = 0;
        
        driverLocation[driver] = mDriver[driver];
        
        score[driver] = 0;
    }
    
    
    for (int passenger = 0; passenger < K; passenger++)
    {
        int min_dist = 4 * N;
        
        int close_driver = -1;
        
        for (int driver = 0; driver < M; driver++)
        {
            int dist = getDistance(mPassenger[passenger].departure, driverLocation[driver]);
            
            if (dist < min_dist)
            {
                min_dist = dist;
                close_driver = driver;
            }
        }
        
        passengerIDs[close_driver][passengerSize[close_driver]++] = passenger;
        
        driverLocation[close_driver] = mPassenger[passenger].arrival;
        
        score[close_driver] = max(score[close_driver], getDistance(mPassenger[passenger].departure, mPassenger[passenger].arrival));
    }
    
    
    int sum_score = 0;
    
    for (int driver = 0; driver < M; driver++)
    {
        assign_driver(driver, passengerSize[driver], passengerIDs[driver]);
        
        sum_score = max(sum_score, score[driver]);
    }
    
    printf("bottom line: %d\n", sum_score);
    
    return;
}

