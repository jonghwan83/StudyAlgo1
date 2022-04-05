#include <iostream>

using namespace std;

class HeapQ {
public:
    int length;
    int H[100000];

    HeapQ() {
        length = 0;
    }
    int getPrarent(int idx) {

        return (idx - 1) / 2;
    }
    int getleftChild(int idx) {
        return (2 * idx + 1);
    }
    int getrightChild(int idx) {
        return (2 * idx + 2);
    }
    void swap(int* x, int* y) {
        int t = *x;
        *x = *y;
        *y = t;
    }

    void insert(int val) {
        H[length] = val;
        length++;

        int idx = length - 1;
        int parent;

        while (idx > 0) {
            parent = getPrarent(idx);
            if (H[idx] > H[parent]) {
                swap(&H[idx], &H[parent]);
                idx = parent;
            }
            else {
                return;
            }
        }
        return;
    }

    int pop() {
        if (length < 1) {
            return 0;
        }

        int answer = H[0];
        H[0] = H[length - 1];
        length--;

        int idx = 0;
        while (true) {
            int leftChild = getleftChild(idx);
            int rightChild = getrightChild(idx);

            if (leftChild >= length && rightChild >= length) {
                break;
            }

            if (H[idx] < H[leftChild] || H[idx] < H[rightChild]) {
                if (H[leftChild] > H[rightChild]) {
                    swap(&H[idx], &H[leftChild]);
                    idx = leftChild;
                }
                else {
                    swap(&H[idx], &H[rightChild]);
                    idx = rightChild;
                }
            }
            else {
                break;
            }
        }
        return answer;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    HeapQ* heapq = new HeapQ;

    int n, k;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> k;
        if (k == 0) {
            cout << heapq->pop() << "\n";
        }
        else {
            heapq->insert(k);
        }
    }

    return 0;
}