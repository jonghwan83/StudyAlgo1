#define MAXWORD 50000
#define MAXROW 20000
#define MAXPAGE 100
#define WORDINROW 120
#define SUBROW 200

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

int N;

struct HeapNode
{
    int st;
    int ed;
};

class Heap
{
public:
    int length;
    HeapNode arr[WORDINROW];

    void init()
    {
        length = 0;
    }

    bool compare(int parent, int child)
    {
        if (arr[parent].st > arr[child].st)
            return true;
        return false;
    }

    void push(int st, int ed)
    {
        HeapNode node = { st, ed };

        int idx = length;
        arr[length++] = node;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx))
        {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    HeapNode pop()
    {
        HeapNode ans = arr[0];

        arr[0] = arr[--length];

        int left, right, child;
        int idx = 0;

        while (2 * idx + 1 < length)
        {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length)
                if (compare(left, right))
                    child = right;
                else
                    child = left;
            else
                child = left;

            if (compare(idx, child))
            {
                HeapNode temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else
                break;
        }

        return ans;
    }

};

struct Word
{
    int st;
    int ed;
    int id;
    bool isRemoved;
};

struct Row
{
    Word word[WORDINROW];
    int len;
    int maxEmpty;
    Heap empties;

    void add(int st, int ed, int id)
    {
        word[len].st = st; word[len].ed = ed;
        word[len].id = id; word[len].isRemoved = false;
        len++;
    }

    void updateMax()
    {
        maxEmpty = 0;
        for (int i = 0; i < empties.length; i++)
            maxEmpty = max(maxEmpty, empties.arr[i].ed - empties.arr[i].st);
    }

    int findById(int mId)
    {
        for (int i = 0; i < len; i++)
        {
            if (!word[i].isRemoved && word[i].id == mId)
                return i;
        }

        return -1;
    }

    void addEmpty(int st, int ed)
    {
        if (ed - st <= 0) { return; }

        HeapNode temp[WORDINROW];
        int tIdx = 0;

        empties.push(st, ed);

        while (empties.length > 0)
        {
            HeapNode curr = empties.pop();

            if (tIdx > 0 && temp[tIdx - 1].ed == curr.st)
            {
                temp[tIdx - 1].ed = curr.ed;
                continue;
            }
            temp[tIdx++] = curr;

        }

        for (int i = 0; i < tIdx; i++)
            empties.push(temp[i].st, temp[i].ed);
    }

    void push(int mId, int mLen)
    {
        HeapNode temp[WORDINROW];
        int tIdx = 0;

        int st = 0; int ed = 0;

        while (empties.length > 0)
        {
            HeapNode curr = empties.pop();

            if (curr.ed - curr.st >= mLen)
            {
                st = curr.st + mLen; ed = curr.ed;
                add(curr.st, curr.st + mLen, mId);

                if (ed - st > 0)
                {
                    temp[tIdx].st = st; temp[tIdx].ed = ed;
                    tIdx++;
                }
                
                break;
            }
            temp[tIdx++] = curr;
        }

        for (int i = 0; i < tIdx; i++)
            empties.push(temp[i].st, temp[i].ed);
        
    }

};

class Page
{
public:
    Row rows[MAXROW / MAXPAGE];
    int maxEmpty;

    void init(int M)
    {
        for (int i = 0; i < MAXROW / MAXPAGE; i++)
        {
            rows[i].len = 0;
            rows[i].empties.init();
            rows[i].empties.push(0, M);
            rows[i].maxEmpty = M;
        }
    }

    void updateMax()
    {
        maxEmpty = 0;
        for (int i = 0; i < MAXROW / MAXPAGE; i++)
            maxEmpty = max(maxEmpty, rows[i].maxEmpty);

    }

    int getRow(int mLen)
    {
        for (int i = 0; i < MAXROW / MAXPAGE; i++)
        {
            if (rows[i].maxEmpty >= mLen)
                return i;
        }

        return -1;
    }

};



//////////////////////////////////////////////////////////////////////


Page pages[MAXPAGE + 1];

int hashID[MAXWORD + 1];

void init(int _N, int M)
{
    N = _N;

    for (int i = 0; i < N / SUBROW; i++)
    {
        pages[i].init(M);
        pages[i].maxEmpty = M;
    }

    for (int i = 0; i < N % SUBROW; i++)
    {
        pages[N / SUBROW].rows[i].len = 0;
        pages[N / SUBROW].rows[i].empties.init();
        pages[N / SUBROW].rows[i].empties.push(0, M);
        pages[N / SUBROW].rows[i].maxEmpty = M;
    }

    pages[N / SUBROW].maxEmpty = M;

    for (int i = 0; i < MAXWORD; i++)
        hashID[i] = -1;
        
}


int writeWord(int mId, int mLen)
{
    int p = 0;

    while (p <= N / SUBROW)
    {
        if (pages[p].maxEmpty >= mLen)
        {
            int sr = pages[p].getRow(mLen);
            if (sr == -1) { return -1; }

            pages[p].rows[sr].push(mId, mLen);

            pages[p].rows[sr].updateMax();
            pages[p].updateMax();

            hashID[mId] = p * SUBROW + sr;
            
            return p * SUBROW + sr;
        }
        p++;
    }

    return -1;
}


int eraseWord(int mId)
{
    int res = hashID[mId];

    int p = hashID[mId] / SUBROW;
    int sr = hashID[mId] % SUBROW;

    int idx = pages[p].rows[sr].findById(mId);

    if (idx == -1) { return -1; }

    pages[p].rows[sr].word[idx].isRemoved = true;
    pages[p].rows[sr].addEmpty(pages[p].rows[sr].word[idx].st, pages[p].rows[sr].word[idx].ed);

    pages[p].rows[sr].updateMax();
    pages[p].updateMax();

    hashID[mId] = -1;
    

    return res;
}
