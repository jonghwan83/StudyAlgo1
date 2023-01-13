#include <deque>
#include <vector>

#define MAXNODE 30000
#define MAXAIRPORT 60
#define INF 1000000

using namespace std;

class Airport {
public:
    int next[MAXNODE];
    int price[MAXNODE];
    int travelTime[MAXNODE];
    int departTime[MAXNODE];
    int nIdx;

    void clear() {
        nIdx = 0;
    }

    void add(int mEndAirport, int mStartTime, int mTravelTime, int mPrice) {
        next[nIdx] = mEndAirport;
        departTime[nIdx] = mStartTime;
        travelTime[nIdx] = mTravelTime;
        price[nIdx] = mPrice;
        nIdx++;
    }
};


int totalAirport;
Airport airports[MAXAIRPORT];
int minCost[MAXAIRPORT];
int minTravel[MAXAIRPORT];

void getMinTravel(int mStartAirport, int mStartTime, int mEndAirport) {
    for (int i = 0; i < totalAirport; i++) { minTravel[i] = INF; }

    deque< vector<int> > queue;

    queue.push_back({mStartAirport, mStartTime, 0});
    minTravel[mStartAirport] = 0;

    while (!queue.empty()) {
        vector<int> curr = queue.front();
        queue.pop_front();

        for (int i = 0; i < airports[curr[0]].nIdx; i++) {
            int waitTime = airports[curr[0]].departTime[i] - curr[1];
            if (waitTime < 0) { waitTime += 24; }
            int t = waitTime + curr[2] + airports[curr[0]].travelTime[i];
            
            if (minTravel[airports[curr[0]].next[i]] > t) {
                minTravel[airports[curr[0]].next[i]] = t;
                int nextTime = airports[curr[0]].departTime[i] + airports[curr[0]].travelTime[i];
                if (nextTime >= 24) { nextTime -= 24; }
                if (airports[curr[0]].next[i] != mEndAirport) {
                    queue.push_back({airports[curr[0]].next[i], nextTime, t});
                }
            }
        }
    }
}

void getMinCost(int mStartAirport, int mEndAirport) {
    for (int i = 0; i < totalAirport; i++) {
        minCost[i] = INF;
    }

    deque<int> queue;

    queue.push_back(mStartAirport);
    minCost[mStartAirport] = 0;

    while (!queue.empty()) {
        int curr = queue.front();
        queue.pop_front();
        
        for (int i = 0; i < airports[curr].nIdx; i++) {
            if (minCost[airports[curr].next[i]] > minCost[curr] + airports[curr].price[i]) {
                minCost[airports[curr].next[i]] = minCost[curr] + airports[curr].price[i];
                if (airports[curr].next[i] != mEndAirport) {
                    queue.push_back(airports[curr].next[i]);
                }
            }
        }
    }
}

void init(int N) {
    totalAirport = N;
    for (int i = 0; i < N; i++) {
        airports[i].clear();
    }
}

void add(int mStartAirport, int mEndAirport, int mStartTime, int mTravelTime, int mPrice) {
    airports[mStartAirport].add(mEndAirport, mStartTime, mTravelTime, mPrice);
}

int minTravelTime(int mStartAirport, int mEndAirport, int mStartTime) {
    getMinTravel(mStartAirport, mStartTime, mEndAirport);

    if (minTravel[mEndAirport] < INF) {
        return minTravel[mEndAirport];
    }

    return -1;
}

int minPrice(int mStartAirport, int mEndAirport) {
    getMinCost(mStartAirport, mEndAirport);

    if (minCost[mEndAirport] < INF) {
        return minCost[mEndAirport];
    }
    return -1;
}
