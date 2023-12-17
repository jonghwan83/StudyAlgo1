#define KIND 6
#define MAXN 5000
#define SUBN 1000

int max(int a, int b)
{
    if (a > b) { return a; }
    return b;
}

int min(int a, int b)
{
    if (a < b) { return a; }
    return b;
}

int abs(int a)
{
    if (a < 0) { return -a; }
    return a;
}

int manhattan(int r1, int c1, int r2, int c2)
{
    return (abs(r1 - r2) + abs(c1 - c2));
}

struct Node
{
    Node* prev;
    Node* next;
    int row;
    int col;
    int color;

    Node(int r, int c, int cl)
    {
        row = r; col = c; color = cl;
        prev = nullptr; next = nullptr;
    }
};

class LinkedList
{
public:
    int length;

    Node* head;
    Node* tail;

    void init()
    {
        Node* h = new Node(-1, -1, -1);
        Node* t = new Node(-1, -1, -1);

        head = h; tail = t;

        head->next = tail;
        tail->prev = head;

        length = 0;
    }

    void push(int row, int col, int color)
    {
        Node* node = new Node(row, col, color);

        node->prev = head;
        node->next = head->next;

        head->next->prev = node;
        head->next = node;

        length++;
    }

    int pop(int row, int col)
    {
        Node* node = head;

        while (node)
        {
            if (row == node->row && col == node->col)
            {
                Node* prev = node->prev;
                Node* next = node->next;

                prev->next = next;
                next->prev = prev;

                length--;

                return node->color;
            }
            node = node->next;
        }

        return -1;
    }

    int subTotal(int r1, int c1, int r2, int c2, int color)
    {
        int ans = 0;

        Node* node = head->next;

        while (node != tail)
        {
            if (r1 <= node->row && node->row <= r2 &&
                c1 <= node->col && node->col <= c2)
            {
                ans++;
            }
            node = node->next;
        }

        return ans;
    }

    int find(int r, int c, int cl)
    {
        Node* node = head->next;

        int ans = MAXN * MAXN;

        while (node != tail)
        {
            if (cl == 0)
                ans = min(ans, manhattan(r, c, node->row, node->col));
            if (cl > 0 && cl == node->color)
                ans = min(ans, manhattan(r, c, node->row, node->col));
            node = node->next;
        }

        return ans;
    }
};

int n;

int n_sub;

LinkedList map[KIND][MAXN / SUBN + 1][MAXN / SUBN + 1];


void init(int N)
{
    n = N;

    n_sub = N / SUBN;

    for (int r = 0; r <= n_sub; r++)
        for (int c = 0; c <= n_sub; c++)
        {
            for (int k = 0; k < KIND; k++)
                map[k][r][c].init();
        }

}

void buildTower(int mRow, int mCol, int mColor)
{
    mRow--; mCol--;

    int r_sub = mRow / SUBN;
    int c_sub = mCol / SUBN;

    map[mColor][r_sub][c_sub].push(mRow, mCol, mColor);
    map[0][r_sub][c_sub].push(mRow, mCol, mColor);
}

void removeTower(int mRow, int mCol)
{
    mRow--; mCol--;

    int r_sub = mRow / SUBN;
    int c_sub = mCol / SUBN;

    int color = map[0][r_sub][c_sub].pop(mRow, mCol);
    if (color > -1)
        map[color][r_sub][c_sub].pop(mRow, mCol);
}


int countTower(int mRow, int mCol, int mColor, int mDis)
{
    mRow--; mCol--;

    int ans = 0;

    int r1_sub = max(0, (mRow - mDis) / SUBN);
    int c1_sub = max(0, (mCol - mDis) / SUBN);

    int r2_sub = min(n_sub, (mRow + mDis) / SUBN);
    int c2_sub = min(n_sub, (mCol + mDis) / SUBN);

    for (int r = r1_sub; r <= r2_sub; r++)
        for (int c = c1_sub; c <= c2_sub; c++)
        {
            if (r == r1_sub || r == r2_sub || c == c1_sub || c == c2_sub)
                ans += map[mColor][r][c].subTotal(
                        max(0, mRow - mDis), max(0, mCol - mDis),
                        min(n - 1, mRow + mDis), min(n - 1, mCol + mDis),
                        mColor);
            else
                ans += map[mColor][r][c].length;
        }

    return ans;
}

int getClosest(int mRow, int mCol, int mColor) {
    mRow--;
    mCol--;

    int ans = MAXN * MAXN;

    for (int r = 0; r <= n_sub; r++)
        for (int c = 0; c <= n_sub; c++)
            ans = min(ans, map[mColor][r][c].find(mRow, mCol, mColor));

    if (ans == MAXN * MAXN)
        return -1;
    return ans;
}
