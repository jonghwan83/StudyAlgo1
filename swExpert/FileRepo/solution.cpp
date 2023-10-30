#define SUBN 5000

#define MAXFILE 12000

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

struct Range {
    int left;
    int right;
};

class Heap {
public:
    int length;
    Range arr[10000];

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].left > arr[child].left) { return true; }
        return false;
    }

    void push(int st, int ed) {
        Range node = { st, ed };

        int idx = length;
        arr[length++] = node;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Range temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    Range pop() {
        Range ans = arr[0];
        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;

        while (2 * idx + 1 < length)
        {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                Range temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }

        return ans;
    }
};



struct Node
{
    int key;
    int value;
    Node* next;
};

class LinkedList {
public:
    Node* head;

    void init() {
        head = nullptr;
    }

    void push(int k, int v) {
        Node* node = new Node();
        node->key = k;
        node->value = v;
        node->next = head;

        head = node;
    }
};

class File {
public:
    int size;
    int id;
    Range piece[30];
    int pIdx;

    void init() {
        pIdx = 0;
        size = 0;
    }

    void push(Range p) {
        if (piece[pIdx - 1].right == p.left)
        {
            piece[pIdx - 1].right = p.right;
        }
        else {
            piece[pIdx++] = p;
        }
    }
};

LinkedList hashID[SUBN];

Heap empties;

File files[MAXFILE];

int fIdx;

int space;

Range isOverlap(Range a, Range b)
{
    Range ans;

    ans.left = max(a.left, b.left);
    ans.right = min(a.right, b.right);

    return ans;
}

void pushByID(int mID, int i)
{
    int idx = mID % SUBN;

    hashID[idx].push(mID, i);
}

int findByID(int mID)
{
    int idx = mID % SUBN;

    Node* node = hashID[idx].head;

    while (node) {
        if (node->key == mID)
        {
            return node->value;
        }

        node = node->next;
    }

    return -1;
}

void init(int N) {

    space = N;

    fIdx = 0;

    empties.init();

    empties.push(1, N + 1);

    for (int i = 0; i < SUBN; i++) {
        hashID[i].init();
    }

    return;
}


int add(int mId, int mSize) {

    if (space < mSize) {
        return -1;
    }

    int idx = findByID(mId);

    if (idx == -1)
    {
        idx = fIdx++;
        pushByID(mId, idx);
    }

    files[idx].init();
    files[idx].id = mId;
    files[idx].size = mSize;

    space -= mSize;

    while (empties.length > 0 && mSize > 0)
    {
        Range empty = empties.pop();

        Range blk = { empty.left, empty.left + mSize };

        Range ovl = isOverlap(empty, blk);

        files[idx].push(ovl);

        mSize -= ovl.right - ovl.left;

        if (mSize == 0 && empty.right > ovl.right)
        {
            empties.push(ovl.right, empty.right);
        }
    }

    return files[idx].piece[0].left;
}


int remove(int mId) {
    int idx = findByID(mId);

    int ans = files[idx].pIdx;

    space += files[idx].size;

    for (int i = 0; i < files[idx].pIdx; i++)
    {
        empties.push(files[idx].piece[i].left, files[idx].piece[i].right);
    }

    files[idx].init();

    return ans;
}


int count(int mStart, int mEnd) {
    int ans = 0;

    Range rng = { mStart, mEnd + 1 };

    for (int i = 0; i < fIdx; i++) {
        for (int j = 0; j < files[i].pIdx; j++) {
            Range ovl = isOverlap(rng, files[i].piece[j]);

            if (ovl.right > ovl.left) {
                ans++;
                break;
            }
        }
    }

    return ans;
}
