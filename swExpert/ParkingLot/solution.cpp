#include <unordered_map>
#include <queue>
#include <cmath>

#define MAXTIME 300001

using namespace std;

class Car {
public:
    int id;
    int parkingTime;
    int waitingTime;
    int totalParkingTime;
    int totalWaitingTime;
    bool isParking;
    bool isWaiting;
    int waitingNo;

    Car() {
        id = 0;
        parkingTime = 0;
        waitingTime = 0;
        totalWaitingTime = 0;
        totalParkingTime = 0;
        isParking = false;
        isWaiting = false;
        waitingNo = 0;
    }
};

int baseTime, baseFee, unitTime, unitFee, capacity, nParking, nWaiting, waitingNo;
unordered_map<int, Car> cars;
priority_queue< vector<int> > pQueue;

int calculateFee(int mTime, int mCar) {
    float parkingTime = (float) mTime - (float)cars[mCar].parkingTime;
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
    capacity = mCapacity;

    nParking = 0;
    nWaiting = 0;
    waitingNo = 0;

    cars.clear();
    pQueue = priority_queue< vector<int> >();

    return;
}

int arrive(int mTime, int mCar) {
    cars[mCar].id = mCar;

    if (nParking < capacity) {
        cars[mCar].isParking = true;
        cars[mCar].parkingTime = mTime;
        nParking++;
    }
    else {
        cars[mCar].isWaiting = true;
        cars[mCar].waitingTime = mTime;
        nWaiting++;

        pQueue.push({ MAXTIME - mTime + cars[mCar].totalWaitingTime - cars[mCar].totalParkingTime, --waitingNo, mCar });
        cars[mCar].waitingNo = waitingNo;
    }

    return nWaiting;
}

int leave(int mTime, int mCar) {
    int ans = 0;
    if (cars[mCar].isParking) {
        ans = calculateFee(mTime, mCar);

        cars[mCar].isParking = false;
        cars[mCar].totalParkingTime += mTime - cars[mCar].parkingTime;
        nParking--;
    }

    if (cars[mCar].isWaiting) {
        cars[mCar].isWaiting = false;
        cars[mCar].totalWaitingTime += mTime - cars[mCar].waitingTime;
        nWaiting--;
        return -1;
    }

    vector<int> car;
    while (pQueue.size() > 0) {
        car = pQueue.top();
        pQueue.pop();

        if (cars[car[2]].isWaiting && cars[car[2]].waitingNo == car[1]) {
            cars[car[2]].isWaiting = false;
            cars[car[2]].totalWaitingTime += mTime - cars[car[2]].waitingTime;
            cars[car[2]].isParking = true;
            cars[car[2]].parkingTime = mTime;
            nParking++;
            nWaiting--;
            break;
        }
    }

    return ans;
}