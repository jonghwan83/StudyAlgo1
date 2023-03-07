#define MAXFRIEND 15
#define MAXMEMBER 20000
#define MAXSIZE 100

class Data {
public:
    int depth;
    int idx;
};

class Queue {
public:
    int st;
    int ed;
    int length;
    Data arr[MAXSIZE];

    Queue() {
        st = 0;
        ed = 0;
        length = 0;
    }

    void push(int nDepth, int mID) {
        Data last = Data();
        last.depth = nDepth; last.idx = mID;
        arr[ed++] = last;
        length++;
    }

    Data pop() {
        Data ans = arr[st++];
        length--;
        return ans;
    }
};

class HeapNode {
public:
    int influence;
    int idx;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXMEMBER * 3];

    void init() { length = 0; }
    bool compare(int parent, int child) {
        if (arr[parent].influence < arr[child].influence) { return true; }
        if (arr[parent].influence == arr[child].influence && arr[parent].idx > arr[child].idx) { return true; }
        return false;
    }

    void push(int inf, int id) {
        HeapNode last = HeapNode();
        last.influence = inf; last.idx = id;

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 < length && compare((idx - 1) / 2, idx)) {
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
                idx= child;
            }
            else { break; }
        }
        return ans;
    }
};

class Member {
public:
    int size;
    int friends[MAXFRIEND];
    int power;
    int influence;

    void init() {
        size = 0;
        influence = 0;
    }

    void push(int idx) {
        friends[size++] = idx;
    }
};

int parameters[4] = { 10, 5, 3, 2};
Member members[MAXMEMBER];
int visited[MAXMEMBER];
int nVisited;
Heap pQueue;
Queue initQueue;

void bfs(int idx) {
    Queue queue;
    queue.push(0, idx);
    visited[idx] = nVisited;

    while (queue.length > 0) {
        Data curr = queue.pop();
        members[idx].influence += parameters[curr.depth] * members[curr.idx].power;

        for (int i = 0; i < members[curr.idx].size; i++) {
            if (visited[members[curr.idx].friends[i]] < nVisited && curr.depth < 3) {
                visited[members[curr.idx].friends[i]] = nVisited;
                queue.push(curr.depth + 1, members[curr.idx].friends[i]);
            }
        }
    }
}

void init(int N, int mPurchasingPower[20000], int M, int mFriend1[20000], int mFriend2[20000])
{
    pQueue.init();
    nVisited = 1;
    for (int i = 0; i < N; i++) {
        members[i].init();
        members[i].power = mPurchasingPower[i];
        visited[i] = 0;
    }

    for (int i = 0; i < M; i++) {
        members[mFriend1[i]].push(mFriend2[i]);
        members[mFriend2[i]].push(mFriend1[i]);
    }

    for (int i = 0; i < N; i++) {
        bfs(i);
        nVisited++;
        pQueue.push(members[i].influence, i);
    }
}

int influencer(int mRank)
{
    HeapNode temp[MAXMEMBER];
    int tIdx = 0;

    HeapNode curr;
    while (mRank > 0) {
        curr = pQueue.pop();
        if (curr.influence == members[curr.idx].influence && visited[curr.idx] < nVisited) {
            mRank--;
            temp[tIdx++] = curr;
            visited[curr.idx] = nVisited;
        }
    }

    for (int i = 0; i < tIdx; i++) {
        pQueue.push(temp[i].influence, temp[i].idx);
    }

    nVisited++;
    return curr.idx;
}

int addPurchasingPower(int mID, int mPower)
{
    members[mID].power += mPower;

    Queue queue;
    queue.push(0, mID);
    visited[mID] = nVisited;

    while (queue.length > 0) {
        Data curr = queue.pop();
        members[curr.idx].influence += parameters[curr.depth] * mPower;
        pQueue.push(members[curr.idx].influence, curr.idx);

        for (int i = 0; i < members[curr.idx].size; i++) {
            if (visited[members[curr.idx].friends[i]] < nVisited && curr.depth < 3) {
                visited[members[curr.idx].friends[i]] = nVisited;
                queue.push(curr.depth + 1, members[curr.idx].friends[i]);
            }
        }
    }

    nVisited++;
    return members[mID].influence;
}

void initInfluence(int idx) {
    Queue queue;
    queue.push(0, idx);
    visited[idx] = nVisited;

    while (queue.length > 0) {
        Data curr = queue.pop();
        members[curr.idx].influence = 0;
        initQueue.push(curr.depth, curr.idx);

        for (int i = 0; i < members[curr.idx].size; i++) {
            if (visited[members[curr.idx].friends[i]] < nVisited && curr.depth < 3) {
                visited[members[curr.idx].friends[i]] = nVisited;
                queue.push(curr.depth + 1, members[curr.idx].friends[i]);
            }
        }
    }
}

int addFriendship(int mID1, int mID2)
{
    initQueue = Queue();
    members[mID1].push(mID2);
    members[mID2].push(mID1);

    initInfluence(mID1);
    nVisited++;
    while (initQueue.length > 0) {
        Data curr = initQueue.pop();
        bfs(curr.idx);
        nVisited++;
        pQueue.push(members[curr.idx].influence, curr.idx);
    }

    return members[mID1].influence + members[mID2].influence;
}