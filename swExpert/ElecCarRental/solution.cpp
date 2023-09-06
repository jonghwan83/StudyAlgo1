#define MAX_N 350
#define INF 350 * 350


struct Position{
    int row;
    int col;
};

struct Element{
    int row;
    int col;
    int distance;
    int battery;
};

class Queue {
public:
    int st, ed, length;
    Element arr[INF];

    void init() {
        st = ed = length = 0;
    }

    void push(int row, int col, int dist, int bat) {
        arr[ed] = { row, col, dist, bat };
        length++; ed++;
    }

    Element pop() {
        length--;
        return arr[st++];
    }
};

struct Node {
    int dest;
    int distance;
    Node* next;
};

class LinkedList {
public:
    int length;
    Node* head;

    void init() {
        length = 0;
        head = nullptr;
    }

    void push(int id, int dist) {
        Node* node = new Node();
        node->dest = id; node->distance = dist;
        
        node->next = head;
        head = node;
        length++;
    }
};

int wall = 202;
int road = 201;

int drows[4] = { -1, 0, 1, 0 };
int dcols[4] = { 0, -1, 0, 1 };


int minDistance[200];
LinkedList graph[200];

Position hashMap[200];
int map[MAX_N][MAX_N];
int battery;
int n, cnt;

Queue queue;

int visited[MAX_N][MAX_N];
int nVisited;

void bfs(int st) {
    nVisited++;

    queue.init();

    queue.push(hashMap[st].row, hashMap[st].col, 0, battery);
    visited[hashMap[st].row][hashMap[st].col] = nVisited;

    while (queue.length > 0) {
        Element curr = queue.pop();

        if (curr.battery == 0) { continue; }

        for (int i = 0; i < 4; i++) {
            int dr = curr.row + drows[i];
            int dc = curr.col + dcols[i];

            if (dr < 0 || dc < 0) { continue; }
            if (dr >= n || dc >= n) { continue; }
            if (map[dr][dc] == wall) { continue; }
            if (visited[dr][dc] >= nVisited) { continue; }

            queue.push(dr, dc, curr.distance + 1, curr.battery - 1);
            visited[dr][dc] = nVisited;

            if (map[dr][dc] < road) {
                graph[st].push(map[dr][dc], curr.distance + 1);
                graph[map[dr][dc]].push(st, curr.distance + 1);
            }
        }
    }

    return;
}

void dijkstra(int st, int dest) {
    queue.init();

    for (int i = 0; i < cnt; i++) {
        minDistance[i] = INF;
    }

    queue.push(hashMap[st].row, hashMap[st].col, 0, 0);
    minDistance[st] = 0;

    while (queue.length > 0) {
        Element curr = queue.pop();

        int id = map[curr.row][curr.col];
        if (curr.distance > minDistance[id]) { continue; }

        Node* node = graph[id].head;

        while (node) {
            if (minDistance[id] + node->distance < minDistance[node->dest]) {
                queue.push(hashMap[node->dest].row, hashMap[node->dest].col, minDistance[id] + node->distance, 0);
                minDistance[node->dest] = minDistance[id] + node->distance;

            }
        
            node = node->next;
        }
    }

    return;
}

void init(int N, int mRange, int mMap[MAX_N][MAX_N])

{
    for (int i = 0; i < 200; i++) {
        graph[i].init();
    }

    n = N;

    battery = mRange;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {

            if (mMap[r][c] == 1) {
                map[r][c] = wall;
            }
            else {
                map[r][c] = road;
            }
            visited[r][c] = 0;
        }
    }

    nVisited = 0;

    cnt = 0;

    return;

}



void add(int mID, int mRow, int mCol)

{

    hashMap[mID].row = mRow;
    hashMap[mID].col = mCol;

    map[mRow][mCol] = mID;

    bfs(mID);

    cnt++;

    return;

}



int distance(int mFrom, int mTo)

{

    dijkstra(mFrom, mTo);

    if (minDistance[mTo] == INF) {
        return -1;
    }

    return minDistance[mTo];

}
