#include <string>
#include <unordered_map>

using namespace std;

#define MAXNODE 201
#define MAXSEARCH 5001

class HeapNode {
public:
    int distance;
    int idx;
};

class Heap {
public:
    HeapNode arr[MAXNODE];
    int length;

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].distance > arr[child].distance) { return true; }
        if (arr[parent].distance == arr[child].distance && arr[parent].idx > arr[child].idx) { return true; }
        return false;
    }

    void push(int dist, int i) {
        HeapNode last = HeapNode();
        last.distance = dist; last.idx = i;

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }
};

class User {
public:
    int next[MAXNODE];
    int length;
    int files;

    void init() {
        length = 0;
        files = 0;
    }

    void push_next(int a) {
        next[length++] = a;
    }
};

class Sharing {
public:
    string file;
    int filesize;
    int users[MAXNODE];
    int length;

    void init() {
        length = 0;
    }

    void push_user(int a) {
        users[length++] = a;
    }

    bool isDuplicated(int a) {
        for (int i = 0; i < length; i++) {
            if (a == users[i]) { return true; }
        }
        return false;
    }
};

class Req {
public:
    int route[MAXNODE];
    int length;
    int sharedIdx;
    int requestUser;

    void init() { length = 0; }
};

User users[MAXNODE];
int bandwidth[MAXNODE][MAXNODE];
int used[MAXNODE][MAXNODE];
Heap pQueue;
int routes[MAXNODE];

int visited[MAXNODE];
int nDfs;
bool escape;
int dist;

unordered_map<string, int> hashFile;
Sharing shared[MAXSEARCH];
int sIdx;

Req reqs[MAXSEARCH];

void getDistance(int st, int ed, int d, int mFilesize) {
    if (st == ed) {
        dist = d;
        escape = true;
        routes[d] = ed;
        return;
    }

    routes[d] = st;

    for (int i = 0; i < users[st].length; i++) {
        if (escape) { return; }

        if (visited[users[st].next[i]] < nDfs &&
            bandwidth[st][users[st].next[i]] - used[st][users[st].next[i]] >= mFilesize) {
            visited[users[st].next[i]] = nDfs;
            getDistance(users[st].next[i], ed, d + 1, mFilesize);
        }
    }

    return;
}

void init(int N, int mUID1[], int mUID2[], int mBandwidth[])
{
    for (int i = 1; i <= N; i++) {
        users[i].init();
        visited[i] = 0;

        for (int j = 0; j <= N; j++) {
            used[i][j] = 0;
        }
    }
    nDfs = 1;

    for (int i = 0; i < N; i++) {
        users[mUID1[i]].push_next(mUID2[i]);
        users[mUID2[i]].push_next(mUID1[i]);

        bandwidth[mUID1[i]][mUID2[i]] = mBandwidth[i];
        bandwidth[mUID2[i]][mUID1[i]] = mBandwidth[i];
    }

    sIdx = 0;
    hashFile.clear();
}

int share(int mUID, char mFilename[], int mFilesize)
{
    unordered_map<string, int>::iterator itr = hashFile.find(mFilename);
    if (itr != hashFile.end()) {
        if (shared[itr->second].isDuplicated(mUID)) { return users[mUID].files; }

        shared[itr->second].push_user(mUID);
        users[mUID].files++;
        return users[mUID].files;
    }

    shared[sIdx].init();
    hashFile[mFilename] = sIdx;
    shared[sIdx].file = mFilename;
    shared[sIdx].filesize = mFilesize;
    shared[sIdx].push_user(mUID);
    users[mUID].files++;

    sIdx++;
    return users[mUID].files;
}

int request(int mUID, char mFilename[], int mTID)
{
    unordered_map<string, int>::iterator itr = hashFile.find(mFilename);
    if (itr == hashFile.end()) { return -1; }

    pQueue.init();

    for (int i = 0; i < shared[itr->second].length; i++) {
        escape = false;
        visited[mUID] = nDfs;
        getDistance(mUID, shared[itr->second].users[i], 0, shared[itr->second].filesize);
        nDfs++;
        if (escape) {
            pQueue.push(dist, shared[itr->second].users[i]);
        }
    }

    if (pQueue.length == 0) { return -1; }

    escape = false;
    visited[mUID] = nDfs;
    getDistance(mUID, pQueue.arr[0].idx, 0, shared[itr->second].filesize);
    nDfs++;

    reqs[mTID].init();
    reqs[mTID].sharedIdx = itr->second;
    reqs[mTID].requestUser = mUID;
    reqs[mTID].length = dist + 1;

    for (int i = 0; i < dist; i++) {
        reqs[mTID].route[i] = routes[i];
        used[routes[i]][routes[i + 1]] += shared[itr->second].filesize;
        used[routes[i + 1]][routes[i]] += shared[itr->second].filesize;
    }
    reqs[mTID].route[dist] = routes[dist];

    return pQueue.arr[0].idx;
}

int complete(int mTID)
{
    for (int i = 0; i < reqs[mTID].length - 1; i++) {
        used[reqs[mTID].route[i]][reqs[mTID].route[i + 1]] -= shared[reqs[mTID].sharedIdx].filesize;
        used[reqs[mTID].route[i + 1]][reqs[mTID].route[i]] -= shared[reqs[mTID].sharedIdx].filesize;

        if (shared[reqs[mTID].sharedIdx].isDuplicated(reqs[mTID].route[i])) { continue; }
        shared[reqs[mTID].sharedIdx].push_user(reqs[mTID].route[i]);
        users[reqs[mTID].route[i]].files++;
    }

    return users[reqs[mTID].requestUser].files;
}