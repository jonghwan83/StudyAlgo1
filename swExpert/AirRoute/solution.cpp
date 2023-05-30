#define MAXAIRPORT 60
#define MAXEDGE 30000
#define INF 100000000

class Node {
public:
    int first;
    int second;
    int third;
};

class Heap {
public:
    int length;
    Node arr[MAXEDGE];

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].first > arr[child].first) { return true; }
        return false;
    }

    void push(int a, int b, int c) {
        Node last{};
        last.first = a; last.second = b; last.third = c;

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Node temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    Node pop() {
        Node ans = arr[0];
        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;

        while (idx * 2 + 1 < length) {
            left = idx * 2 + 1;
            right = idx * 2 + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
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
    Heap queue;
    queue.init();

    int minTravel[totalAirports];
    for (int i = 0; i < totalAirports; i++) {
        minTravel[i] = INF;
    }

    queue.push(0, st, sTime);
    minTravel[st] = 0;

    while (queue.length > 0) {
        Node curr = queue.pop();

        if (minTravel[curr.second] > curr.first) { continue; }

        for (int i = 0; i < airports[curr.second].length; i++) {
            int dest = airports[curr.second].next[i];

            int t_wait = convetTime(airports[curr.second].start[i] - curr.third);

            if (curr.first + airports[curr.second].time[i] + t_wait < minTravel[dest]) {
                minTravel[dest] = curr.first + airports[curr.second].time[i] + t_wait;
                if (dest != ed) {
                    queue.push(minTravel[dest], dest, convetTime(airports[curr.second].start[i] + airports[curr.second].time[i]));
                }
            }
        }
    }

    return minTravel[ed];
}


int getMinPrice(int st, int ed) {
    Heap queue;
    queue.init();

    int minCost[totalAirports];

    for (int i = 0; i < totalAirports; i++) {
        minCost[i] = INF;
    }

    queue.push(0, st, 0);
    minCost[st] = 0;

    while (queue.length > 0) {
        Node curr = queue.pop();

        if (minCost[curr.second] < curr.first) { continue; }

        for (int i = 0; i < airports[curr.second].length; i++) {
            int dest = airports[curr.second].next[i];

            if (minCost[curr.second] + airports[curr.second].price[i] < minCost[dest]) {
                minCost[dest] = minCost[curr.second] + airports[curr.second].price[i];
                if (dest != ed) {
                    queue.push(minCost[dest], dest, 0);
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
