#include <iostream>

using namespace std;

class HeapQ{
public:
    int length;
    int H[100000];

    HeapQ(){
        length = 0;
    }

    void swap(int* a, int* b){
        int t = *a;
        *a = *b;
        *b = t;
    }

    int getParent(int idx){
        return (idx-1) / 2;
    }

    int getleftChild(int idx){
        return idx * 2 + 1;
    }

    int getrightChild(int idx){
        return idx * 2 + 2;
    }

    void push(int a){
        H[length] = a;
        length++;

        int idx = length - 1;
        while (idx > 0){
            int parent = getParent(idx);
            if (abs(H[idx]) <= abs(H[parent])){
                swap(&H[idx], &H[parent]);
            } else if (abs(H[idx]) == abs(H[parent]) && H[idx] < H[parent]) {
                swap(&H[idx], &H[parent]);
            } else {
                break;
            }
            idx = parent;
        }
        return;
    }
    
    int pop(){
        if (length < 1){
            return 0;
        }

        int answer = H[0];
        H[0] = H[length-1];
        length--;
        int idx = 0;

        while (idx < length){
            int leftChild = getleftChild(idx);
            int rightChild = getrightChild(idx);

            if (leftChild >= length && rightChild >= length){
                break;
            }

            int child;
            if (abs(H[leftChild]) < abs(H[rightChild])){
                child = leftChild;
            } else {
                if (H[leftChild] < H[rightChild]){
                    child = leftChild;
                } else {
                    child = rightChild;
                }
            }

            if (abs(H[idx]) > abs(H[child])){
                swap(&H[idx], &H[child]);
            } else if (abs(H[idx]) == abs(H[child]) && H[idx] > H[child]) {
                swap(&H[idx], &H[child]);
            } else {
                break;
            }
            idx = child;
        }
        return answer;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    HeapQ* heapq = new HeapQ;
    int n, k;
    cin >> n;
    for (int i=0; i < n; i++){
        cin >> k;
        if (k == 0){
            cout << heapq->pop() << "\n";
        } else {
            heapq->push(k);
        }
    }

    return 0;
}