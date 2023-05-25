#define MAXCIVIL 30000
#define MAXN 1000

class Node {
public:
    int r;
    int c;

    Node* next;
};

class LinkedList {
public:
    int id;
    bool isRemoved;
    int length;
    Node* head;
    Node* tail;

    void init() {
        id = -1;
        isRemoved = false;
        length = 0;
        head = nullptr;
        tail = nullptr;
    }

    void push(int row, int col) {
        Node* node = new Node();
        node->r = row; node->c = col;

        node->next = head;
        head = node;
        if (!tail) { tail = node; }
        length++;
    }
};

struct Location {
    int idx;
    int i;
};

class HashChain {
public:
    int length[MAXN];
    int id[MAXN][MAXCIVIL];
    int idx[MAXN][MAXCIVIL];

    void init() {
        for (int i = 0; i < MAXN; i++) {
            length[i] = 0;
        }
    }

    void push(int mID, int mIdx) {
        int key = mID % MAXN;
        id[key][length[key]] = mID;
        idx[key][length[key]] = mIdx;

        length[key]++;
    }

    Location find(int mID) {
        Location ans;
        ans.idx = -1; ans.i = -1;

        int key = mID % MAXN;
        for (int i = 0; i < length[key]; i++) {
            if (id[key][i] == mID) {
                ans.idx = idx[key][i];
                ans.i = i;
                return ans;
            }
        }

        return ans;
    }
};


int gridSize;
HashChain hashIdx;
LinkedList civilization[MAXCIVIL];
int grid[MAXN][MAXN];
int cIdx;

int drows[4] = { -1, 1, 0, 0 };
int dcols[4] = { 0, 0, -1, 1 };

void assembleLink(int idx1, int idx2) {
    Node* node = civilization[idx2].head;

    while (node) {
        grid[node->r][node->c] = idx1;
        node = node->next;
    }

    civilization[idx1].tail->next = civilization[idx2].head;
    civilization[idx1].tail = civilization[idx2].tail;
    civilization[idx1].length += civilization[idx2].length;

    civilization[idx2].length = 0;
    civilization[idx2].head = nullptr;
    civilization[idx2].tail = nullptr;
}

int getNeighbor(int r, int c) {
    int dr, dc;

    struct Neighbor {
        int idx[4];
        int cnt[4];
        int length;

        Neighbor() { length = 0; }

        int isInclusive(int mID) {
            for (int i = 0; i < length; i++) {
                if (idx[i] == mID) { return i; }
            }
            return -1;
        }
    };

    Neighbor neighbors;

    for (int i = 0; i < 4; i++) {
        dr = r + drows[i];
        dc = c + dcols[i];

        if (dr < 0 || dc < 0) { continue; }
        if (dr >= gridSize || dc >= gridSize) { continue; }

        if (grid[dr][dc] > -1) {
            int idx = neighbors.isInclusive(grid[dr][dc]);
            if (idx > -1) {
                neighbors.cnt[idx]++;
            }
            else {
                neighbors.idx[neighbors.length] = grid[dr][dc];
                neighbors.cnt[neighbors.length] = 1;
                neighbors.length++;
            }
        }
    }

    int neighbor = MAXCIVIL + 1;
    int count = 0;

    for (int i = 0; i < neighbors.length; i++) {
        if (neighbors.cnt[i] > count) {
            count = neighbors.cnt[i];
            neighbor = neighbors.idx[i];
        }
        else if (count == neighbors.cnt[i] && civilization[neighbors.idx[i]].id < civilization[neighbor].id) {
            neighbor = neighbors.idx[i];
        }
    }

    if (neighbor == MAXCIVIL + 1) { return -1; }

    return neighbor;
}

void init(int N) {
    hashIdx.init();

    cIdx = 0;

    gridSize = N;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = -1;
        }
    }
}

int newCivilization(int r, int c, int mID) {
    r--; c--;

    int neighbor = getNeighbor(r, c);

    if (neighbor == -1) {
        int idx = hashIdx.find(mID).idx;

        if (idx == -1) {
            civilization[cIdx].init();

            civilization[cIdx].id = mID;
            civilization[cIdx].isRemoved = false;

            civilization[cIdx].push(r, c);
            grid[r][c] = cIdx;

            hashIdx.push(mID, cIdx);

            cIdx++;
        }
        else {
            civilization[idx].isRemoved = false;

            civilization[idx].push(r, c);
            grid[r][c] = idx;
        }

        return mID;
    }

    civilization[neighbor].push(r, c);
    grid[r][c] = neighbor;

    return civilization[neighbor].id;
}

int removeCivilization(int mID) {
    int idx = hashIdx.find(mID).idx;

    if (idx == -1) { return 0; }
    if (civilization[idx].isRemoved) { return 0; }

    civilization[idx].isRemoved = true;
    int ans = civilization[idx].length;

    Node* node = civilization[idx].head;

    while (node) {
        grid[node->r][node->c] = -1;
        node = node->next;
    }

    civilization[idx].length = 0;
    civilization[idx].head = nullptr;
    civilization[idx].tail = nullptr;

    return ans;
}

int getCivilization(int r, int c) {
    r--; c--;
    int idx = grid[r][c];

    if (idx == -1) { return 0; }
    if (civilization[idx].isRemoved) { return 0; }

    return civilization[idx].id;
}

int getCivilizationArea(int mID) {
    int idx = hashIdx.find(mID).idx;

    if (idx == -1) { return 0; }
    if (civilization[idx].isRemoved) { return 0; }

    return civilization[idx].length;
}

int mergeCivilization(int mID1, int mID2) {
    Location idx1 = hashIdx.find(mID1);
    Location idx2 = hashIdx.find(mID2);

    if (civilization[idx2.idx].length > civilization[idx1.idx].length) {
        assembleLink(idx2.idx, idx1.idx);

        int key1 = mID1 % MAXN;
        int key2 = mID2 % MAXN;

        hashIdx.idx[key1][idx1.i] = idx2.idx;
        hashIdx.idx[key2][idx2.i] = idx1.idx;

        int temp = civilization[idx2.idx].id;
        civilization[idx2.idx].id = civilization[idx1.idx].id;
        civilization[idx1.idx].id = temp;

        return civilization[idx2.idx].length;
    }
    else {
        assembleLink(idx1.idx, idx2.idx);
        return civilization[idx1.idx].length;
    }
}

