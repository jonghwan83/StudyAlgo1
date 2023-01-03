#include <deque>

#define MAXRIDER 2000
#define MAXCUSTOMER 500
#define MAXORDER 30000

using namespace std;

class Data {
public:
    int timestamp;
    int id;

    Data() {}
    Data(int d, int i) {
        timestamp = d;
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

    void push(int d, int i) {
        Data last = Data(d, i);
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
    int employee;
    int x;
    int y;
};

class Customer {
public:
    int x;
    int y;
};

int now;
Rider riders[MAXRIDER];
Customer customers[MAXCUSTOMER];
deque<int> employees;
Heap nearRiders;
Heap nextTimestamp;
Heap waitingOrder;

void updateTime(int mTimeStamp) {
    while (nextTimestamp.arr[0].timestamp <= mTimeStamp && nextTimestamp.length > 0) {
        Data curr = nextTimestamp.pop();
        if (riders[curr.id].status == 1) {
            int goal = riders[curr.id].targetId;
            riders[curr.id].x = customers[goal].x;
            riders[curr.id].y = customers[goal].y;
            riders[curr.id].status = 2;

            employees.push_back(riders[curr.id].employee);

            nextTimestamp.push(curr.timestamp + riders[curr.id].x + riders[curr.id].y, curr.id);
        }
        else if (riders[curr.id].status == 2) {
            riders[curr.id].status = 0;
            nearRiders.push(riders[curr.id].x + riders[curr.id].y, curr.id);

            if (waitingOrder.length > 0 && employees.size() > 0) {
                Data order = waitingOrder.pop();
                Data rider = nearRiders.pop();

                nextTimestamp.push(curr.timestamp + riders[rider.id].x + riders[rider.id].y, rider.id);
                riders[rider.id].status = 1;
                riders[rider.id].targetId = order.id;

                riders[rider.id].employee = employees.front();
                employees.pop_front();
                riders[rider.id].x = 0;
                riders[rider.id].y = 0;
            }
        }
    }

    now = mTimeStamp;
}

void init(int N, int U, int uX[], int uY[], int R, int rX[], int rY[]) {
    now = 0;

    nearRiders.init();
    waitingOrder.init();
    nextTimestamp.init();

    employees.clear();

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

    for (int i = 0; i < N; i++) {
        employees.push_back(i);
    }
}

int order(int mTimeStamp, int uID) {
    updateTime(mTimeStamp);

    if (nearRiders.length < 1 || employees.empty()) {
        waitingOrder.push(mTimeStamp, uID);
        return employees.size();
    }
    Data rider = nearRiders.pop();
    nextTimestamp.push(mTimeStamp + rider.timestamp, rider.id);
    riders[rider.id].status = 1;
    riders[rider.id].targetId = uID;

    riders[rider.id].employee = employees.front();
    employees.pop_front();
    riders[rider.id].x = 0;
    riders[rider.id].y = 0;

    return employees.size();

}

int checkWaitingRiders(int mTimeStamp) {
    updateTime(mTimeStamp);

    return nearRiders.length;
}