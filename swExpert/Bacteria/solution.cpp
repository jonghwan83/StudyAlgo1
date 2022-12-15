#include <unordered_map>
#include <string>

#define MAX_BCNT 100
#define MAX_NAME 10

#define MAXBACTERIA 50000

using namespace std;

class Repository {
public:
    string name;
    int life;
    int storedTime;
    int count;

    Repository() {
        name = "";
        life = -1;
        storedTime = -1;
        count = 0;
    }
};

class Data {
public:
    int value;
    int id;

    Data() {}
    Data(int v, int i) {
        value = v;
        id = i;
    }
};

class Heap {
public:
    Data arr[MAXBACTERIA];
    int length;

    Heap() {
        length = 0;
    }

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (arr[parent].value > arr[child].value) { return true; }
        if (arr[parent].value == arr[child].value && arr[parent].id > arr[child].id) { return true; }
        return false;
    }

    void push(int val, int i) {
        Data last = Data(val, i);
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
        length--;
        arr[0] = arr[length];

        int idx = 0;
        Data temp;
        while (2 * idx + 1 < length) {
            int child;
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            if (right < length) {
                if (arr[right].value < arr[left].value) { child = right; }
                else if (arr[right].value == arr[left].value && arr[right].id < arr[left].id) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                temp = arr[child];
                arr[child] = arr[idx];
                arr[idx] = temp;
                idx = child;
            }
            else { break; }
        }
        return ans;
    }
};

class idxList {
public:
    int idx = 0;
    int arr[MAXBACTERIA];

    void addIdx(int index) {
        arr[idx] = index;
        idx++;
    }
};

unordered_map<string, int[2]> hashBacteria;
int bIdx, now;
Repository repo[MAXBACTERIA];
Heap nextHalfTime, minLife;


void updateTime(int tStamp) {
    while (now < tStamp) {
        now++;
        while (now == nextHalfTime.arr[0].value && nextHalfTime.length > 0) {
            Data curr = nextHalfTime.pop();
            int idx = curr.id;

            repo[idx].life /= 2;
            if (repo[idx].life < 10) { 
                hashBacteria[repo[idx].name][1] -= repo[idx].count;
                repo[idx].count = 0; 
            }
            else if (repo[idx].count > 0) {
                nextHalfTime.push(now + hashBacteria[repo[idx].name][0], idx);
                minLife.push(repo[idx].life, idx);
            }
        }
        if (nextHalfTime.length > 0 && nextHalfTime.arr[0].value <= tStamp) {
            now = nextHalfTime.arr[0].value - 1;
        }
        else {
            now = tStamp;
        }
    }
}

void init(int N, char bNameList[MAX_BCNT][MAX_NAME], int mHalfTime[MAX_BCNT])
{
    now = 0;
    bIdx = 0;
    hashBacteria.clear();

    for (int i = 0; i < N; i++) {
        hashBacteria[bNameList[i]][0] = mHalfTime[i];
        hashBacteria[bNameList[i]][1] = 0;
    }

    nextHalfTime.init();
    minLife.init();
}

void addBacteria(int tStamp, char bName[MAX_NAME], int mLife, int mCnt)
{
    updateTime(tStamp);

    repo[bIdx].name = bName;
    repo[bIdx].life = mLife;
    repo[bIdx].storedTime = tStamp;
    repo[bIdx].count = mCnt;

    hashBacteria[bName][1] += mCnt;

    nextHalfTime.push(tStamp + hashBacteria[bName][0], bIdx);
    minLife.push(mLife, bIdx);

    bIdx++;
}

int takeOut(int tStamp, int mCnt)
{
    updateTime(tStamp);

    int ans = 0;
    int k = 0;
    while (k < mCnt) {
        if (repo[minLife.arr[0].id].count == 0 || minLife.arr[0].value != repo[minLife.arr[0].id].life) {
            minLife.pop();
            continue;
        }
        if (repo[minLife.arr[0].id].count > mCnt - k) {
            repo[minLife.arr[0].id].count -= (mCnt - k);
            hashBacteria[repo[minLife.arr[0].id].name][1] -= (mCnt - k);
            ans += minLife.arr[0].value * (mCnt - k);
            k += (mCnt - k);
        }
        else {
            ans += minLife.arr[0].value * repo[minLife.arr[0].id].count;
            hashBacteria[repo[minLife.arr[0].id].name][1] -= repo[minLife.arr[0].id].count;
            k += repo[minLife.arr[0].id].count;
            repo[minLife.arr[0].id].count = 0;
            minLife.pop();
        }
    }
    return ans;
}

int checkBacteria(int tStamp, char bName[MAX_NAME])
{
    updateTime(tStamp);

    return hashBacteria[bName][1];
}