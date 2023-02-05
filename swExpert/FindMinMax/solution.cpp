#define MAXCOL 350
#define MAXROW 200000 / MAXCOL
#define MAXVAL 100000000


class Heap {
public:
    int arr[MAXROW * MAXCOL];
    int length;
    bool isMaxPriority;

    void init(bool isMaxQueue) {
        length = 0;
        isMaxPriority = isMaxQueue;
    }

    bool compare(int parent, int child) {
        if (isMaxPriority) {
            if (arr[parent] < arr[child]) { return true; }
        }
        else {
            if (arr[parent] > arr[child]) { return true; }
        }
        return false;
    }

    void push(int a) {
        arr[length] = a;
        int idx = length;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            int temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
        length++;
    }
};

class NumArray {
public:
    int arr[MAXCOL];
    bool isRemoved;
    bool isExist[MAXCOL];
    int maxValue;
    int minValue;
    int length;
    int count;

    NumArray() {
        isRemoved = false;
        maxValue = -1;
        minValue = MAXVAL;
        length = 0;
        count = 0;
    }

    void update() {
        maxValue = -1;
        minValue = MAXVAL;

        for (int i = 0; i < count; i++) {
            if (isExist[i]) {
                if (arr[i] < minValue) { minValue = arr[i]; }
                if (arr[i] > maxValue) { maxValue = arr[i]; }
            }
        }

        if (maxValue == -1) {
            isRemoved = true;
        }
    }
};

int idx;
NumArray numArray[MAXROW];

void addValue(int value) {
    if (numArray[idx].count >= MAXCOL) { numArray[++idx] = NumArray(); }
    numArray[idx].arr[numArray[idx].count] = value;
    numArray[idx].isExist[numArray[idx].count] = true;
    if (value < numArray[idx].minValue) { numArray[idx].minValue = value; }
    if (value > numArray[idx].maxValue) { numArray[idx].maxValue = value; }
    numArray[idx].length++;
    numArray[idx].count++;
}

void init(int N, int mValue[])
{
    idx = 0;
    numArray[idx] = NumArray();
    for (int i = 0; i < N; i++) {
        addValue(mValue[i]);
    }
}
void add(int M, int mValue[])
{
    for (int i = 0; i < M; i++) {
        addValue(mValue[i]);
    }
}
void erase(int mFrom, int mTo)
{
    int numIdx;
    int cnt = 0;
    for (int i = 0; i <= idx; i++) {
        if (!numArray[i].isRemoved) { cnt += numArray[i].length; }
        if (cnt >= mFrom) {
            numIdx = i;
            break;
        }
    }

    int k = mTo - mFrom + 1;
    int loc;
    int t = 0;
    for (int i = 0; i < numArray[numIdx].count; i++) {
        if (numArray[numIdx].isExist[i]) { t++; }
        if (t == numArray[numIdx].length - (cnt - mFrom)) {
            loc = i;
            break;
        }
    }
    while (k > 0) {
        if (loc >= numArray[numIdx].count) {
            numArray[numIdx].update();
            numIdx++;
            loc = 0;
        }
        if (numArray[numIdx].isRemoved) {
            numIdx++;
            loc = 0;
            continue;
        }

        if(numArray[numIdx].isExist[loc]) {
            numArray[numIdx].isExist[loc] = false;
            numArray[numIdx].length--;
            k--;
        }
        loc++;
    }
    numArray[numIdx].update();
}
int find(int K)
{
    Heap minQueue;
    Heap maxQueue;
    minQueue.init(false); maxQueue.init(true);

    int loc = numArray[idx].count - 1;
    int numIdx = idx;
    while (K > 0) {
        if (loc < 0) { loc = numArray[--numIdx].count - 1; }
        if (numArray[numIdx].isRemoved) {
            loc = numArray[--numIdx].count - 1;
            continue;
        }
        if (K > numArray[numIdx].length) {
            minQueue.push(numArray[numIdx].minValue);
            maxQueue.push(numArray[numIdx].maxValue);
            K -= numArray[numIdx].length;
            loc = numArray[--numIdx].count - 1;
            continue;
        }

        if (numArray[numIdx].isExist[loc]) {
            minQueue.push(numArray[numIdx].arr[loc]);
            maxQueue.push(numArray[numIdx].arr[loc]);
            K--;
        }
        loc--;
    }
    return maxQueue.arr[0] - minQueue.arr[0];
}