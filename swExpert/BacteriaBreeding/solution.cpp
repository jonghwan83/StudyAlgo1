struct Result

{

    int row;

    int col;

};



struct Bacteria

{

    int id;

    int size;

    int time;

};

#define PERIMETER 201
#define MAXARR 3001

struct HeapNode {
    int score;
    int first, second;

    HeapNode() {
        score = 0; first = 0; second = 0;
    }
};

class Heap {
public:
    int length;
    HeapNode arr[MAXARR];

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].score > arr[child].score) { return true; }
        if (arr[parent].score == arr[child].score && arr[parent].first > arr[child].first) { return true; }
        if (arr[parent].score == arr[child].score && arr[parent].first == arr[child].first &&
            arr[parent].second > arr[child].second) {
            return true;
        }

        return false;
    }


    void push(int score, int first, int second) {
        HeapNode node;
        node.score = score; node.first = first; node.second = second;

        int idx = length;
        arr[length++] = node;

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

        while (2 * idx + 1 < length) {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

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

struct Microbe {

    int size, lifespan, born;
    bool isRemoved;

    void init() {
        isRemoved = false;
        size = 0; lifespan = 0; born = 0;
    }

};


int abs(int a) {

    if (a < 0) { return -a; }
    return a;
}

int manhattan(int row1, int col1, int row2, int col2) {

    return (abs(row2 - row1) + abs(col2 - col1));
}


int n;
int visited[PERIMETER][PERIMETER];
int nVisited;

int grid[PERIMETER][PERIMETER];
Microbe microbes[MAXARR];

int drows[4] = { -1, 0, 0, 1 };
int dcols[4] = { 0, -1, 1, 0 };

Heap timeHeap;
Heap queue;

void updateTime(int mTime) {

    while (timeHeap.arr[0].score <= mTime && timeHeap.length > 0) {
        HeapNode curr = timeHeap.pop();

        if (microbes[curr.first].isRemoved) { continue; }

        microbes[curr.first].isRemoved = true;
    }

}

void init(int N)

{

    n = N;

    nVisited = 0;

    timeHeap.init();

    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {

            grid[i][j] = 0;
            visited[i][j] = 0;

        }
    }

}


Result pushMicrobe(int row, int col, int id) {
    

    nVisited++;

    int k = microbes[id].size;

    queue.init();

    queue.push(0, row, col);
    visited[row][col] = nVisited;

    Result ans = { 0, 0 };

    while (queue.length > 0 && k > 0) {

        HeapNode curr = queue.pop();

        if (grid[curr.first][curr.second] > 0 && !microbes[grid[curr.first][curr.second]].isRemoved) { continue; }

        grid[curr.first][curr.second] = id; k--;
        if (k == 0) {
            ans = { curr.first, curr.second };
            break;
        }
        
        for (int i = 0; i < 4; i++) {

            int dr = curr.first + drows[i];
            int dc = curr.second + dcols[i];
               
            if (dr < 1 || dc < 1) { continue; }
            if (dr > n || dc > n) { continue; }
            if (grid[dr][dc] > 0 && !microbes[grid[dr][dc]].isRemoved) { continue; }
            if (visited[dr][dc] >= nVisited) { continue; }


            queue.push(manhattan(row, col, dr, dc), dr, dc);
            visited[dr][dc] = nVisited;
        }

    }


    if (k > 0) {
        microbes[id].isRemoved = true;
    }

    return ans;
}



Result putBacteria(int mTime, int mRow, int mCol, Bacteria mBac)

{
    microbes[mBac.id].init();

    updateTime(mTime);

    microbes[mBac.id].born = mTime;
    microbes[mBac.id].lifespan = mBac.time;
    microbes[mBac.id].size = mBac.size;

    timeHeap.push(mTime + mBac.time, mBac.id, 0);

    Result ret = pushMicrobe(mRow, mCol, mBac.id);

    return ret;

}



int killBacteria(int mTime, int mRow, int mCol)

{
    updateTime(mTime);

    int ans = 0;

    if (grid[mRow][mCol] > 0 && !microbes[grid[mRow][mCol]].isRemoved) {
        microbes[grid[mRow][mCol]].isRemoved = true;
        ans = grid[mRow][mCol];
    }

    return ans;

}



int checkCell(int mTime, int mRow, int mCol)

{

    updateTime(mTime);

    if (microbes[grid[mRow][mCol]].isRemoved) { return 0; }

    return grid[mRow][mCol];

}
