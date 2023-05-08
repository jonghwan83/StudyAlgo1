struct Result {
    int top;
    int count;
};

#define MAXROW 10000
#define MAXCOL 1000000

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}


class Piece {
public:
    int start;
    int end;

    Piece() {
        start = -1;
        end = -1;
    }
};

class Node {
public: 
    int checked;
    Piece data;
    Node* next; 
    Node* prev;

    Node() {
        checked = 0;
        next = nullptr;
        prev = nullptr;
    }
};

class LinkedList{
public:
    Node* head;
    Node* tail;
    int length;
    int nChecked;

    void init() {
        length = 0;
        nChecked = 0;
        
        Node* h = new Node();
        Node* t = new Node();
        head = h; tail = t;
        h->next = t;
        t->prev = h;
    }

    Node* front() {
        return head->next;
    }

    void push_back(int st, int ed) {
        Node* node = new Node();
        node->data.start = st;
        node->data.end = ed;
        node->checked = nChecked;
        
        node->prev = tail->prev;
        node->next = tail;
        tail->prev->next = node;
        tail->prev = node;

        length++;
    }

    Node* pop_front() {
        Node* ans = head->next;

        if (length > 0) {
            Node* node = head->next;

            head->next = node->next;
            node->next->prev = head;

            length--;
        }

        return ans;
    }
};


class HeapNode {
public:
    int height;
    int start;
    int end;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXCOL / 2];

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].height > arr[child].height) { return true; }
        if (arr[parent].height == arr[child].height && arr[parent].start > arr[child].start) { return true; }
        return false;
    }

    void push(int h, int st, int ed) {
        HeapNode last;
        last.height = h; last.start = st; last.end = ed;

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

bool checkOverlap(int st1, int ed1, int st2, int ed2) {
    int max_st = max(st1, st2);
    int min_ed = min(ed1, ed2);

    return min_ed > max_st;
}

class Row {
public:
    LinkedList blocks;
    int total;

    void init() {
        total = 0;
        blocks.init();
    }
};

Heap pQueue;
Row rows[MAXROW];
int bottomIdx, topIdx;
int colSize;
long int cnt;

void init(int C)
{
    colSize = C;
    bottomIdx = 0; topIdx = 0;
    cnt = 0;

    for (int i = 0; i < MAXROW; i++) {
        rows[i].init();
    }
}



Result dropBlocks(int mCol, int mHeight, int mLength)

{

    Result ret;
    ret.top = 0;
    ret.count = 0;

    pQueue.init();

    for (int i = 0; i < mHeight; i++) {
        pQueue.push(bottomIdx + i, mCol, mCol + mLength);
    }   

    while (pQueue.length > 0) {
        HeapNode curr = pQueue.pop();
        if (curr.height > topIdx) {
            topIdx = curr.height;
        }

        rows[curr.height].blocks.nChecked++;
        while (rows[curr.height].blocks.length > 0 && rows[curr.height].blocks.front()->checked < rows[curr.height].blocks.nChecked) {
            Node* b = rows[curr.height].blocks.pop_front();
            rows[curr.height].total -= (b->data.end - b->data.start);
            cnt -= (b->data.end - b->data.start);

            if (checkOverlap(b->data.start, b->data.end, curr.start, curr.end)) {
                int ovl_st = max(b->data.start, curr.start);
                int ovl_ed = min(b->data.end, curr.end);
                pQueue.push(curr.height + 1, ovl_st, ovl_ed);

                curr.start = min(b->data.start, curr.start);
                curr.end = max(b->data.end, curr.end);
            }
            else {
                rows[curr.height].blocks.push_back(b->data.start, b->data.end);
                rows[curr.height].total += (b->data.end - b->data.start);
                cnt += (b->data.end - b->data.start);
            }
        }

        rows[curr.height].blocks.push_back(curr.start, curr.end);
        rows[curr.height].total += (curr.end - curr.start);
        cnt += (curr.end - curr.start);
        if (curr.height > topIdx) {
            topIdx = curr.height;
        }
    }

    while (rows[bottomIdx].total >= colSize) {
        bottomIdx++;
        cnt -= colSize;
    }

    ret.top = topIdx - bottomIdx + 1;
    ret.count = cnt % MAXCOL;

    return ret;
}
