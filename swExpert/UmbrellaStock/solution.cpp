#define MAXARR 10000

class Data {
public:
    int idx;
    int distance;
};

class Heap {
public:
    Data arr[MAXARR];
    int length;

    void init() { length = 0; }
    bool compare(int parent, int child) {
        if (arr[parent].distance > arr[child].distance) { return true; }
        if (arr[parent].distance == arr[child].distance && arr[parent].idx > arr[child].idx) { return true; }
        return false;
    }
    void push(int i, int d) {
        Data last = Data();
        last.idx = i; last.distance = d;

        arr[length] = last;
        int idx = length++;
        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Data temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    Data pop() {
        Data ans = arr[0];
        arr[0] = arr[--length];

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

class Children {
public:
    int arr[MAXARR];
    int size;

    Children() { size = 0; }
};

class Stock {
public:
    int parent;
    int distant;
    int quantity;
    int subTotal;
    Children children;

    Stock() {
        parent = -1;
        distant = 0;
        quantity = 0;
        subTotal = 0;
        children = Children();
    }
};

Stock stocks[MAXARR];
int dist2root, totalArr;
int dfsVisited[MAXARR];
int bfsVisited[MAXARR];
int nDfs, nBfs;

void move2root(int k, bool isPlus, int quant) {
    if (k == 0) {
        return;
    }

    if (dfsVisited[k] < nDfs) {
        dist2root += stocks[k].distant;
        dfsVisited[k] = nDfs;
    }
    else {
        dist2root -= stocks[k].distant;
    }
    
    if (isPlus) {
        stocks[stocks[k].parent].subTotal += quant;
    }
    else {
        stocks[stocks[k].parent].subTotal -= quant;
    }
    move2root(stocks[k].parent, isPlus, quant);
}

void plusSub(int k, int quant) {
    if (k == 0) {
        return;
    }

    stocks[stocks[k].parent].subTotal += quant;
    plusSub(stocks[k].parent, quant);
}

void init(int N, int mParent[], int mDistance[], int mQuantity[])
{
    totalArr = N;
    for (int i = 0; i < N; i++) {
        stocks[i] = Stock();
    }

    for (int i = 0; i < N; i++) {
        stocks[i].parent = mParent[i];
        stocks[i].distant = mDistance[i];
        stocks[i].quantity = mQuantity[i];
        if (mParent[i] != -1) { stocks[mParent[i]].children.arr[stocks[mParent[i]].children.size++] = i; }
        
        plusSub(i, mQuantity[i]);
        dfsVisited[i] = 0;
        bfsVisited[i] = 0;
    }

    nDfs = 1;
    nBfs = 1;
}

int carry(int mFrom, int mTo, int mQuantity)
{
    dist2root = 0;
    move2root(mFrom, false, mQuantity); 
    move2root(mTo, true, mQuantity);
    nDfs++;

    stocks[mFrom].quantity -= mQuantity;
    stocks[mTo].quantity += mQuantity;

    return mQuantity * dist2root;
}

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int gather(int mID, int mQuantity)
{
    int ans = 0;

    Heap queue;
    queue.init();

    queue.push(mID, 0);
    bfsVisited[mID] = nBfs;
    while (queue.length > 0 && mQuantity > 0) {
        Data curr = queue.pop();

        if (curr.idx != mID) {
            int quant = min(stocks[curr.idx].quantity, mQuantity);
            ans += carry(curr.idx, mID, quant);
            mQuantity -= quant;
        }

        for (int i = 0; i < stocks[curr.idx].children.size; i++) {
            if (bfsVisited[stocks[curr.idx].children.arr[i]] < nBfs) {
                bfsVisited[stocks[curr.idx].children.arr[i]] = nBfs;
                queue.push(stocks[curr.idx].children.arr[i], curr.distance + stocks[stocks[curr.idx].children.arr[i]].distant);
            }
        }
        if (bfsVisited[stocks[curr.idx].parent] < nBfs) {
            bfsVisited[stocks[curr.idx].parent] = nBfs;
            queue.push(stocks[curr.idx].parent, curr.distance + stocks[curr.idx].distant);
        }
    }

    nBfs++;
    return ans;
}

int sum(int mID)
{
    return stocks[mID].quantity + stocks[mID].subTotal;
}
