#include <queue>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#define MAXSIZE 5000000

using namespace std;

class Heap {
public:
    int arr[MAXSIZE];
    int length;

    Heap() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (arr[parent] < arr[child]) { return true; }
        return false;
    }

    void push(int a) {
        arr[length] = a;

        int idx = length;
        while (compare((idx - 1) / 2, idx) && idx > 0) {
            int temp = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = arr[idx];
            arr[idx] = temp;
            idx = (idx - 1) / 2;
        }
        length++;
    }
};

priority_queue<int> pq;
vector<int> vec;
Heap heap;

int main() {
    clock_t start, end;

    start = clock();
    for (int i = 0; i < MAXSIZE; i++) {
        pq.push(i);
    }
    end = clock() - start;
    printf("elapsed(priority_queue): %f\n", (float) end / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < MAXSIZE; i++) {
        vec.push_back(i);
        push_heap(vec.begin(), vec.end());
    }
    end = clock() - start;
    printf("elapsed(vec): %f\n", (float) end / CLOCKS_PER_SEC);

     start = clock();
     for (int i = 0; i < MAXSIZE; i++) {
         heap.push(i);
     }
     end = clock() - start;
     printf("elapsed(custom): %f\n", (float) end / CLOCKS_PER_SEC);

    return 0;
}