#include <queue>

using namespace std;

const int MAX_DEVICE = 256;

const int MAX_ROOM = 52;



struct DetectedDevice

{

    int id;

    int power;

    int tx;

    int ty;

};

extern int scan_device(int mDeviceId, int mScanPower, DetectedDevice mDetected[]);





struct Device
{
    int id;

    int room;

    int scanPower;

    int numDetected;

    DetectedDevice detected[MAX_DEVICE];

    void search()
    {
        numDetected = scan_device(id, scanPower, detected);
    }
};

int currPower;

Device devices[MAX_DEVICE];

int deviceIdx;

int deviceCnt;

int cntInRoom[MAX_DEVICE];

priority_queue< pair<int, int> > scan_queue;

int add_device(int id)
{
    devices[deviceIdx].id = id;
    devices[deviceIdx].room = deviceIdx;
    devices[deviceIdx].scanPower = 0;
    devices[deviceIdx].numDetected = 0;
    return deviceIdx++;
}

int find_by_id(int id)
{
    for (int i = 0; i < deviceIdx; i++)
    {
        if (devices[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

int change_room(int oldRoom, int newRoom)
{
    int cnt = 0;
    for (int i = 0; i < deviceIdx; i++)
    {
        if (devices[i].room == oldRoom)
        {
            devices[i].room = newRoom;
        }
        if (devices[i].room == newRoom)
        {
            cnt++;
        }
    }
    return cnt;
}

void prepare_scan()
{
    if (!scan_queue.empty()) { return; }
    for (int i = 0; i < deviceIdx; i++)
    {
        scan_queue.push( { -cntInRoom[devices[i].room], -i } );
    }
}

void check_device(int idx)
{
    for (int i = 0; i < devices[idx].numDetected; i++)
    {
        DetectedDevice detected = devices[idx].detected[i];

        int newIdx = find_by_id(detected.id);

        if (newIdx == -1)
        {
            newIdx = add_device(detected.id);
        }

        int dist = abs(detected.ty) + abs(detected.tx);

        if (dist + detected.power == devices[idx].scanPower)
        {
            int newRoom = min(devices[idx].room, devices[newIdx].room);
            int oldRoom = max(devices[idx].room, devices[newIdx].room);

            deviceCnt -= cntInRoom[oldRoom];
            cntInRoom[oldRoom] = 0;
            int cnt = change_room(oldRoom, newRoom);
            deviceCnt -= cntInRoom[newRoom];
            cntInRoom[newRoom] = cnt;
            deviceCnt += cnt;
        }
    }

    if (cntInRoom[devices[idx].room] == 0 && devices[idx].scanPower == 200)
    {
        cntInRoom[devices[idx].room] = 1;
        deviceCnt++;
    }

}

void scan(int mDeviceId, int mTotalDevice)
{
    scan_queue = priority_queue< pair<int, int> >();

    for (int i = 0; i < mTotalDevice; i++)
    {
        cntInRoom[devices[i].room] = 0;
    }

    currPower = 200;

    deviceIdx = 0;

    deviceCnt = 0;

    int idx = add_device(mDeviceId);

    while (deviceCnt < mTotalDevice)
    {
        prepare_scan();

        pair<int, int> top = scan_queue.top();
        scan_queue.pop();

        if (-top.first != cntInRoom[devices[-top.second].room])
        {
            continue;
        }

        devices[-top.second].scanPower = currPower;
        devices[-top.second].search();

        check_device(-top.second);
    }

    return;

}

void result(int mDeviceIds[][MAX_DEVICE])
{
    scan_queue = priority_queue< pair<int, int> >();

    int hashRoom[MAX_DEVICE];
    int idxInRoom[MAX_DEVICE];
    int roomIdx = 0;

    for (int i = 0; i < deviceIdx; i++)
    {
        scan_queue.push( { -devices[i].id, i });
        hashRoom[i] = -1;
        idxInRoom[i] = 0;
    }



    while (!scan_queue.empty())
    {
        pair<int, int> top = scan_queue.top();
        scan_queue.pop();

        int device = top.second;

        if (hashRoom[devices[device].room] == -1)
        {
            hashRoom[devices[device].room] = roomIdx++;
        }

        int room = hashRoom[devices[device].room];
        mDeviceIds[room][idxInRoom[room]++] = devices[device].id;
    }

    return;

}