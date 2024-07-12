// #include <iostream>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

static const int MAP_SIZE = 1'000;

static const int SPACESHIP_NUM = 10'000;

static const int MAX_LOC = 1'000;

struct Spaceship
{

    int height;

    int width;
};

struct Point

{

    int y, x;
};

struct Node

{

    int value;

    int num;
};

class Heap

{

public:
    int length;

    Node arr[SPACESHIP_NUM];

    void init() { length = 0; }

    bool compare(int parent, int child)

    {

        if (arr[parent].value < arr[child].value)

        {

            return true;
        }

        return false;
    }

    void push(int s, int num)

    {

        Node node = {s, num};

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

        Node node = arr[0];

        arr[0] = arr[--length];

        int idx = 0;

        int left, right, child;

        while (2 * idx + 1 < length)

        {

            left = 2 * idx + 1;

            right = 2 * idx + 2;

            if (right < length)

                if (compare(left, right))

                {

                    child = right;
                }

                else
                {
                    child = left;
                }

            else
            {
                child = left;
            }

            if (compare(idx, child))

            {

                Node temp = arr[idx];

                arr[idx] = arr[child];

                arr[child] = temp;

                idx = child;
            }

            else
            {
                break;
            }
        }

        return node;
    }
};

Spaceship spaceships[SPACESHIP_NUM];

Heap ship_by_size;

Point pos_on_map[4][4][MAX_LOC]; // 2, 3, 4, 5

int pos_length[4][4];

int pos_idx[4][4];

bool occupied_by_ship[MAP_SIZE][MAP_SIZE];

int offset;

void getPossibleLocation(int width, int height, int map[][MAP_SIZE])

{

    int off_height = height - offset;

    int off_width = width - offset;

    int dx = width - 1;

    int dy = height - 1;

    for (int y = 0; y < MAP_SIZE; y++)

        for (int x = 0; x < MAP_SIZE; x++)

        {

            int y2 = y + dy;

            int x2 = x + dx;

            if (y2 >= MAP_SIZE || x2 >= MAP_SIZE)

                continue;

            int lo = MIN(MIN(MIN(map[y][x], map[y][x2]), map[y2][x]), map[y2][x2]);

            int hi = MAX(MAX(MAX(map[y][x], map[y][x2]), map[y2][x]), map[y2][x2]);

            if (hi - lo > 6)
            {
                continue;
            }

            pos_on_map[off_height][off_width][pos_length[off_height][off_width]++] = {y, x};
        }
}

bool checkLand(int x1, int y1, int x2, int y2)
{

    for (int y = y1; y <= y2; y++)

        for (int x = x1; x <= x2; x++)

            if (occupied_by_ship[y][x])
            {
                return false;
            }

    return true;
}

void landShip(int x1, int y1, int x2, int y2)

{

    for (int y = y1; y <= y2; y++)

        for (int x = x1; x <= x2; x++)

            occupied_by_ship[y][x] = true;
}

void init(int mMap[][MAP_SIZE], struct Spaceship mSpaces[])

{

    offset = 2;

    for (int y = 0; y < 4; y++)

        for (int x = 0; x < 4; x++)

            pos_length[y][x] = 0;

    ship_by_size.init();

    for (int y = 0; y < MAP_SIZE; y++)

        for (int x = 0; x < MAP_SIZE; x++)

        {

            occupied_by_ship[y][x] = false;
        }

    for (int s = 0; s < SPACESHIP_NUM; s++)

    {

        spaceships[s] = mSpaces[s];

        ship_by_size.push(mSpaces[s].height * mSpaces[s].width * MIN(mSpaces[s].height, mSpaces[s].width), s);
    }

    for (int y = 2; y < 6; y++)

        for (int x = 2; x < 6; x++)

        {

            getPossibleLocation(y, x, mMap);
        }

    // int sum = 0;

    // for (int i = 0; i < 4; i++)

    //     for (int j = 0; j < 4; j++)

    //         sum += pos_length[i][j];

    // printf("max position: %d\n", sum);
}

void process(int mRows[], int mCols[], int mDirs[])

{

    for (int i = 0; i < 4; i++)

        for (int j = 0; j < 4; j++)

            pos_idx[i][j] = 0;

    while (ship_by_size.length > 0)

    {

        Node curr = ship_by_size.pop();

        int off_height = spaceships[curr.num].height - offset;

        int off_width = spaceships[curr.num].width - offset;

        int idx = pos_idx[off_height][off_width]++;

        if (idx < pos_length[off_height][off_width])

        {

            int y1 = pos_on_map[off_height][off_width][idx].y;

            int x1 = pos_on_map[off_height][off_width][idx].x;

            int y2 = y1 + spaceships[curr.num].height - 1;

            int x2 = x1 + spaceships[curr.num].width - 1;

            if (checkLand(x1, y1, x2, y2))

            {

                landShip(x1, y1, x2, y2);

                mRows[curr.num] = y1;

                mCols[curr.num] = x1;

                mDirs[curr.num] = 0;
            }
        }

        else

        {
            idx = pos_length[off_width][off_height];

            if (idx < pos_length[off_width][off_height])
            {
                int y1 = pos_on_map[off_width][off_height][idx].y;

                int x1 = pos_on_map[off_width][off_height][idx].x;

                int y2 = y1 + spaceships[curr.num].width - 1;

                int x2 = x1 + spaceships[curr.num].height - 1;

                if (checkLand(x1, y1, x2, y2))

                {

                    landShip(x1, y1, x2, y2);

                    mRows[curr.num] = y1;

                    mCols[curr.num] = x1;

                    mDirs[curr.num] = 1;
                }
            }
        }
    }
}
