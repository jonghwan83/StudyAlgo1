#define ABS(x) ((x<0)?(-(x)):(x))

static const int MAP_SIZE = 100;

static const int ANTENNA_NUM = 150;

static const int ANTENNA_CAPA = 100;

static const int UE_NUM = 10'000;

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

int min(int a, int b) {
    if (a > b) { return b; }
    return a;
}

struct Coordinates

{

    int y, x;



    Coordinates()

    {

        y = x = 0;

    }

    Coordinates(int _y, int _x)

    {

        y = _y;

        x = _x;

    }



    int operator-(Coordinates& param)

    {

        return ABS(y - param.y) + ABS(x - param.x);

    }

};

struct HeapNode {
    int row;
    int col;
    int id;
};

class Heap {
public:
    int length;
    HeapNode arr[ANTENNA_NUM];
    Coordinates loc;

    void init() { length = 0; }

    bool compare(int parent, int child) {
        Coordinates p = Coordinates(arr[parent].row, arr[parent].col);
        Coordinates c = Coordinates(arr[child].row, arr[child].col);
        if (loc - p > loc - c) { return true; }
        return false;
    }

    void push(int r, int c, int i) {
        HeapNode node = { r, c, i };

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


Heap pQueue[MAP_SIZE][MAP_SIZE];

int cnt[ANTENNA_NUM];


void init(Coordinates antenna_list[])
{
    for (int r = 0; r < MAP_SIZE; r++) {
        for (int c = 0; c < MAP_SIZE; c++) {

            pQueue[r][c].init();
            pQueue[r][c].loc = Coordinates(r, c);

            for (int i = 0; i < ANTENNA_NUM; i++) {
                pQueue[r][c].push(antenna_list[i].y, antenna_list[i].x, i);
            }

        }
    }

}



void scanUE(Coordinates UE_list[], int antenna_range[], int assign_antenna[])
{

    HeapNode temp[ANTENNA_NUM];
    int tIdx;

    for (int i = 0; i < ANTENNA_NUM; i++) {
        cnt[i] = 0;
    }

    for (int i = 0; i < UE_NUM; i++) {

        tIdx = 0;

        int row = UE_list[i].y; int col = UE_list[i].x;

        while (pQueue[row][col].length > 0) {
            HeapNode curr = pQueue[row][col].pop();

            temp[tIdx++] = curr;

            if (cnt[curr.id] >= ANTENNA_CAPA) { continue; }

            cnt[curr.id]++;

            assign_antenna[i] = curr.id;

            Coordinates ant = Coordinates(curr.row, curr.col);

            antenna_range[curr.id] = max(antenna_range[curr.id], pQueue[row][col].loc - ant + 4);
            antenna_range[curr.id] = min(antenna_range[curr.id], 199);

            break;
        }

        for (int t = 0; t < tIdx; t++) {
            pQueue[row][col].push(temp[t].row, temp[t].col, temp[t].id);
        }

    }

}