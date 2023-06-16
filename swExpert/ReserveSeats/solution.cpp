struct Result {
    int id;
    int num;
};

#define MAXN 101
#define MAXTHEATERS 2001
#define MAXRESERVE 50001

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

class Heap {
public:
    int arr[MAXN];
    int length;

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent] > arr[child]) { return true; }
        return false;
    }

    void push(int a) {
        int idx = length;
        arr[length++] = a;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            int temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    int pop() {
        int ans = arr[0];
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
                int temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }
        return ans;
    }
};

class Queue {
public:
    int arr[MAXN];
    int length, st, ed;

    void init() {
        length = 0; st = 0; ed = 0;
    }

    void push(int a) {
        length++;
        arr[ed++] = a;
    }

    int pop() {
        length--;
        return arr[st++];
    }

    int front() {
        return arr[st];
    }
};

int around[4] = { -10, -1, 1, 10 };
int visited[MAXN];
int nVisited;

class Theater {
public:
    int seatMap[MAXN];
    int nSeat;
    int maxBundle;
    int minID;

    void init() {
        nSeat = 100;
        maxBundle = 100;
        minID = 1;
        for (int i = 0; i < MAXN; i++) {
            seatMap[i] = 0;
        }
    }

    Heap checkSeat(int st, int k) {

        Heap ans; ans.init();

        Heap queue; queue.init();

        queue.push(st);
        visited[st] = nVisited;

        while (queue.length > 0 && ans.length < k) {
            int curr = queue.pop();

            ans.push(curr);

            for (int i = 0; i < 4; i++) {
                int d = curr + around[i];

                if (d < 1 || d > 100) { continue; }
                if (curr % 10 == 0 && d % 10 == 1) { continue; }
                if (curr % 10 == 1 && d % 10 == 0) { continue; }

                if (seatMap[d] == 0) {
                    if (visited[d] < nVisited) {
                        visited[d] = nVisited;
                        queue.push(d);
                    }
                }
            }
        }

        return ans;
    }

};

class Reservation {
public:
    int theater;
    Queue seatIDs;

    void init() {
        seatIDs.init();
    }
};

int total;
Theater theaters[MAXTHEATERS];
Reservation reservations[MAXRESERVE];

void init(int N)
{
    nVisited = 0;

    for (int i = 0; i < MAXN; i++) {
        visited[i] = 0;
    }

    for (int i = 1; i <= N; i++) {
        theaters[i].init();
    }

    total = N;
}

Result reserveSeats(int mID, int K)
{
    Result res;
    res.id = 0;
    res.num = 0;

    for (int i = 1; i <= total; i++) {
        if (theaters[i].nSeat < K) { continue; }
        if (theaters[i].maxBundle < K) { continue; }
        res.id = i;
        break;
    }

    if (res.id == 0) { return res; }

    reservations[mID].init();
    reservations[mID].theater = res.id;

    Heap pQueue; pQueue.init();
    nVisited++;

    for (int i = theaters[res.id].minID; i < MAXN; i++) {
        if (theaters[res.id].seatMap[i] == 0) {
            pQueue.push(i);
        }
    }

    Queue temp; temp.init();

    while (pQueue.length > 0) {
        int st = pQueue.pop();

        if (visited[st] >= nVisited) { continue; }
        Heap seatIDs = theaters[res.id].checkSeat(st, K);

        if (seatIDs.length < K) {
            temp.push(st);
            continue;
        }

        theaters[res.id].nSeat -= seatIDs.length;

        while (seatIDs.length > 0) {
            int id = seatIDs.pop();

            theaters[res.id].seatMap[id] = mID;
            reservations[mID].seatIDs.push(id);
        }

        while (temp.length > 0) {
            pQueue.push(temp.pop());
        }
        break;
    }

    nVisited++;
    theaters[res.id].maxBundle = 0;
    theaters[res.id].minID = 101;

    while (pQueue.length > 0) {
        int st = pQueue.pop();

        if (visited[st] >= nVisited) { continue; }
        if (theaters[res.id].seatMap[st] > 0) { continue; }

        Heap seatIDs = theaters[res.id].checkSeat(st, 100);

        theaters[res.id].maxBundle = max(theaters[res.id].maxBundle, seatIDs.length);
        theaters[res.id].minID = min(theaters[res.id].minID, seatIDs.arr[0]);
    }

    res.num = reservations[mID].seatIDs.front();

    return res;
}

Result cancelReservation(int mID)
{
    Result res;
    res.id = 0;
    res.num = 0;

    nVisited++;

    res.id = reservations[mID].theater;
    int cnt = reservations[mID].seatIDs.length;
    theaters[res.id].minID = min(theaters[res.id].minID, reservations[mID].seatIDs.front());

    theaters[res.id].nSeat += cnt;

    while (reservations[mID].seatIDs.length > 0) {
        int id = reservations[mID].seatIDs.pop();

        if (theaters[res.id].seatMap[id] == mID) {
            res.num += id;
            visited[id] = nVisited;
            theaters[res.id].seatMap[id] = 0;
        }

        for (int i = 0; i < 4; i++) {
            int d = id + around[i];

            if (d < 1 || d > 100) { continue; }
            if (id % 10 == 0 && d % 10 == 1) { continue; }
            if (id % 10 == 1 && d % 10 == 0) { continue; }

            if (theaters[res.id].seatMap[d] == 0 && visited[d] < nVisited) {
                cnt++;
                visited[d] = nVisited;
                reservations[mID].seatIDs.push(d);
            }
        }
    }

    theaters[res.id].maxBundle = max(theaters[res.id].maxBundle, cnt);

    return res;
}