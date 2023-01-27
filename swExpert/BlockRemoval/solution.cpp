#include <deque>

#define MAXDROP 1000
#define MAXARR 10000

using namespace std;

class Data {
public:
    int left;
    int right;
    int total;
    int time;

    Data() {}
    Data(int l, int r, int t) {
        left = l;
        right = r;
        total = r - l;
        time = t;
    }
};

class Heap {
public:
    Data arr[MAXARR];
    int length;

    void init() { length = 0; }
    bool compare(int parent, int child) {
        if (arr[parent].time > arr[child].time) { return true; }
        return false;
    }

    void push(int left, int right, int time) {
        Data last = Data(left, right, time);

        arr[length] = last;
        int idx = length;
        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Data temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
        length++;
    }

    Data pop() {
        Data ans = arr[0];
        length--;
        arr[0] = arr[length];

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
                Data temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }
        return ans;
    }
};

int gridRow, gridCol;
Heap blocks;
deque< pair<int, int> > erasers;
deque<Data> pushBlocks;

void updateTime(int mTimestamp) {
    while (blocks.arr[0].time <= mTimestamp - gridRow && blocks.length > 0) {
        Data curr = blocks.pop();
    }
}

int getBlockCount() {
    int ans = 0;
    for (int i = 0; i < blocks.length; i++) {
        ans += blocks.arr[i].total;
    }
    return ans;
}

Data overlapRange(pair<int, int> a1, pair<int, int> a2) {
    Data ans;
    ans.left = max(a1.first, a2.first);
    ans.right = min(a1.second, a2.second);
    ans.total = ans.right - ans.left;
    if (ans.total < 0) { ans.total = 0; }

    return ans;
}

void eraseBlock(Data block) {
    int size = erasers.size();
    
    Data overlapped;
    pair<int, int> e;
    for (int i = 0; i < size; i++) {
        e = erasers.front();
        erasers.pop_front();

        overlapped = overlapRange(make_pair(block.left, block.right), e);
        
        if (overlapped.total == 0) {
            erasers.push_back(e);
            continue;
        }

        if (block.left < overlapped.left) {
            Data temp = Data(block.left, overlapped.left, block.time);
            pushBlocks.push_back(temp);
        }
        block.left = overlapped.right;
        if (e.first < overlapped.left) {
            erasers.push_back(make_pair(e.first, overlapped.left));
        }
        if (e.second > overlapped.right) {
            erasers.push_back(make_pair(overlapped.right, e.second));
        }
    }
    if (block.left < block.right) {
        pushBlocks.push_back(block);
    }
}

void erase() {
    while (blocks.length > 0) {
        Data curr = blocks.pop();
        eraseBlock(curr);
    }

    for (int i = 0; i < pushBlocks.size(); i++) {
        blocks.push(pushBlocks[i].left, pushBlocks[i].right, pushBlocks[i].time);
    }
}

void init(int R, int C) {
    blocks.init();

    gridCol = C;
    gridRow = R;
}

int dropBlocks(int mTimestamp, int mCol, int mLen) {
    updateTime(mTimestamp);

    blocks.push(mCol, mCol + mLen, mTimestamp);

    return getBlockCount();
}

int removeBlocks(int mTimestamp) {
    updateTime(mTimestamp);

    pushBlocks.clear();
    erasers.clear();

    erasers.push_back(make_pair(0, gridCol));
    erase();

    return getBlockCount();
}