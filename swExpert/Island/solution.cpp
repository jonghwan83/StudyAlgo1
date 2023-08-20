
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
    Node *next;
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

int drows[4] = { 0, 1, 0, -1 };
int dcols[4] = { 1, 0, -1, 0 };

void install(int M, int mStructure[5], int row, int col, int dir) {

    for (int i = 0; i < M; i++) {
        island[row + drows[dir] * i][col + dcols[dir] * i] += mStructure[i];
    }
}

void uninstall(int M, int mStructure[5], int row, int col, int dir) {

    for (int i = 0; i < M; i++) {
        island[row + drows[dir] * i][col + dcols[dir] * i] -= mStructure[i];
    }
}

int simulate(int mSeaLevel) {
    nVisited++;

    int submerged = 0;

    Queue queue; queue.init();

    for (int r = 0; r < n; r++) {

        if (island[r][0] < mSeaLevel && visited[r][0] < nVisited) {
            queue.push(r, 0);
            visited[r][0] = nVisited; submerged++;
        }

        if (island[r][n - 1] < mSeaLevel && visited[r][n - 1] < nVisited) {
            queue.push(r, n - 1);
            visited[r][n - 1] = nVisited; submerged++;
        }
    }

    for (int c = 0; c < n; c++) {

        if (island[0][c] < mSeaLevel && visited[0][c] < nVisited) {
            queue.push(0, c);
            visited[0][c] = nVisited; submerged++;
        }

        if (island[n - 1][c] < mSeaLevel && visited[n - 1][c] < nVisited) {
            queue.push(n - 1, c);
            visited[n - 1][c] = nVisited; submerged++;
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

            if (island[dr][dc] < mSeaLevel) {
                queue.push(dr, dc);
                visited[dr][dc] = nVisited; submerged++;
            }

        }

    }

    return n * n - submerged;
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


void init(int N, int mMap[20][20])
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

int numberOfCandidate(int M, int mStructure[5])
{
    if (M == 1) { return n * n; }

    bool symmetric = isSymmetric(M, mStructure);

    HashKey fwd;

    for (int i = 1; i < M; i++) {
        fwd.key[i - 1] = mStructure[i] - mStructure[0] + OFFSET;
    }

    int ans = hashStructure[fwd.key[0]][fwd.key[1]][fwd.key[2]][fwd.key[3]].length;

    if (symmetric) { ans /= 2; }

    return ans;
}

int maxArea(int M, int mStructure[5], int mSeaLevel)
{

    int ans = 0;

    if (M == 1) {
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {

                install(M, mStructure, r, c, 0);

                ans = max(ans, simulate(mSeaLevel));

                uninstall(M, mStructure, r, c, 0);
            }
        }

        return ans;
    }


    HashKey fwd;

    for (int i = 1; i < M; i++) {
        fwd.key[i - 1] = mStructure[i] - mStructure[0] + OFFSET;
    }

    if (hashStructure[fwd.key[0]][fwd.key[1]][fwd.key[2]][fwd.key[3]].length < 1) { return -1; }

    Node* node = hashStructure[fwd.key[0]][fwd.key[1]][fwd.key[2]][fwd.key[3]].head;

    while (node) {
        install(M, mStructure, node->row, node->col, node->dir);

        ans = max(ans, simulate(mSeaLevel));

        uninstall(M, mStructure, node->row, node->col, node->dir);
        node = node->next;
    }

    return ans;
}
