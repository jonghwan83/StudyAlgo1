#include <vector>
#include <iostream>

using namespace std;

int n, m, k;
vector<int> arr;
vector<int> tree;

void makeTree(int idx, int start, int end) {
    if (start == end) {
        tree[idx] = arr[start];
        return;
    }

    makeTree(2 * idx + 1, start, (start + end) / 2);
    makeTree(2 * idx + 2, (start + end) / 2 + 1, end);
    tree[idx] = tree[2 * idx + 1] + tree[2 * idx + 2];
}

void update(int node, int idx, int start, int end, int diff) {
    if (idx < start || idx > end) { return; }
    tree[node] = tree[node] + diff;
    if (start != end) {
        update(2 * node + 1, idx, start, (start + end) / 2, diff);
        update(2 * node + 2, idx, (start + end) / 2 + 1, end, diff);
    }
}

void updateTree(int idx, int start, int end, int val) {
    int diff = arr[idx] - val;
    arr[idx] = val;
    update(0, idx, start, end, diff);
}

int main() {
    freopen("sample_input.txt", "r", stdin);
    cin >> n >> m >> k;

    arr.resize(n);
    tree.resize(2 * n - 1);
    for (int i=0; i < n; i++) {
        cin >> arr[i];
    }

    makeTree(0, 0, arr.size()-1);
    int c, idx, val;
    for (int i=0; i < n+m; i++) {
        cin >> c >> idx >> val;
        if (c == 1) {
            updateTree(idx, 0, tree.size() - 1,val);
        }
    }

    cout << tree[0] << endl;
}