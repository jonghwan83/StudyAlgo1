#define MAXLength 10000
#define MAXServer 10

class HeapNode {
public:
    int distance;
    int idx;

    HeapNode() { }
    HeapNode(int d, int i) {
        distance = d;
        idx =i;
    }
};

class Heap {
public:
    HeapNode arr[MAXLength];
    int length;
    bool isMaxDist;
    bool isMaxIdx;

    void init(bool isMaxDistance, bool isMaxIndex) {
        length = 0;
        isMaxDist = isMaxDistance;
        isMaxIdx = isMaxIndex;
    }

    bool compare(int parent, int child) {
        if (isMaxDist) {
            if (arr[parent].distance < arr[child].distance) { return true; }
        }
        else {
            if (arr[parent].distance > arr[child].distance) { return true; }
        }
        if (isMaxIdx) {
            if (arr[parent].distance == arr[child].distance && arr[parent].idx < arr[child].idx) { return true; }
        }
        else {
            if (arr[parent].distance == arr[child].distance && arr[parent].idx > arr[child].idx) { return true; }
        }

        return false;
    }

     void push(int d, int i) {
        HeapNode last = HeapNode(d, i);
        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
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
                else { child = left; }
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

class User {
public:
    int axis;
    int maxDistance;
    bool isRemoved;
    int server;
    int distance[MAXServer];

    void init() {
        maxDistance = 0;
        isRemoved = false;
        server = -1;
        for (int i = 0; i < MAXServer; i++) { distance[i] = 0; }
    }
};

class Server {
public:
    Heap assigned;
    int capa;
    int axis;
    int nUsers;

    void init() {
        nUsers = 0;
        assigned.init(false, true);
    }
};

int abs(int a) {
    if (a < 0) { return -a; }
    return a;
}

int min (int a , int b) {
    if (a > b) { return b; }
    return a;
}

int max (int a, int b) {
    if (a > b) { return a; }
    return b;
}

int totalServer, lineSize;
Server servers[MAXServer];
User users[MAXLength];
int visited[MAXServer];
int nDfs;
Heap waiting;
Heap closest;
HeapNode temp[MAXLength];

int getDistance(int uid, int sid) {
    return min(abs(servers[sid].axis - users[uid].axis),
               lineSize - max(servers[sid].axis, users[uid].axis) + min(servers[sid].axis, users[uid].axis));
}

void init(int L, int N, int C, int axis[MAXServer]) {
    waiting.init(true, false);
    totalServer = N;
    lineSize = L;

    for (int i = 0; i < N; i++) {
        servers[i].init();
        servers[i].axis = axis[i];
        servers[i].capa = C;
        visited[i] = 0;
    }
    nDfs = 1;
}

void push_user(int uid) {
    closest.init(false, false);

    for (int i = 0; i < totalServer; i++) {
        closest.push(users[uid].distance[i], i);
    }

    HeapNode currServer;
    while (closest.length > 0) {
        currServer = closest.pop();
        if (visited[currServer.idx] < nDfs) { break; }
    }

    servers[currServer.idx].assigned.push(users[uid].maxDistance, uid);
    servers[currServer.idx].nUsers++;
    users[uid].server = currServer.idx;
    visited[currServer.idx] = nDfs;

    if (servers[currServer.idx].nUsers > servers[currServer.idx].capa) {
        HeapNode currUser = servers[currServer.idx].assigned.pop();
        while (users[currUser.idx].isRemoved || users[currUser.idx].server != currServer.idx) {
            currUser = servers[currServer.idx].assigned.pop();
        }
        servers[currServer.idx].nUsers--;
        push_user(currUser.idx);
    }
}

int add_user(int uid, int axis) {
    users[uid].init();
    users[uid].axis = axis;

    for (int i = 0; i < totalServer; i++) {
        int dist = getDistance(uid, i);
        users[uid].distance[i] = dist;
        if (dist > users[uid].maxDistance) {
            users[uid].maxDistance = dist;
        }
    }

    push_user(uid);
    nDfs++;
    waiting.push(users[uid].maxDistance, uid);

    return users[uid].server;
}

void pop_user(int sid) {
    int tIdx = 0;

    HeapNode userInfo;
    bool addNewUser = false;
    while (waiting.length > 0) {
        userInfo = waiting.pop();
        if (users[userInfo.idx].isRemoved) { continue; }

        temp[tIdx++] = userInfo;
        if (users[userInfo.idx].server == sid) {
            continue;
        }

        int oldDist = getDistance(userInfo.idx, users[userInfo.idx].server);
        int newDist = getDistance(userInfo.idx, sid);

        if (newDist < oldDist) {
            addNewUser = true;
            break;
        }
        else if (newDist == oldDist && sid < users[userInfo.idx].server) {
            addNewUser = true;
            break;
        }
    }

    for (int i = 0; i < tIdx; i++) {
        waiting.push(temp[i].distance, temp[i].idx);
    }

    if (addNewUser) {
        int oldServer = users[userInfo.idx].server;
        servers[sid].assigned.push(users[userInfo.idx].maxDistance, userInfo.idx);
        servers[sid].nUsers++;
        users[userInfo.idx].server = sid;

        servers[oldServer].nUsers--;
        pop_user(oldServer);
    }

}

int remove_user(int uid) {
    users[uid].isRemoved = true;
    int ans = users[uid].server;
    servers[ans].nUsers--;

    pop_user(ans);

    return ans;
}

int get_users(int sid) {
    return servers[sid].nUsers;

}