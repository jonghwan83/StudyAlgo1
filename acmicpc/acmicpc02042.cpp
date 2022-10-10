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

    makeTree(2 * idx, start, (start + end) / 2);
    makeTree(2 * idx + 1, (start + end) / 2 + 1, end);
    tree[idx] = tree[2 * idx] + tree[2 * idx + 1];
}

void update(int node, int idx, int start, int end, int diff) {
    if (idx < start || idx > end) { return; }
    tree[node] = tree[node] + diff;
    if (start != end) {
        update(2 * node, idx, start, (start + end) / 2, diff);
        update(2 * node + 1, idx, (start + end) / 2 + 1, end, diff);
    }
}

void updateTree(int idx, int val) {
    int diff = val - arr[idx];
    arr[idx] = val;
    update(1, idx, 0, arr.size() - 1, diff);
}

int query(int node, int start, int end, int left, int right) {
    if (left > end || right < start) { return 0; }
    if (left <= start && end <= right) { return tree[node]; }
    int lsum = query(node * 2, start, (start + end) / 2, left, right);
    int rsum = query(node * 2 + 1, (start + end) / 2 + 1, end, left, right);
    
    return lsum + rsum;
}

int main() {
    freopen("../sample_input.txt", "r", stdin);
    cin >> n >> m >> k;

    arr.resize(n);
    tree.resize(2 * n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    makeTree(1, 0, arr.size()-1);
    int c, idx, val;
    for (int i=0; i < n+m; i++) {
        cin >> c >> idx >> val;
        if (c == 1) {
            updateTree(idx - 1, val);
        }
        if (c == 2) {
            cout << query(1, 0, arr.size()-1, idx, val) << endl;
        }
    }

    for (int i=0; i < tree.size(); i++) {
        cout << tree[i] << " ";
    }
    cout << endl;
}