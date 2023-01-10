#include <unordered_map>
#include <string>
#include <cmath>

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
                if (compare(left, right)) { child = right; }
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

unordered_map<string, int[2]> hashBacteria;
int bIdx;
Repository repo[MAXBACTERIA];
Heap nextHalfTime, minLife;


void updateTime(int tStamp) {
    while (nextHalfTime.arr[0].value <= tStamp && nextHalfTime.length > 0) {
        Data curr = nextHalfTime.pop();
        repo[curr.id].life /= 2;

        if (repo[curr.id].life < 10) {
            hashBacteria[repo[curr.id].name][1] -= repo[curr.id].count;
            repo[curr.id].count = 0;
            continue;
        }
        nextHalfTime.push(curr.value + hashBacteria[repo[curr.id].name][0], curr.id);
        minLife.push(repo[curr.id].life, curr.id);
    }
}

void init(int N, char bNameList[MAX_BCNT][MAX_NAME], int mHalfTime[MAX_BCNT])
{
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
    repo[bIdx].name = bName;
    repo[bIdx].life = mLife;
    repo[bIdx].count = mCnt;
    repo[bIdx].storedTime = tStamp;

    hashBacteria[bName][1] += mCnt;

    minLife.push(mLife, bIdx);
    nextHalfTime.push(tStamp + hashBacteria[bName][0], bIdx);
    bIdx++;
}

int takeOut(int tStamp, int mCnt)
{
    updateTime(tStamp);
    int ans = 0;

    while (mCnt > 0 && minLife.length > 0) {
        Data curr = minLife.arr[0];
        if (repo[curr.id].life != curr.value) { 
            minLife.pop();
            continue; 
        }
        int cnt = min(repo[curr.id].count, mCnt);
        mCnt -= cnt;
        repo[curr.id].count -= cnt;
        hashBacteria[repo[curr.id].name][1] -= cnt;
        ans += repo[curr.id].life * cnt;

        if (repo[curr.id].count < 1) {
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
