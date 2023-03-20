#include <unordered_map>

using namespace std;

#define MAXCAR 70000
#define MAXTIME 300001

class HeapNode {
public:
    int totalWaiting;
    int wait;
    int idx;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXCAR];

    void init() { length = 0; }
    bool compare(int parent, int child) {
        if (arr[parent].totalWaiting < arr[child].totalWaiting) { return true; }
        if (arr[parent].totalWaiting == arr[child].totalWaiting && arr[parent].wait > arr[child].wait) { return true; }
        return false;
    }

    void push(int waiting, int w, int i) {
        HeapNode last = HeapNode();
        last.totalWaiting = waiting; last.wait = w; last.idx = i;

        int idx = length;
        arr[length++] = last;
        while((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    HeapNode pop() {
        HeapNode ans = arr[0];
        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;
        while (2 * idx + 1 < length) {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left;}
            }
            else { child = left; }

            if (compare(idx, child)) {
                HeapNode temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }
        return ans;
    }
};

class Car {
public:
    int id;
    int arrived;
    int parked;
    bool isParking;
    bool isWaiting;
    int totalWaiting;
    int totalParking;
    int ticket;

    Car() {
        arrived = 0;
        parked = 0;
        isParking = false;
        isWaiting = false;
        totalParking = 0;
        totalWaiting = 0;
        ticket = 0;
    }
};

int baseTime, baseFee, unitTime, unitFee, capa;
int nParking, nWaiting, waitNo;
Car cars[MAXCAR];
unordered_map<int, int> hashCar;
Heap waiting;
int cIdx;

int calculateFee(int mTime, int idx) {
    float parkingTime = (float) mTime - (float)cars[idx].parked;
    if (parkingTime <= baseTime) {
        return baseFee;
    }
    return baseFee + ceil((parkingTime - (float)baseTime) / unitTime) * unitFee;
}

void init(int mBaseTime, int mBaseFee, int mUnitTime, int mUnitFee, int mCapacity) {
    baseTime = mBaseTime;
    baseFee = mBaseFee;
    unitTime = mUnitTime;
    unitFee = mUnitFee;
    capa = mCapacity;

    hashCar.clear();
    nParking = 0;
    nWaiting = 0;
    cIdx = 0;
    waitNo = 0;
    waiting.init();

    return;
}

void push_car(int idx, int mTime, int mCar) {
    cars[idx].id = mCar;
    cars[idx].arrived = mTime;

    if (nParking < capa) {
        cars[idx].isParking = true;
        cars[idx].parked = mTime;
        nParking++;
    }
    else {
        cars[idx].isWaiting = true;
        cars[idx].ticket = waitNo;
        waiting.push(MAXTIME - mTime + cars[idx].totalWaiting - cars[idx].totalParking, waitNo++, idx);
        nWaiting++;
    }
}

int arrive(int mTime, int mCar) {
    unordered_map<int, int>::iterator itr = hashCar.find(mCar);
    if (itr == hashCar.end()) {
        cars[cIdx] = Car();
        hashCar[mCar] = cIdx;
        push_car(cIdx, mTime, mCar);
        cIdx++;
    }
    else {
        push_car(itr->second, mTime, mCar);
    }
    return nWaiting;
}

int leave(int mTime, int mCar) {
    unordered_map<int, int>::iterator itr = hashCar.find(mCar);
    if (cars[itr->second].isWaiting) {
        cars[itr->second].isWaiting = false;
        cars[itr->second].totalWaiting += mTime - cars[itr->second].arrived;
        nWaiting--;
        return -1;
    }

    int ans = calculateFee(mTime, itr->second);
    cars[itr->second].totalParking += mTime - cars[itr->second].parked;
    cars[itr->second].isParking = false;
    nParking--;

    HeapNode curr;
    while (waiting.length > 0) {
        curr = waiting.pop();
        if (cars[curr.idx].isWaiting && curr.wait == cars[curr.idx].ticket) {
            cars[curr.idx].totalWaiting += mTime - cars[curr.idx].arrived;
            cars[curr.idx].isWaiting = false;
            push_car(curr.idx, mTime, cars[curr.idx].id);
            nWaiting--;
            break;
        }
    }

    return ans;
}