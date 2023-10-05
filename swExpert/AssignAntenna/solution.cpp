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
    HeapNode arr[UE_NUM];
    Coordinates antenna;

    void init() { length = 0; }

    bool compare(int parent, int child) {
        Coordinates p = Coordinates(arr[parent].row, arr[parent].col);
        Coordinates c = Coordinates(arr[child].row, arr[child].col);
        if (antenna - p > antenna - c) { return true;}
        return false;
    }

    void push(int r, int c, int i) {
        HeapNode node = {r, c, i };

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


Heap pQueue[ANTENNA_NUM];

int cnt[ANTENNA_NUM];

bool checked[UE_NUM];

int nChecked;

int ans_range[ANTENNA_NUM];
int ans_assign[UE_NUM];


void init(Coordinates antenna_list[])
{


    for (int i = 0; i < ANTENNA_NUM; i++) {
        pQueue[i].init();
        pQueue[i].antenna = antenna_list[i];

        cnt[i] = 0;
    }

    nChecked = 0;

}



void scanUE(Coordinates UE_list[], int antenna_range[], int assign_antenna[])
{

    if (nChecked > 0) {

        for (int i = 0; i < ANTENNA_NUM; i++) {
            antenna_range[i] = ans_range[i];
        }

        for (int i = 0; i < UE_NUM; i++) {
            assign_antenna[i] = ans_assign[i];
        }

        return;

    }

    for (int i = 0; i < UE_NUM; i++) {

        checked[i] = false;

        for (int k = 0; k < ANTENNA_NUM; k++) {

            pQueue[k].push(UE_list[i].y, UE_list[i].x, i);
        }

    }



    while (nChecked < UE_NUM) {

        for (int k = 0; k < ANTENNA_NUM; k++) {

            while (cnt[k] < 100 && pQueue[k].length > 0) {

                HeapNode curr = pQueue[k].pop();

                if (checked[curr.id]) { continue; }

                checked[curr.id] = true;

                nChecked++;

                assign_antenna[curr.id] = k;

                cnt[k]++;

                Coordinates c = Coordinates(curr.row, curr.col);

                antenna_range[k] = max(antenna_range[k], c - pQueue[k].antenna + 45);

                antenna_range[k] = min(antenna_range[k], 199);

                break;

            }

        }

    }

    for (int i = 0; i < ANTENNA_NUM; i++) {
        ans_range[i] = antenna_range[i];
    }

    for (int i = 0; i < UE_NUM; i++) {
        ans_assign[i] = assign_antenna[i];
    }

}