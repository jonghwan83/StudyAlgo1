#define MAXRIDER 2000
#define MAXCUSTOMER 500
#define MAXORDER 20000

class Data {
public:
    int timestamp;
    int id;

    Data() {}
    Data(int t, int i) {
        timestamp = t;
        id = i;
    }
};

class Heap {
public:
    Data arr[MAXORDER];
    int length;

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (arr[parent].timestamp > arr[child].timestamp) { return true; }
        if (arr[parent].timestamp == arr[child].timestamp && arr[parent].id > arr[child].id) { return true; }
        return false;
    }

    void push(int t, int i) {
        Data last = Data(t, i);
        arr[length] = last;
        int idx = length;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Data temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
        length++;
    }

    Data pop() {
        Data ans = arr[0];
        length--;
        arr[0] = arr[length];
        int idx = 0;

        int left, right, child;
        while (2 * idx + 1 < length) {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                Data temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }
        return ans;
    }
};

class Rider {
public:
    int status;
    int targetId;
    int x;
    int y;
};

class Customer {
public:
    int x;
    int y;
};

Rider riders[MAXRIDER];
Customer customers[MAXCUSTOMER];
int employees, now;
Heap nearRiders;
Heap nextTimestamp;
Heap waitingOrder;

void checkDelivery() {
    while (nextTimestamp.length > 0 && nextTimestamp.arr[0].timestamp <= now) {
        Data curr = nextTimestamp.pop();
        if (riders[curr.id].status == 1) {
            int target = riders[curr.id].targetId;
            riders[curr.id].status = 2;
            riders[curr.id].x = customers[target].x;
            riders[curr.id].y = customers[target].y;

            nextTimestamp.push(curr.timestamp + customers[target].x + customers[target].y, curr.id);
            employees++;
        }
        else if (riders[curr.id].status == 2) {
            riders[curr.id].status = 0;
            riders[curr.id].targetId = -1;
            nearRiders.push(riders[curr.id].x + riders[curr.id].y, curr.id);
        }
    }
}

void assignDelivery() {
    while (waitingOrder.arr[0].timestamp <= now && waitingOrder.length > 0 && employees > 0 && nearRiders.length > 0) {
        Data order = waitingOrder.pop();
        Data rider = nearRiders.pop();

        riders[rider.id].status = 1;
        riders[rider.id].targetId = order.id;
        riders[rider.id].x = 0;
        riders[rider.id].y = 0;

        employees--;
        nextTimestamp.push(now + rider.timestamp, rider.id);
    }
}

int min(int a, int b) {
    if (a < b) { return  a; }
    else { return b; }
}

void updateTime(int mTimeStamp) {
    while (now < mTimeStamp) {
        now++;
        checkDelivery();
        assignDelivery();

        int t = 40000001;
        if (now < waitingOrder.arr[0].timestamp && waitingOrder.length > 0) {
            t = min(waitingOrder.arr[0].timestamp, t);
        }
        if (now < nextTimestamp.arr[0].timestamp && nextTimestamp.length > 0) {
            t = min(nextTimestamp.arr[0].timestamp, t);
        }
        if (t > mTimeStamp) { now = mTimeStamp; }
        else { now = t - 1; }
    }
}

void init(int N, int U, int uX[], int uY[], int R, int rX[], int rY[]) {
    now = 0;

    nearRiders.init();
    waitingOrder.init();
    nextTimestamp.init();

    employees = N;
    for (int i = 0; i < R; i++) {
        riders[i].status = 0;
        riders[i].targetId = -1;
        riders[i].x = rX[i];
        riders[i].y = rY[i];
        nearRiders.push(rX[i] + rY[i], i);
    }

    for (int i = 0; i < U; i++) {
        customers[i].x = uX[i];
        customers[i].y = uY[i];
    }
}

int order(int mTimeStamp, int uID) {
    updateTime(mTimeStamp - 1);
    now++;
    waitingOrder.push(mTimeStamp, uID);
    checkDelivery();
    assignDelivery();

    return employees;
}

int checkWaitingRiders(int mTimeStamp) {
    updateTime(mTimeStamp);

    return nearRiders.length;
}