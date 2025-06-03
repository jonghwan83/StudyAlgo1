#define MAXWIDTH 50000

int min(int a, int b)
{
    return a < b? a : b;
}
int max(int a, int b)
{
    return a > b? a : b;
}

struct Node
{
    int data;
    Node* next;
    
    Node(int a)
    {
        data = a;
        next = nullptr;
    }
};

class LinkedList
{
public:
    Node* head;
    int length;
    
    void init()
    {
        head = nullptr;
        length = 0;
    }
    
    void addNode(int a)
    {
        Node* newNode = new Node(a);
        
        newNode->next = head;
        head = newNode;
        length++;
    }
};


int valley[MAXWIDTH];
LinkedList steps[10][10][10][10];



void buildTank(int mLen, int mTank[], int start)
{
    for (int i = 0; i < mLen; i++)
    {
        valley[i + start] += mTank[i];
    }
    
}

void removeTank(int mLen, int mTank[], int start)
{
    for (int i = 0; i < mLen; i++)
    {
        valley[i + start] -= mTank[i];
    }
}

int getLocalMinima(int curr, int dx, int maxHeight)
{
    int localMinima = curr;
    
    bool isNegativeSlope = false;
    
    while (true)
    {
        curr = curr + dx;
        
        if (valley[localMinima] > valley[curr])
        {
            localMinima = curr;
        }
        
        if (valley[curr + dx] > valley[curr])
        {
            isNegativeSlope = true;
        }
        
        if (isNegativeSlope && valley[curr + dx] < valley[curr]) break;
        if (valley[curr] >= maxHeight) break;
        
    }
    
    return localMinima;
}

int simulate(int localMinima, int water, int maxHeight, int start)
{
    int space = 0;
    int lowest = valley[localMinima];
    
    bool isFlood = false;
    
    int L = localMinima - 1;
    int R = localMinima + 1;
    
    for (int depth = lowest; depth < maxHeight; depth++)
    {
        while (valley[L] <= depth)
        {
            L--;
            if (valley[L] < valley[L + 1])
            {
                isFlood = true;
                break;
            }
        }
        
        while (valley[R] <= depth)
        {
            R++;
            if (valley[R] < valley[R - 1])
            {
                isFlood = true;
                break;
            }
        }
        
        if (isFlood) break;
        
        int moreSpace = R - L - 1;
        
        if (space + moreSpace < water)
        {
            space += moreSpace;
        }
        else if (space + moreSpace == water)
        {
            return space + moreSpace;
        }
        else
        {
            return space;
        }
    }
    
    return space;
}

void init(int N, int mHeight[])
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            for (int k = 0; k < 10; k++)
                for (int l = 0; l < 10; l++)
                    steps[i][j][k][l].init();
    
    valley[0] = mHeight[0];
    valley[N - 1] = mHeight[N - 1];
    
    for (int i = 1; i < N - 1; i++)
    {
        valley[i] = mHeight[i];
        
        int diff[4] = { 0, 0, 0, 0 };
        
        diff[0] = (150 - mHeight[i + 1]) - (150 - mHeight[i]) + 5;
        if (diff[0] < 1 || diff[0] > 9) continue;
        
        diff[1] = (150 - mHeight[i + 2]) - (150 - mHeight[i + 1]) + 5;
        if (diff[1] < 1 || diff[1] > 9) continue;
        
        steps[diff[0]][diff[1]][0][0].addNode(i);
        
        if (i + 3 > N - 1) continue;
        diff[2] = (150 - mHeight[i + 3]) - (150 - mHeight[i + 2]) + 5;
        if (diff[2] < 1 || diff[2] > 9) continue;
        
        steps[diff[0]][diff[1]][diff[2]][0].addNode(i);
        
        if (i + 4 > N - 1) continue;
        diff[3] = (150 - mHeight[i + 4]) - (150 - mHeight[i + 3]) + 5;
        if (diff[3] < 1 || diff[3] > 9) continue;
        
        steps[diff[0]][diff[1]][diff[2]][diff[3]].addNode(i);
    }
    
}

int countPosition(int mLen, int mTank[])
{
    int res = 0;
    
    int diff[4] = { 0, 0, 0, 0};
    
    for (int i = 0; i < mLen - 1; i++)
    {
        diff[i] = mTank[i + 1] - mTank[i] + 5;
    }
    
    res = steps[diff[0]][diff[1]][diff[2]][diff[3]].length;
    
    return res;
}

int buildAndPourOut(int mLen, int mTank[], int mWater)
{
    int diff[4] = { 0, 0, 0, 0};
    
    for (int i = 0; i < mLen - 1; i++)
    {
        diff[i] = mTank[i + 1] - mTank[i] + 5;
    }
    
    Node* st = steps[diff[0]][diff[1]][diff[2]][diff[3]].head;
    
    int maxFillled = 0;
    while (st)
    {
        buildTank(mLen, mTank, st->data);
        int topTank = valley[st->data];
        
        int leftLocalMinima = getLocalMinima(st->data, -1, topTank);
        int rightLocalMinima = getLocalMinima(st->data + mLen - 1, 1, topTank);
        
        if (valley[st->data - 1] < valley[st->data])
        {
            maxFillled = max(maxFillled, simulate(leftLocalMinima, mWater, topTank, st->data));
        }
        
        if (valley[st->data + mLen] < valley[st->data + mLen - 1])
        {
            maxFillled = max(maxFillled, simulate(rightLocalMinima, mWater, topTank, st->data + mLen - 1));
        }
        
        removeTank(mLen, mTank, st->data);
        st = st->next;
    }
    return maxFillled;
}
