extern void move(int mode);

#define MAXARR 50000
#define MAXP 1024

int abs(int a) {
    if (a < 0) { return -a; }
    return a;
}

int pow(int n, int r) {
    int ans = 1;

    for (int i = 0; i < r; i++) {
        ans *= n;
    }

    return ans;
}

class Node {
public:
    int data;
    Node* next;
};

class LinkedList {
public:
    int length;
    Node* head;
    Node* tail;

    void init() {
        length = 0;
        head = nullptr;
        tail = nullptr;
    }

    void push(int a) {
        Node* node = new Node();
        node->data = a;

        if (!head) {
            head = node;
        }
        else {
            tail->next = node;
        }

        tail = node;
        length++;
    }

    void copy(LinkedList a) {

        Node* node = a.head;
        while (node) {
            push(node->data);
            node = node->next;
        }
    }
};


class HeapNode {
public:
    int score;
    int loc;
    int p;
    LinkedList seq;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXARR];

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (arr[parent].score > arr[child].score) { return true; }
        if (arr[parent].score == arr[child].score && arr[parent].loc > arr[child].loc) { return true; }
        if (arr[parent].score == arr[child].score && arr[parent].loc == arr[child].loc && arr[parent].seq.length > arr[child].seq.length) { return true; }
        return false;
    }

    void push(int s, int l, int p, LinkedList seq) {
        HeapNode last;
        last.score = s; last.loc = l; last.seq = seq; last.p = p;


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

int dir[4] = { -3, 3, -1, 1 };
int rev[9] = { 8, 7, 6, 5, 4, 3, 2, 1, 0 };

int swap(int a, int b, int puzzle) {

    int p[9];

    for (int i = 0; i < 9; i++) {
        p[i] = (puzzle / pow(10, rev[i])) % 10;
    }

    int temp = p[a];
    p[a] = p[b];
    p[b] = temp;

    int ans = 0;

    for (int i = 0; i < 9; i++) {
        ans += pow(10, rev[i]) * p[i];
    }
    return ans;
}

int getScore(int puzzle) {
    int ans = 0;

    for (int i = 0; i < 9; i++) {
        int k = (puzzle / pow(10, rev[i])) % 10;

        if (k == 9) { continue; }
        int rGoal = (k - 1) / 3;
        int cGoal = (k - 1) % 3;

        int rCurr = i / 3;
        int cCurr = i % 3;

        int dist = abs(rCurr - rGoal) + abs(cCurr - cGoal);

        ans += dist;
    }

    return ans;
}

int p2key(int puzzle) {
    return puzzle % MAXP;
}

LinkedList visited[MAXP];

void pushVisited(int puzzle) {
    int key = p2key(puzzle);
    visited[key].push(puzzle);
}

bool isInclude(int puzzle) {
    int key = p2key(puzzle);

    Node* node = visited[key].head;

    while (node) {
        if (node->data == puzzle) { return true; }
        node = node->next;
    }
    return false;
}


void test(const int P[3][3]) {

    int puzzle = 0; int st;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int k = i * 3 + j;

            if (P[i][j] == 0) {
                puzzle += pow(10, rev[k]) * 9;
                st = k;
            }
            else {
                puzzle += pow(10, rev[k]) * P[i][j];
            }

        }
    }

    for (int i = 0; i < MAXP; i++) {
        visited[i].init();
    }


    Heap pQueue; pQueue.init();

    LinkedList a; a.init();
    pQueue.push(0, st, puzzle, a);

    pushVisited(puzzle);

    int k = 0;
    while (pQueue.length > 0) {
        HeapNode curr = pQueue.pop(); k++;

        for (int i = 0; i < 4; i++) {
            int d = curr.loc + dir[i];

            if (d < 0 || d > 8) { continue; }
            if (curr.loc % 3 == 0 && d % 3 == 2) { continue; }
            if (curr.loc % 3 == 2 && d % 3 == 0) { continue; }

            int newPuzzle = swap(curr.loc, d, curr.p);

            if (newPuzzle == 123456789) {
                curr.seq.push(i);

                Node* node = curr.seq.head;
                while (node) {
                    move(node->data);
                    node = node->next;
                }
                return;
            }

            if (!isInclude(newPuzzle)) {
                pushVisited(newPuzzle);

                LinkedList s; s.init();
                s.copy(curr.seq);
                s.push(i);

                int g = s.length;
                int h = getScore(newPuzzle);

                pQueue.push(g + h, d, newPuzzle, s);
            }
        }
    }


}

