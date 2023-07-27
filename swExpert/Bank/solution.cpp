
#define MAXARR 1000
#define INF 1000000

struct HeapNode {
    int weight;
    int idx;
};

class Heap {
public:
    HeapNode arr[MAXARR];
    int length;

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (arr[parent].weight > arr[child].weight) { return true; }
        return false;
    }

    void push(int w, int i) {
        HeapNode last; last.weight = w; last.idx = i;

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

        while ((idx * 2 + 1 < length)) {
            left = idx * 2 + 1;
            right = idx * 2 + 2;

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


int moneyCount[8];
int units[8] = { 50000, 10000, 5000, 1000, 500, 100, 50, 10 };

void save(int M[8]) {
    for (int i = 0; i < 8; i++) {
        moneyCount[i] = M[i];
    }
}

void withdraw(int account[1000], int result[1000][8]) {

    Heap pQueue[8];
    for (int i = 0; i < 8; i++) {
        pQueue[i].init();
    }

    for (int i = 0; i < 8; i++) {

        for (int j = 0; j < 1000; j++) {

            if (i > 0) { break; }

            int temp = account[j] / units[i];
            if (temp == 0) {
                pQueue[i].push(INF, j);
            }
            else {
                pQueue[i].push(temp, j);
            }

        }

        while (pQueue[i].length > 0) {
            HeapNode curr = pQueue[i].pop();

            if (curr.weight == INF || moneyCount[i] < curr.weight) {
                result[curr.idx][i] = 0;

                if (i + 1 < 8) {
                    int temp = account[curr.idx] / units[i + 1];
                    if (temp == 0) {
                        pQueue[i + 1].push(INF, curr.idx);
                    }
                    else {
                        pQueue[i + 1].push(temp, curr.idx);
                    }
                }
                continue;
            }

            result[curr.idx][i] = curr.weight;
            account[curr.idx] -= curr.weight * units[i];
            moneyCount[i] -= curr.weight;

            if (i + 1 < 8) {
                int temp = account[curr.idx] / units[i + 1];
                if (temp == 0) {
                    pQueue[i + 1].push(INF, curr.idx);
                }
                else {
                    pQueue[i + 1].push(temp, curr.idx);
                }
            }

        }

    }


}