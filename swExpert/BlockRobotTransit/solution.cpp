
#define MAXN 20
#define OFFSET 10


int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

struct Element {
    int row;
    int col;
};

class Queue {
public:
    int length, st, ed;
    Element arr[MAXN * MAXN];

    void init() {
        length = 0; st = 0; ed = 0;
    }

    void push(int r, int c) {
        arr[ed].row = r; arr[ed].col = c;
        ed++; length++;
    }

    Element pop() {
        length--;
        return arr[st++];
    }
};


struct Node {
    int row;
    int col;
    int dir;
    Node* next;
};

class LinkedList {
public:
    int length;
    Node* head;

    void init() {
        length = 0; head = nullptr;
    }

    void push(int r, int c, int d) {
        Node* node = new Node();
        node->row = r; node->col = c; node->dir = d;

        node->next = head;
        head = node;
        length++;
    }
};

struct HashKey {
    int key[4];

    HashKey() {
        for (int i = 0; i < 4; i++) {
            key[i] = 0;
        }
    }
};


int n;
LinkedList hashStructure[MAXN][MAXN][MAXN][MAXN];
int island[MAXN][MAXN];

int nVisited;
int visited[MAXN][MAXN];

int drows[4] = { -1, 0, 1, 0 };
int dcols[4] = { 0, -1, 0, 1 };

void install(int M, int mStructure[], int row, int col, int dir) {

    for (int i = 0; i < M; i++) {
        island[row + drows[dir] * i][col + dcols[dir] * i] += mStructure[i];
    }
}

void uninstall(int M, int mStructure[], int row, int col, int dir) {

    for (int i = 0; i < M; i++) {
        island[row + drows[dir] * i][col + dcols[dir] * i] -= mStructure[i];
    }
}

int simulate(int mDir) {

    nVisited++;

    int ans = 0;

    Queue queue; queue.init();

    for (int i = 0; i < n; i++) {
        if (mDir == 0) {
            queue.push(0, i); visited[0][i] = nVisited;
        }
        else if (mDir == 1) {
            queue.push(i, n - 1); visited[i][n - 1] = nVisited;
        }
        else if (mDir == 2) {
            queue.push(n - 1, i); visited[n - 1][i] = nVisited;
        }
        else {
            queue.push(i, 0); visited[i][0] = nVisited;
        }
    }

    while (queue.length > 0) {
        Element curr = queue.pop();

        for (int i = 0; i < 4; i++) {
            int dr = curr.row + drows[i];
            int dc = curr.col + dcols[i];

            if (dr < 0 || dc < 0) { continue; }
            if (dr >= n || dc >= n) { continue; }
            if (visited[dr][dc] >= nVisited) { continue; }
            if (island[dr][dc] < island[curr.row][curr.col]) { continue; }

            queue.push(dr, dc); visited[dr][dc] = nVisited;
            ans++;

        }
    }

    return n * n - n - ans;
}



bool isSymmetric(int M, int mStructure[]) {

    for (int i = 0; i < M / 2; i++) {
        if (mStructure[i] != mStructure[M - 1 - i]) {
            return false;
        }
    }

    return true;
}

void initHash() {

    for (int i = 0; i < MAXN; i++) {
        for (int j = 0; j < MAXN; j++) {
            for (int k = 0; k < MAXN; k++) {
                for (int l = 0; l < MAXN; l++) {
                    hashStructure[i][j][k][l].init();
                }
            }
        }
    }
}

void init(int N, int mMap[MAXN][MAXN])
{
    nVisited = 0;

    n = N;

    initHash();

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {

            island[r][c] = mMap[r][c];
            visited[r][c] = 0;

            for (int i = 0; i < 4; i++) {

                HashKey hash;

                for (int j = 1; j < 5; j++) {

                    int dr = r + drows[i] * j;
                    int dc = c + dcols[i] * j;

                    if (dc >= n || dr >= n) { continue; }
                    if (dc < 0 || dr < 0) { continue; }

                    hash.key[j - 1] = mMap[r][c] - mMap[dr][dc] + OFFSET;
                    hashStructure[hash.key[0]][hash.key[1]][hash.key[2]][hash.key[3]].push(r, c, i);
                }

            }

        }
    }


}

int numberOfCandidate(int M, int mStructure[])
{
    bool symmetric = isSymmetric(M, mStructure);

    HashKey fwd;

    for (int i = 1; i < M; i++) {
        fwd.key[i - 1] = mStructure[i] - mStructure[0] + OFFSET;
    }

    int ans = hashStructure[fwd.key[0]][fwd.key[1]][fwd.key[2]][fwd.key[3]].length;

    if (symmetric) { ans /= 2; }

    return ans;
}



int maxBlockedRobots(int M, int mStructure[], int mDir)
{
    HashKey fwd;

    for (int i = 1; i < M; i++) {
        fwd.key[i - 1] = mStructure[i] - mStructure[0] + OFFSET;
    }

    int ans = 0;

    Node* node = hashStructure[fwd.key[0]][fwd.key[1]][fwd.key[2]][fwd.key[3]].head;

    while (node) {
        install(M, mStructure, node->row, node->col, node->dir);

        ans = max(ans, simulate(mDir));

        uninstall(M, mStructure, node->row, node->col, node->dir);

        node = node->next;
    }

    return ans;
}
