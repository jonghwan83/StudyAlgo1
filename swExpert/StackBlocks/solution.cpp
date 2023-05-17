struct Result {

    int top;

    int count;

};

#define SUBSET 1000
#define MAXROW 10000

int colSize;

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

class Grid {
public:
    int height[SUBSET];
    int minHeight;
    int maxHeight;
    int length;
    int cnt;

    void init() {
        cnt = 0;
        minHeight = 0;
        maxHeight = 0;
        for (int i = 0; i < SUBSET; i++) {
            height[i] = 0;
        }
    }

    void push(int st, int ed, int mHeight) {
        for (int i = st; i < ed; i++) {
            height[i] += mHeight;
            maxHeight = max(maxHeight, height[i] + cnt);
        }

        minHeight = MAXROW + 1;
        for (int i = 0; i < length; i++) {
            minHeight = min(minHeight, height[i] + cnt);
        }
    }
};

Grid grids[SUBSET + 1];
long long int total;
int gridSize;


void init(int C)

{
    total = 0;
    colSize = C;
    gridSize = C / SUBSET;

    for (int i = 0; i <= SUBSET; i++) {
        grids[i].init();
        grids[i].length = min(C, SUBSET);
    }
}

Result dropBlocks(int mCol, int mHeight, int mLength)

{

    Result ret;

    ret.top = 0;

    ret.count = 0;

    total += mHeight * mLength;


    int st = mCol / SUBSET;
    int ed = (mCol + mLength) / SUBSET;

    for (int i = st + 1; i < ed; i++) {
        grids[i].cnt += mHeight;
        grids[i].minHeight += mHeight;
        grids[i].maxHeight += mHeight;
    }

    int sIdx = mCol % SUBSET;
    int eIdx = (mCol + mLength - 1) % SUBSET + 1;

    if (st == ed) {
        grids[st].push(sIdx, eIdx, mHeight);
    }
    else {
        grids[st].push(sIdx, SUBSET, mHeight);
        grids[ed].push(0, eIdx, mHeight);
    }

    int top = 0;
    int bottom = MAXROW + 1;
    for (int i = 0; i <= gridSize; i++) {
        top = max(top, grids[i].maxHeight);
        bottom = min(bottom, grids[i].minHeight);
    }


    ret.top = top - bottom;
    ret.count = (total - colSize * (long long int) bottom) % 1000000;

    return ret;

}
