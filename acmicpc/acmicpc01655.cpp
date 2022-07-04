#include <iostream>

using namespace std;

class minheap{
    public:
        int n;
        int arr[100000][2];    

    
    minheap() {
        n = 0;
    }

    int get_parent(int idx) {
        int result = (idx - 1) / 2;
        return result;
    }

    int get_left(int idx) {
        int result = 2 * idx + 1;
        return result;
    }

    int get_right(int idx) {
        int result = 2 * idx + 2;
        return result;
    }

    void push(int a, int b) {
        arr[n][0] = a;
        arr[n][1] = b;
        n++;

        int idx = n - 1;

        while (idx > 0) {
            int parent = get_parent(idx);
            if (arr[parent][0] > arr[idx][0]) {
                swap(arr[parent], arr[idx]);
                idx = parent;
            }
            else {
                break;
            }
        }
    }

    int pop() {
        int result = arr[0][1];
        swap(arr[0], arr[n-1]);
        n--;

        int idx = 0;
        while (idx < n) {
            int left = get_left(idx);
            int right = get_right(idx);

            if (left < n && right < n) {
                if (arr[left][0] < arr[idx][0]) {
                    swap(arr[left], arr[idx]);
                    idx = left;
                } 
                else if (arr[right][0] < arr[idx][0]) {
                    swap(arr[right], arr[idx]);
                    idx = right;
                }
                else {
                    break;
                }
            }
            else if (left < n && right >= n) {
                if (arr[left][0] < arr[idx][0]) {
                    swap(arr[left], arr[idx]);
                    idx = left;
                }
                else {
                    break;
                }
            }
            else if (left >= n && right < n) {
                if (arr[right][0] < arr[idx][0]) {
                    swap(arr[right], arr[idx]);
                    idx = right;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
        return result;
    }
};

int main() {
    minheap leftq;
    minheap rightq;

    int k, m;

    cin >> k;

    for (int i=0; i < k; i++) {
        cin >> m;
        if (leftq.n == rightq.n) {
            leftq.push(-m, m);
        }
        else {
            rightq.push(m, m);
        }

        if (leftq.n > 0 && rightq.n > 0) {
            if (leftq.arr[0][1] > rightq.arr[0][1]) {
                int a = leftq.pop();
                int b = rightq.pop();
                leftq.push(-b, b);
                rightq.push(a, a);
            }
        }

        cout << leftq.arr[0][1] << "\n";
    }
    return 0;
}