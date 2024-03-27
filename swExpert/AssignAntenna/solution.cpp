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

Coordinates ue_list_backup[UE_NUM];

int capa[ANTENNA_NUM];

int subtask;

int move[UE_NUM];  // 0: up, 1: left, 2: down, 3: right

int drows[5] = { -1, 0, 1, 0, 0 };
int dcols[5] = { 0, -1, 0, 1, 0 };

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

        int min_dist3 = MAP_SIZE * MAP_SIZE * ANTENNA_NUM;

        int min_dist4 = MAP_SIZE * MAP_SIZE * ANTENNA_NUM;

        for (int antenna = 0; antenna < ANTENNA_NUM; antenna++)
        {
            if (capa[antenna] >= ANTENNA_CAPA) { continue; }

            int dist = antenna_list[antenna] - UE_list[ue];

            if (dist < min_dist) { min_dist = dist; }

            else if (min_dist < dist && dist < min_dist2) { min_dist2 = dist; }

            else if (min_dist2 < dist && dist < min_dist3) { min_dist3 = dist; }

            else if (min_dist3 < dist && dist < min_dist4) { min_dist4 = dist; }

        }

        pQueue.push(min_dist + min_dist2 + min_dist3 + min_dist4, ue);

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

        if (subtask == 0)
        {
            ue_list_backup[curr.ue] = UE_list[curr.ue];
        }

        else
        {
            int drow = UE_list[curr.ue].y - ue_list_backup[curr.ue].y;
            
            int dcol = UE_list[curr.ue].x - ue_list_backup[curr.ue].x;

            if (drow < 0 && dcol == 0) { move[curr.ue] = 0; }
            else if (drow == 0 && dcol < 0) { move[curr.ue] = 1; }
            else if (drow > 0 && dcol == 0) { move[curr.ue] = 2; }
            else if (drow == 0 && dcol > 0) { move[curr.ue] = 3; }
            else if (drow == 0 && dcol == 0) { move[curr.ue] = 4; }
            else { move[curr.ue] = -1; }

            ue_list_backup[curr.ue] = UE_list[curr.ue];
        }

    }

    for (int ue = 0; ue < UE_NUM; ue++)
    {
        int antenna = assign_antenna[ue];

        int dist = antenna_list[antenna] - UE_list[ue];
    
        if (subtask == 0 || move[ue] == -1)
        {
            antenna_range[antenna] = max(antenna_range[antenna], dist + 4);
        }
        else
        {
            Coordinates new_one;
            new_one.y = UE_list[ue].y + drows[move[ue]] * 4;
            new_one.x = UE_list[ue].x + dcols[move[ue]] * 4;

            if (new_one.y >= MAP_SIZE || new_one.x >= MAP_SIZE || new_one.y < 0 || new_one.x < 0)
            {
                antenna_range[antenna] = max(antenna_range[antenna], dist + 4);
            }
            else
            {
                dist = max(antenna_list[antenna] - new_one, antenna_list[antenna] - UE_list[ue]);

                antenna_range[antenna] = max(antenna_range[antenna], dist);
            }

        }
        
    }

    subtask++;
}