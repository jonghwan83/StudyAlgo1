#define MAXPERIMETER 10000
#define MAXRECT 10000
#define PERIMETER 100

class HeapNode {
public:
    int z;
    int idx;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXRECT];

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].z < arr[child].z) { return true; }
        return false;
    }

    void push(int stack, int i) {
        HeapNode last;
        last.z = stack, last.idx = i;

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    HeapNode pop() {
        HeapNode ans = arr[0];
        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;

        while (idx * 2 + 1 < length) {
            left = idx * 2 + 1;
            right = idx * 2 + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                HeapNode temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }

        return ans;
    }
};

struct Point
{
    int x;
    int y;
};



class Rectangle {
public:
    int id;
    Point p1;
    Point p2;
    int z;
    bool isRemoved;

    void push(int mID, int mX, int mY, int mHeight, int mWidth, int zaxis) {
        id = mID;
        p1.x = mX;
        p1.y = mY;
        p2.x = mX + mWidth - 1;
        p2.y = mY + mHeight - 1;
        z = zaxis;

        isRemoved = false;
    }
};



Rectangle rects[MAXRECT * 2 + 1];
Heap pQueue[PERIMETER + 2][PERIMETER + 2];
int hashIdx[MAXRECT + 1];
int idx, zAxis;



int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

bool checkOverlap (int idx1, int idx2) {
    int x1_max = max(rects[idx1].p1.x, rects[idx2].p1.x);
    int x2_min = min(rects[idx1].p2.x, rects[idx2].p2.x);

    int y1_max = max(rects[idx1].p1.y, rects[idx2].p1.y);
    int y2_min = min(rects[idx1].p2.y, rects[idx2].p2.y);

    return x1_max <= x2_min && y1_max <= y2_min;
}

bool checkPoint(int idx, int x, int y) {
    if (rects[idx].p1.x <= x && x <= rects[idx].p2.x && rects[idx].p1.y <= y && y <= rects[idx].p2.y) {
        return true;
    }

    return false;
}

	
void init(int N)
{

    idx = 0;
    zAxis = 1;

    for (int i = 0; i < PERIMETER + 2; i++) {
        for (int j = 0; j < PERIMETER + 2; j++) {
            pQueue[i][j].init();
        }
    }

    for (int i = 0; i < MAXRECT + 1; i ++) {
        hashIdx[i] = -1;
    }
}



void addRect(int mID, int mY, int mX, int mHeight, int mWidth)
{

    hashIdx[mID] = idx;

    rects[idx].push(mID, mX, mY, mHeight, mWidth, zAxis++);

    int row1 = rects[idx].p1.y / PERIMETER;
    int row2 = rects[idx].p2.y / PERIMETER;
    int col1 = rects[idx].p1.x / PERIMETER;
    int col2 = rects[idx].p2.x / PERIMETER;

    for (int i = row1; i <= row2; i++) {
        for (int j = col1; j <= col2; j++) {
            pQueue[i][j].push(rects[idx].z, idx);
        }
    }

    idx++;
}




int moveFront(int mID)
{
    int cIdx = hashIdx[mID];
    if (cIdx == -1) { return 0; }
    if (rects[cIdx].isRemoved) { return 0; }

    int row1 = rects[cIdx].p1.y / PERIMETER;
    int row2 = rects[cIdx].p2.y / PERIMETER;
    int col1 = rects[cIdx].p1.x / PERIMETER;
    int col2 = rects[cIdx].p2.x / PERIMETER;

    int z = 0;
    int ansIdx = -1;
    bool check = false;
    

    for (int i = row1; i <= row2; i++) {
        for (int j = col1; j <= col2; j++) {
            
            int tIdx = 0;
            HeapNode temp[MAXRECT];

            while (pQueue[i][j].length > 0) {
                HeapNode curr = pQueue[i][j].pop();

                if (rects[curr.idx].isRemoved) { continue; }

                temp[tIdx++] = curr;

                if (curr.idx == cIdx) { continue; }
                if (rects[curr.idx].z < z) { break; }

                if (checkOverlap(cIdx, curr.idx)) {
                    if (rects[curr.idx].z > z) {
                        z = rects[curr.idx].z;
                        ansIdx = curr.idx;
                        check = true;
                        break;
                    }
                }
            }

            for (int k = 0; k < tIdx; k++) {
                pQueue[i][j].push(temp[k].z, temp[k].idx);
            }

        }
    }

    rects[cIdx].isRemoved = true;

    int mHeight = rects[cIdx].p2.y - rects[cIdx].p1.y + 1;
    int mWidth = rects[cIdx].p2.x - rects[cIdx].p1.x + 1;

    addRect(rects[cIdx].id, rects[cIdx].p1.y, rects[cIdx].p1.x, mHeight, mWidth);

    if (check) { return rects[ansIdx].id; }
    return 0;
}



int check(int mY, int mX)
{
    int row = mY / PERIMETER;
    int col = mX / PERIMETER;

    int tIdx = 0;
    HeapNode temp[MAXRECT];
    HeapNode curr;

    int ans = 0;

    while (pQueue[row][col].length > 0) {
        curr = pQueue[row][col].pop();

        if (rects[curr.idx].isRemoved) { continue; }

        temp[tIdx++] = curr;

        if (checkPoint(curr.idx, mX, mY)) {
            ans = rects[curr.idx].id;
            break;
        }
    }

    for (int i = 0; i < tIdx; i++) {
        pQueue[row][col].push(temp[i].z, temp[i].idx);
    }

    return ans;
}

int selectAndErase(int mY, int mX)
{
    int mID = check(mY, mX);

    if (mID == 0) { return 0; }

    int cIdx = hashIdx[mID];

    if (rects[cIdx].isRemoved) { return 0; }

    rects[cIdx].isRemoved = true;
    
    return rects[cIdx].id;
}


void selectAndMove(int y1, int x1, int y2, int x2)
{
    int mID = check(y1, x1);

    if (mID == 0) { return; }

    int cIdx = hashIdx[mID];

    if (rects[cIdx].isRemoved) { return; }

    rects[cIdx].isRemoved = true;

    int mHeight = rects[cIdx].p2.y - rects[cIdx].p1.y + 1;
    int mWidth = rects[cIdx].p2.x - rects[cIdx].p1.x + 1;

    rects[idx].push(rects[cIdx].id, x2, y2, mHeight, mWidth, rects[cIdx].z);

    hashIdx[mID] = idx;

    int row1 = rects[idx].p1.y / PERIMETER;
    int row2 = rects[idx].p2.y / PERIMETER;
    int col1 = rects[idx].p1.x / PERIMETER;
    int col2 = rects[idx].p2.x / PERIMETER;

    for (int i = row1; i <= row2; i++) {
        for (int j = col1; j <= col2; j++) {
            pQueue[i][j].push(rects[idx].z, idx);
        }
    }

    idx++;
}
