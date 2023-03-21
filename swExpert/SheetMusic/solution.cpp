#define MAXN 8000

class Node {
public:
    int idx;
    Node* next;
};

class LinkedList {
public:
    int length;
    Node* head;
    Node* tail;

    LinkedList() {
        length = 0;
        head = nullptr;
        tail = nullptr;
    }

    void init(int a) {
        Node* node = new Node();
        node->idx = a;

        head = node;
        tail = node;
        length++;
    }
};

class Motive {
public:
    int tone[2][4];
    int beat[2][4];
    bool isMerged;
    int candidate[MAXN];
    int size;

    void push_tone(int mTone[2][4]) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 4; j++) {
                tone[i][j] = mTone[i][j];
            }
        }
    }

    void push_beat(int mBeat[2][4]) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 4; j++) {
                beat[i][j] = mBeat[i][j];
            }
        }
    }
};

class Queue {
public:
    int st, ed, length;
    int arr[MAXN * 4];

    Queue() {
        st = 0; ed = 0; length = 0;
    }
     void push(int a) {
        arr[ed++] = a;
        length++;
    }

    int pop() {
        length--;
        return arr[st++];
    }
};

Motive motives[MAXN];
LinkedList sheet[MAXN];

void connect(int idx1, int idx2) {
    sheet[idx1].tail->next = sheet[idx2].head;
    sheet[idx1].tail = sheet[idx2].tail;
    sheet[idx1].length += sheet[idx2].length;

    motives[idx2].isMerged = true;
}

bool compare(int idx1, int idx2) {
    int nTone = 0;

    for (int i = 0; i < 4; i++) {
        if (motives[idx1].beat[1][i] != motives[idx2].beat[0][i]) { return false; }
        if (motives[idx1].tone[1][i] == motives[idx2].tone[0][i]) { nTone++; }
    }

    if (nTone < 2) { return false; }
    return true;
}

void compose(int N, int mTone[MAXN][2][4], int mBeat[MAXN][2][4], int mAns[MAXN])
{
    mAns[0] = 0;

    Queue queue;
    int second = -1;

    for (int i = 0; i < N; i++) {
        motives[i].push_tone(mTone[i]);
        motives[i].push_beat(mBeat[i]);
        motives[i].isMerged = false;

        sheet[i] = LinkedList();
        sheet[i].init(i);

        if (i != 0) { queue.push(i); }
    }

    int curr;
    while (queue.length > 0) {
        curr = queue.pop();

        if (motives[curr].isMerged) { continue; }

        int cnt = 0;
        int cIdx = -1;
        for (int i = queue.st; i < queue.ed; i++) {
            if (motives[queue.arr[i]].isMerged) { continue; }
            if (compare(sheet[curr].tail->idx, sheet[queue.arr[i]].head->idx)) {
                cIdx = queue.arr[i];
                cnt++;
                if (cnt > 1) { break; }
            }
        }

        if (cnt == 1) { connect(curr, cIdx); }
        if (sheet[curr].length == N - 1) {
            second = curr;
            break;
        }
        queue.push(curr);
    }

    Node* node = sheet[second].head;
    int i = 1;
    while (node) {
        mAns[i++] = node->idx;
        node = node->next;
    }
}
