#include <vector>

#define MAXPASSENGERS 100000
#define MAXCAR 10
#define MAXJOB 1000

using namespace std;

class Passengers {
public:
    int point;
    int job;
    int car;
};

int totalPassenger, totalCar;
Passengers passengers[MAXPASSENGERS];
vector< vector<int> > hashJob(MAXJOB);

class Data {
public:
    int point;
    int id;
    Data() {}
    Data(int p, int i) {
        point = p;
        id = i;
    }
};


class Heap {
public:
    Data arr[MAXPASSENGERS];
    int length;
    bool isMinHeap;

    Heap() {
        length = 0;
        isMinHeap = false;
    }

    void init(bool isMin) {
        length = 0;
        isMinHeap = isMin;
    }

    bool compare(int parent, int child) {
        if (isMinHeap) {
            if (arr[parent].point > arr[child].point) { return true; }
            if (arr[parent].point == arr[child].point && arr[parent].id < arr[child].id) { return true; }
            return false;
        }
        else {
            if (arr[parent].point < arr[child].point) { return true; }
            if (arr[parent].point == arr[child].point && arr[parent].id > arr[child].id) { return true; }
            return false;
        }
    }

    void push(int point, int id) {
        Data last = Data(point, id);
        arr[length] = last;
        int idx = length;

        Data temp;
        while (idx > 0 && compare((idx - 1) / 2, idx)) {
            temp = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = arr[idx];
            arr[idx] = temp;
            idx = (idx - 1) / 2;
        }
        length++;
    }

    Data pop() {
        Data ans = arr[0];
        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;
        while (2 * idx + 1 < length) {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length) {
                if (compare(left, right)) { child = right;}
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

Heap minheap[MAXCAR];
Heap maxheap[MAXCAR];

void init(int N, int M, int J, int mPoint[], int mJobID[])
{
    for (int i = 0; i < J; i++) {
        hashJob[i].clear();
    }

    totalPassenger = N;
    totalCar = N / M;

    for (int i = 0; i < totalCar; i++) {
        minheap[i].init(true);
        maxheap[i].init(false);
    }

    for (int i = 0; i < N; i++) {
        passengers[i].point = mPoint[i];
        passengers[i].job = mJobID[i];
        passengers[i].car = i / M;
        hashJob[mJobID[i]].push_back(i);

        minheap[i / M].push(mPoint[i], i);
        maxheap[i / M].push(mPoint[i], i);
    }
}

void destroy()
{
}

int update(int mID, int mPoint)
{
    passengers[mID].point += mPoint;

    int car = passengers[mID].car;
    minheap[car].push(passengers[mID].point, mID);
    maxheap[car].push(passengers[mID].point, mID);

    return passengers[mID].point;
}

int updateByJob(int mJobID, int mPoint)
{
    int ans = 0;
    for (int idx : hashJob[mJobID]) {
        passengers[idx].point += mPoint;
        ans += passengers[idx].point;

        int car = passengers[idx].car;
        minheap[car].push(passengers[idx].point, idx);
        maxheap[car].push(passengers[idx].point, idx);
    }
    return ans;
}

int move(int mNum)
{
    int ans = 0;
    vector< vector< vector<int> > > maxTemp(totalCar);
    vector< vector< vector<int> > > minTemp(totalCar);

    for (int car = 0; car < totalCar; car++) {
        if (car > 0) {
            int k = 0;
            Data curr;
            while (k < mNum) {
                curr = maxheap[car].pop();
                int mID = curr.id;

                if (passengers[mID].point == curr.point && passengers[mID].car == car) {
                    ans += passengers[mID].point;
                    passengers[mID].car = car - 1;

                    maxTemp[car - 1].push_back({ passengers[mID].point, mID });
                    minTemp[car - 1].push_back({ passengers[mID].point, mID });

                    k++;
                }
            }
        }
        if (car < totalCar - 1) {
            int k = 0;
            Data curr;
            while (k < mNum) {
                curr = minheap[car].pop();
                int mID = curr.id;

                if (passengers[mID].point == curr.point && passengers[mID].car == car) {
                    ans += passengers[mID].point;
                    passengers[mID].car = car + 1;

                    minTemp[car + 1].push_back({ passengers[mID].point, mID });
                    maxTemp[car + 1].push_back({ passengers[mID].point, mID });

                    k++;
                }
            }
        }
    }

    for (int car = 0; car < totalCar; car++) {
        for (auto info : maxTemp[car]) {
            maxheap[car].push(info[0], info[1]);
        }
        for (auto info : minTemp[car]) {
            minheap[car].push(info[0], info[1]);
        }
    }

    return ans;
}