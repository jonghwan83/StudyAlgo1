#define MAXAIRPORT 60
#define MAXEDGE 30000
#define INF 100000000

class Node {
public:
    int first;
    int second;
    int third;
};

class Queue {
public:
    Node arr[MAXEDGE];
    int st, ed;
    int length;

    void init() {
        st = 0; ed = 0;
        length = 0;
    }

    void push(int a, int b, int c) {
        length++;
        arr[ed].first = a;
        arr[ed].second = b;
        arr[ed].third = c;
        ed++;
    }

    Node pop() {
        length--;
        return arr[st++];
    }
};

class Airport {
public:
    int length;
    int next[MAXEDGE];
    int price[MAXEDGE];
    int start[MAXEDGE];
    int time[MAXEDGE];

    void push(int nextAirport, int P, int D, int T) {
        next[length] = nextAirport;
        start[length] = D;
        price[length] = P;
        time[length] = T;
        length++;
    }
};

Airport airports[MAXAIRPORT];
int totalAirports;

int convetTime(int t) {
    if (t >= 24) { return t - 24; }
    if (t < 0) { return t + 24; }
    return t;
}

int getMinTravel(int st, int ed, int sTime) {
    Queue queue;
    queue.init();

    int minTravel[totalAirports];
    for (int i = 0; i < totalAirports; i++) {
        minTravel[i] = INF;
    }

    queue.push(st, sTime, 0);
    minTravel[st] = 0;

    while (queue.length > 0) {
        Node curr = queue.pop();

        for (int i = 0; i < airports[curr.first].length; i++) {
            int dest = airports[curr.first].next[i];

            int t_wait = convetTime(airports[curr.first].start[i] - curr.second);

            if (curr.third + airports[curr.first].time[i] + t_wait < minTravel[dest]) {
                minTravel[dest] = curr.third + airports[curr.first].time[i] + t_wait;
                if (dest != ed) {
                    queue.push(dest, convetTime(airports[curr.first].start[i] + airports[curr.first].time[i]), minTravel[dest]);
                }
            }
        }
    }

    return minTravel[ed];
}

int getMinPrice(int st, int ed) {
    Queue queue;
    queue.init();

    int minCost[totalAirports];

    for (int i = 0; i < totalAirports; i++) {
        minCost[i] = INF;
    }

    queue.push(st, 0, 0);
    minCost[st] = 0;

    while (queue.length > 0) {
        Node curr = queue.pop();

        for (int i = 0; i < airports[curr.first].length; i++) {
            int dest = airports[curr.first].next[i];

            if (minCost[curr.first] + airports[curr.first].price[i] < minCost[dest]) {
                minCost[dest] = minCost[curr.first] + airports[curr.first].price[i];
                if (dest != ed) {
                    queue.push(dest, 0, 0);
                }
            }
        }
    }

    return minCost[ed];
}

void init(int N) {
    totalAirports = N;

    for (int i = 0; i < N; i++) {
        airports[i].length = 0;
    }
}

void add(int mStartAirport, int mEndAirport, int mStartTime, int mTravelTime, int mPrice) {
    airports[mStartAirport].push(mEndAirport, mPrice, mStartTime, mTravelTime);
}

int minTravelTime(int mStartAirport, int mEndAirport, int mStartTime) {
    int ans = getMinTravel(mStartAirport, mEndAirport, mStartTime);

    if (ans == INF) { return - 1; }
    return ans;
}

int minPrice(int mStartAirport, int mEndAirport) {
    int ans = getMinPrice(mStartAirport, mEndAirport);

    if (ans == INF) { return -1; }
    return ans;
}