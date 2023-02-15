#include <unordered_map>

#define MAXDEVICE 11000
#define SUBSIZE 1000

using namespace std;

class Heap {
public:
    int arr[SUBSIZE];
    int length;

    void init() { length = 0; }
    bool compare(int parent, int child) {
        if (arr[parent] > arr[child]) { return true; }
        else { return false; }
    }

    void push(int time) {
        int last = time;
        arr[length] = last;

        int idx = length;
        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            int temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
        length++;
    }
};

class Children {
public:
    int arr[SUBSIZE];
    int length;

    Children() { length = 0; }
    void push(int a) { arr[length++] = a; }
};

class Device {
public:
    bool isPC;
    int id;
    int parent;
    int isConnected;
    int time;
    int capa;
    int capacity;
    int nConnect;
    Children children;

    Device() {
        isPC = false;
        id = -1;
        parent = -1;
        isConnected = false;
        time = 0;
        capa = 0;
        capacity = 0;
        nConnect = 0;
        children = Children();
    }

    void init(bool pc, int mTime, int mParent, int mID) {
        isPC = pc;
        time = mTime;
        parent = mParent;
        isConnected = true;
        id = mID;
    }
};

struct Result {
    int finish;
    int param;
};

int dIdx, topHub;
Device devices[MAXDEVICE];
unordered_map<int, int> hashDevice;
Heap pQueue;
int pcList[SUBSIZE];
int pIdx;

void connectHub(int idx) {
    if (idx == 0 || devices[idx].nConnect > 0) {
        devices[idx].nConnect++;
        topHub = idx;
        return;
    }
    devices[idx].nConnect++;
    connectHub(devices[idx].parent);
}

void divideCapa(int idx) {
    if (devices[idx].children.length == 0 || devices[idx].nConnect == 0) { return; }

    for (int i = 0; i < devices[idx].children.length; i++) {
        devices[devices[idx].children.arr[i]].capa = devices[idx].capa / devices[idx].nConnect;
        divideCapa(devices[idx].children.arr[i]);
    }
}

void disconnectHub(int idx) {
    devices[idx].nConnect--;
    if (idx == 0 || devices[idx].nConnect > 0) {
        return;
    }
    devices[idx].isConnected = false;
    disconnectHub(devices[idx].parent);
}

void updateTime(int mTime) {
    while (true) {
        pQueue.init();
        for (int i = 0; i < pIdx; i++) {
            int pc = pcList[i];
            if (devices[pc].isConnected) {
                int amount = devices[pc].capa * (mTime - devices[pc].time);
                if (amount >= devices[pc].capacity) {
                    int expected = (devices[pc].capacity - amount) / devices[pc].capa;
                    pQueue.push(mTime + expected);
                }
            }
        }

        if (pQueue.length > 0) {
            int t = pQueue.arr[0];
            for (int i = 0; i < pIdx; i++) {
                int pc = pcList[i];
                if (devices[pc].isConnected) {
                    devices[pc].capacity -= devices[pc].capa * (t - devices[pc].time);
                    devices[pc].time = t;
                    if (devices[pc].capacity <= 0) {
                        devices[pc].isConnected = false;
                        devices[pc].time = t;
                        disconnectHub(devices[pc].parent);
                    }
                }
            }
        }
        else { break; }
        divideCapa(0);
    }

    for (int i = 0; i < pIdx; i++) {
        int pc = pcList[i];
        if (devices[pc].isConnected) {
            devices[pc].capacity -= devices[pc].capa * (mTime - devices[pc].time);
            devices[pc].time = mTime;
        }
    }
}



void init(int mCapa)
{
    hashDevice.clear();

    devices[0] = Device();
    dIdx = 1;

    devices[0].capa = mCapa;
    pIdx = 0;
}

void addHub(int mTime, int mParentID, int mID)
{
    devices[dIdx] = Device();
    hashDevice[mID] = dIdx;

    int parent = hashDevice[mParentID];
    devices[parent].children.push(dIdx);

    devices[dIdx++].init(false, mTime, parent, mID);
}

int removeHub(int mTime, int mID)
{
    updateTime(mTime);
    int idx = hashDevice[mID];
    if (devices[idx].nConnect > 0) {
        return 0;
    }
    return 1;
}

void requestDL(int mTime, int mParentID, int mpcID, int mSize)
{
    updateTime(mTime);

    devices[dIdx] = Device();
    hashDevice[mpcID] = dIdx;
    pcList[pIdx++] = dIdx;

    int parent = hashDevice[mParentID];
    devices[parent].children.push(dIdx);

    devices[dIdx].init(true, mTime, parent, mpcID);
    devices[dIdx].capacity = mSize;
    connectHub(dIdx);
    divideCapa(topHub);

    dIdx++;
}

Result checkPC(int mTime, int mpcID)
{
    Result res;
    res.finish = 0;
    res.param = 0;

    int idx = hashDevice[mpcID];
    updateTime(mTime);

    if (!devices[idx].isConnected) {
        res.finish = 1;
        res.param = devices[idx].time;
    }
    else { res.param = devices[idx].capacity; }
    return res;
}