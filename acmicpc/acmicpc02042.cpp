#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

int n, m, k;
vector<long long> arr;
vector<long long> tree;

void makeTree(int idx, int start, int end) {
    if (start == end) {
        tree[idx] = arr[start];
        return;
    }

    makeTree(2 * idx, start, (start + end) / 2);
    makeTree(2 * idx + 1, (start + end) / 2 + 1, end);
    tree[idx] = tree[2 * idx] + tree[2 * idx + 1];
}

void update(int node, long long idx, int start, int end, long long diff) {
    if (idx < start || idx > end) { return; }
    tree[node] = tree[node] + diff;
    if (start != end) {
        update(2 * node, idx, start, (start + end) / 2, diff);
        update(2 * node + 1, idx, (start + end) / 2 + 1, end, diff);
    }
}

void updateTree(long long idx, long long val) {
    long long diff = val - arr[idx];
    arr[idx] = val;
    update(1, idx, 0, (int)arr.size() - 1, diff);
}

long long query(int node, int start, int end, long long left, long long right) {
    if (left > end || right < start) { return 0; }
    if (left <= start && end <= right) { return tree[node]; }
    long long lsum = query(node * 2, start, (start + end) / 2, left, right);
    long long rsum = query(node * 2 + 1, (start + end) / 2 + 1, end, left, right);

    return lsum + rsum;
}

int main() {
    //freopen("sample_input.txt", "r", stdin);
    cin >> n >> m >> k;
    int h = (int)ceil(log2(n));

    arr.resize(n);
    tree.resize(1 << (h+1));
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    makeTree(1, 0, (int)arr.size() - 1);
    int c;
    long long t1, t2;
    for (int i = 0; i < m + k; i++) {
        cin >> c >> t1 >> t2;
        if (c == 1) {
            updateTree(t1 - 1, t2);
        }
        if (c == 2) {
            cout << query(1, 0, (int)arr.size() - 1, t1 - 1, t2 - 1) << "\n";
        }
    }
}