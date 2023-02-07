#define MAXCOL 350
#define MAXROW 200000 / MAXCOL
#define MAXVAL 100000000

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
    int minAns = MAXVAL;
    int maxAns = -1;

    int loc = numArray[idx].count - 1;
    int numIdx = idx;
    while (K > 0) {
        if (loc < 0) { loc = numArray[--numIdx].count - 1; }
        if (numArray[numIdx].isRemoved) {
            loc = numArray[--numIdx].count - 1;
            continue;
        }
        if (K > numArray[numIdx].length) {
            if (numArray[numIdx].minValue < minAns) { minAns = numArray[numIdx].minValue; }
            if (numArray[numIdx].maxValue > maxAns) { maxAns = numArray[numIdx].maxValue; }
            K -= numArray[numIdx].length;
            loc = numArray[--numIdx].count - 1;
            continue;
        }

        if (numArray[numIdx].isExist[loc]) {
            if (numArray[numIdx].arr[loc] < minAns) { minAns = numArray[numIdx].arr[loc]; }
            if (numArray[numIdx].arr[loc] > maxAns) { maxAns = numArray[numIdx].arr[loc]; }
            K--;
        }
        loc--;
    }
    return maxAns - minAns;
}
