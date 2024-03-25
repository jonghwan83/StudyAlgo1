#define ABS(x) ((x<0)?(-(x)):(x))

static const int MAP_SIZE = 100;

static const int ANTENNA_NUM = 150;

static const int ANTENNA_CAPA = 100;

static const int UE_NUM = 10'000;

int max(int a, int b) { return a > b ? a : b; }

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



struct Node
{
    int loss;
    int ue;
};

class Heap
{
public:
    int length;
    Node arr[UE_NUM];

    void init() { length = 0; }

    bool compare(int parent, int child)
    {
        if (arr[parent].loss < arr[child].loss)
            return true;
        return false;
    }

    void push(int dist, int ue)
    {
        Node node = { dist, ue };

        int idx = length;
        arr[length++] = node;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx))
        {
            Node temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    Node pop()
    {
        Node ans = arr[0];

        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;

        while (2 * idx + 1 < length)
        {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length)
                if (compare(left, right)) { child = right; }
                else { child = left; }
            else { child = left; }

            if (compare(idx, child))
            {
                Node temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }

        return ans;
    }
};




Coordinates antenna_list[ANTENNA_NUM];

int capa[ANTENNA_NUM];

int subtask;

Heap pQueue;

void init(Coordinates _antenna_list[])
{
    subtask = 0;

    for (int i = 0; i < ANTENNA_NUM; i++)
    {
        antenna_list[i] = _antenna_list[i];
    }
}



void scanUE(Coordinates UE_list[], int antenna_range[], int assign_antenna[])
{
    for (int i = 0; i < ANTENNA_NUM; i++)
    {
        capa[i] = 0;

        antenna_range[i] = 0;
    }

    pQueue.init();

    for (int ue = 0; ue < UE_NUM; ue++)
    {
        int antenna_id = -1;

        int min_dist = MAP_SIZE * MAP_SIZE * ANTENNA_NUM;

        int min_dist2 = MAP_SIZE * MAP_SIZE * ANTENNA_NUM;

        for (int antenna = 0; antenna < ANTENNA_NUM; antenna++)
        {
            if (capa[antenna] >= ANTENNA_CAPA) { continue; }

            int dist = antenna_list[antenna] - UE_list[ue];

            if (dist < min_dist)
            {
                min_dist = dist;
            }

            if (min_dist < dist && dist < min_dist2)
            {
                min_dist2 = dist;
            }

        }

        pQueue.push(min_dist + min_dist2, ue);

//        assign_antenna[ue] = antenna_id;
//        antenna_range[antenna_id] = max(antenna_range[antenna_id], min_dist + 4);
//        capa[antenna_id]++;
    }

    while (pQueue.length > 0)
    {
        Node curr = pQueue.pop();

        int min_dist = MAP_SIZE * MAP_SIZE;

        int antenna_id = -1;

        for (int antenna = 0; antenna < ANTENNA_NUM; antenna++)
        {
            int dist = antenna_list[antenna] - UE_list[curr.ue];

            if (dist < min_dist && capa[antenna] < ANTENNA_CAPA)
            {
                min_dist = dist;

                antenna_id = antenna;
            }
        }

        assign_antenna[curr.ue] = antenna_id;

        capa[antenna_id]++;
    }


    for (int ue = 0; ue < UE_NUM; ue++)
    {
        int antenna = assign_antenna[ue];

        int dist = antenna_list[antenna] - UE_list[ue];

        if (antenna_range[antenna] < dist + 4)
        {
            antenna_range[antenna] = dist + 4;
        }
    }

    subtask++;
}