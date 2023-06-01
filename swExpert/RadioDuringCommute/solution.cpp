#define MAXN 100000
#define SUBSET 320
#define MAXTYPE 1000

class Queue {
public:
    int length;
    int arr[MAXN];
    int st, ed;

    void init() {
        length = 0;
        st = 0; ed = 0;
    }

    void push(int a) {
        length++;
        arr[ed++] = a;
    }
};


Queue types[MAXTYPE];

class Road {
public:
    int subTotal;
    int time[SUBSET];

    int getSubByRange(int st, int ed) {
        int ans = 0;
        
        for (int i = st; i < ed; i++) {
            ans += time[i];
        }

        return ans;
    }
};

Road roads[SUBSET];

void init(int N, int M, int mType[], int mTime[])
{
    
    for (int i = 0; i < M; i++) {
        types[i].init();
    }

    for (int i = 0; i < N - 1; i++) {

        int row = i / SUBSET;
        int col = i % SUBSET;

        if (col == 0) {  roads[row].subTotal = 0; }

        roads[row].time[col] = mTime[i];

        roads[row].subTotal += mTime[i];

        types[mType[i]].push(i);
    }
}

void destroy()
{



}


void update(int mID, int mNewTime)

{

    int row = mID / SUBSET;
    int col = mID % SUBSET;

    int oldVal = roads[row].time[col];
    roads[row].subTotal -= oldVal;
    roads[row].subTotal += mNewTime;
    roads[row].time[col] = mNewTime;
}



int updateByType(int mTypeID, int mRatio256)
{
    int ans = 0;

    for (int i = 0; i < types[mTypeID].length; i++) {
        int row = types[mTypeID].arr[i] / SUBSET;
        int col = types[mTypeID].arr[i] % SUBSET;

        int oldVal = roads[row].time[col];
        int newVal = roads[row].time[col] * mRatio256 / 256;

        roads[row].subTotal -= oldVal;
        roads[row].subTotal += newVal;

        roads[row].time[col] = newVal;

        ans += newVal;
    }

    return ans;
}



int calculate(int mA, int mB)
{
    if (mA > mB) {
        int temp = mA;
        mA = mB;
        mB = temp;
    }

    int row1 = mA / SUBSET;
    int col1 = mA % SUBSET;
    int row2 = mB / SUBSET;
    int col2 = mB % SUBSET;

    int ans = 0;

    if (row1 == row2) {
        ans += roads[row1].getSubByRange(col1, col2);
    }
    else {
        ans += roads[row1].getSubByRange(col1, SUBSET);
        ans += roads[row2].getSubByRange(0, col2);

    }

    for (int i = row1 + 1; i < row2; i++) {
        ans += roads[i].subTotal;
    }
    
    return ans;
}
