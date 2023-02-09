#include <cstring>
#include <string>
#include <unordered_map>

#define MAX_STOCK_LEN 10
#define MAXSTOCK 100000
#define MAXCOL 400

using namespace std;

class Data {
public:
    char name[MAX_STOCK_LEN + 1];
    int id;
};

class Heap {
public:
    Data arr[MAXSTOCK];
    int length;

    void init() { length = 0; }
    bool compare(int parent, int child) {
        if (strcmp(arr[parent].name, arr[child].name) > 0) { return true; }
        if (strcmp(arr[parent].name, arr[child].name) == 0 && arr[parent].id > arr[child].id) { return false; }
        return false;
    }

    void push(char mStock[MAX_STOCK_LEN + 1], int i) {
        Data last = Data();
        strcpy(last.name, mStock);
        last.id = i;
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

class Stock {
public:
    int initPrice;
    int diff;
    string name;

    Stock() {
        initPrice = 0;
        diff = 0;
    }
};

Stock stocks[MAXSTOCK];

class Group {
public:
    int diff;
    int idx[MAXCOL];
    int maxIdx;
    int length;

    Group() {
        maxIdx = MAXSTOCK + 1;
        diff = 0;
        length = 0;
    }

    void update() {
        for (int i = 0; i < length; i++) {
            if (stocks[idx[i]].diff > stocks[maxIdx].diff) {
                maxIdx = idx[i];
            }
            else if (stocks[idx[i]].diff == stocks[maxIdx].diff) {
                maxIdx = min(maxIdx, idx[i]);
            }
        }
    }
};

Heap pQueue;

unordered_map<string, int> hashStock;
Group group[MAXSTOCK / MAXCOL];

void init(int N, char mStocks[][MAX_STOCK_LEN + 1], int mPrices[])
{
    hashStock.clear();
    pQueue.init();

    for (int i = 0; i < N; i++) {
        pQueue.push(mStocks[i], i);

        stocks[i].name = mStocks[i];
        stocks[i].diff = 0;
        stocks[i].initPrice = mPrices[i];
    }

    int gIdx = 0;
    int subIdx = 0;
    group[gIdx] = Group();

    while (pQueue.length > 0) {
        Data curr = pQueue.pop();

        if (subIdx >= MAXCOL) {
            gIdx++;
            subIdx = 0;
            group[gIdx] = Group();
        }
        group[gIdx].idx[subIdx] = curr.id;
        group[gIdx].length++;

        if (curr.id < group[gIdx].maxIdx) { group[gIdx].maxIdx = curr.id; }
        hashStock[curr.name] = gIdx * MAXCOL + subIdx;

        subIdx++;
    }
}

void changePrices(char mFromStock[], char mToStock[], int mPriceDiff)
{
    int row = hashStock[mFromStock] / MAXCOL;
    int col = hashStock[mFromStock] % MAXCOL;
    int eRow = hashStock[mToStock] / MAXCOL;
    int eCol = hashStock[mToStock] % MAXCOL;

    while (row <= eRow) {
        if (row < eRow && col == 0) {
            group[row].diff += mPriceDiff;
            row++;
            continue;
        }

        stocks[group[row].idx[col]].diff += mPriceDiff;
        col++;
        if (col >= group[row].length) {
            group[row].update();
            row++;
            col = 0;
        }

        if (row == eRow && col > eCol) {
            group[row].update();
            break;
        }
    }
}

int getPrice(char mStock[])
{
    int row = hashStock[mStock] / MAXCOL;
    int col = hashStock[mStock] % MAXCOL;

    return stocks[group[row].idx[col]].diff + group[row].diff + stocks[group[row].idx[col]].initPrice;
}

int getMostIncreasedStock(char mFromStock[], char mToStock[])
{
    int row = hashStock[mFromStock] / MAXCOL;
    int col = hashStock[mFromStock] % MAXCOL;
    int eRow = hashStock[mToStock] / MAXCOL;
    int eCol = hashStock[mToStock] % MAXCOL;

    int mostIncresed = -1000001;
    int ans = -1;

    while (row <= eRow) {
        if (row < eRow && col == 0) {
            if (stocks[group[row].maxIdx].diff + group[row].diff > mostIncresed) {
                mostIncresed = stocks[group[row].maxIdx].diff + group[row].diff;
                ans = group[row].maxIdx;
            }
            else if (stocks[group[row].maxIdx].diff + group[row].diff == mostIncresed) {
                ans = min(group[row].maxIdx, ans);
            }
            row++;
            continue;
        }

        if (stocks[group[row].idx[col]].diff + group[row].diff > mostIncresed) {
            mostIncresed = stocks[group[row].idx[col]].diff + group[row].diff;
            ans = group[row].idx[col];
        }
        else if (stocks[group[row].idx[col]].diff + group[row].diff == mostIncresed) {
            ans = min(group[row].idx[col], ans);
        }

        col++;
        if (col >= group[row].length) {
            row++;
            col = 0;
        }

        if (row == eRow && col > eCol) { break; }
    }
    return ans;
}