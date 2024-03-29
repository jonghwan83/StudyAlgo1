
#define MAXN 3001

int abs(int a) { return a < 0? -a: a; }


int N;

int holes[MAXN][MAXN];

int visited[MAXN][MAXN];
int nVisited;

int dist, cid;
int rNext, cNext;

struct Coordinate
{
    int row, col;
};

struct Node
{
    Node* next;
    Coordinate location;
    
    Node(int row, int col)
    {
        location.row = row; location.col = col;
    }
};

class LinkedList
{
public:
    int length;
    Node* head;
    
    void init() { head = nullptr; }
    
    void push(int row, int col)
    {
        Node* node = new Node(row, col);
        
        node->next = head;
        head = node;
        
        length++;
    }
    
    void searchHole(int row, int col)
    {
        Node* node = head;
        
        while (node)
        {
            int dr = node->location.row; int dc = node->location.col;
            
            if (holes[dr][dc] > 0 && visited[dr][dc] < nVisited)
            {
                int newDist = (abs(dr - row) + abs(dc - col)) * 10;
                
                if (newDist < dist)
                {
                    dist = newDist;
                    rNext = dr; cNext = dc;
                }
                else if (newDist == dist && dr < rNext)
                {
                    rNext = dr; cNext = dc;
                }
                else if (newDist == dist && dr == rNext && dc < cNext)
                {
                    rNext = dr; cNext = dc;
                }
            }
            
            node = node->next;
        }
    }
};

LinkedList hashRow[MAXN];
LinkedList hashCol[MAXN];

class Diagonal   // row = -y
{
public:
    Coordinate st, ed; // row, col
    int slope, intercept;  // x, y
    
    bool isOnTheLine(int row, int col)
    {
        int x = col; int y = -row;
        
        int _y = x * slope + intercept;
        
        if (_y != y ) { return false; }
        
        if (col < st.col || col > ed.col) { return false;}
        
        return true;
    }
    
    void searchHole(int row, int col)
    {
        
        for (int x = st.col; x <= ed.col; x++)
        {
            int y = slope * x + intercept;
            int dr = -y; int dc = x;
            
            if (holes[dr][dc] > 0 && visited[dr][dc] < nVisited)
            {
                int newDist = abs(dc - col) * 14;
                
                if (newDist < dist)
                {
                    dist = newDist;
                    rNext = dr; cNext = dc;
                }
                else if (newDist == dist && dr < rNext)
                {
                    rNext = dr; cNext = dc;
                }
                else if (newDist == dist && dr == rNext && dc < cNext)
                {
                    rNext = dr; cNext = dc;
                }
                
            }
            
        }
    }
};


Diagonal diags[MAXN * 2][5];
int idxDiag[MAXN * 2];


void checkDiagonal(int row, int col)
{
    int x = col; int y = -row;
    
    int key1 = abs(y - x);
    int key2 = abs(y + x);
    
    for (int i = 0; i < idxDiag[key1]; i++)
    {
        if (!diags[key1][i].isOnTheLine(row, col)) { continue; }
        
        diags[key1][i].searchHole(row, col);
    }
    
    
    for (int i = 0; i < idxDiag[key2]; i++)
    {
        if (!diags[key2][i].isOnTheLine(row, col)) { continue; }
        
        diags[key2][i].searchHole(row, col);
    }
    
}


void init(int _N)
{
    nVisited = 0;
    
    N = _N;
    
    for (int r = 0; r <= N; r++)
    {
        hashRow[r].init();
        hashCol[r].init();
        idxDiag[r] = 0;
        idxDiag[r + N] = 0;
        
        for (int c = 0; c <= N; c++)
        {
            holes[r][c] = -1;
            visited[r][c] = 0;
        }
    }

}



void addDiagonal(int mARow, int mACol, int mBRow, int mBCol)
{
    int x1, x2, y1, y2;
    
    if (mACol < mBCol)
    {
        x1 = mACol; y1 = -mARow;
        x2 = mBCol; y2 = -mBRow;
    }
    else
    {
        x2 = mACol; y2 = -mARow;
        x1 = mBCol; y1 = -mBRow;
    }
    
    int slope = (y2 - y1) / (x2 - x1);
    int intercept = y1 - slope * x1;
    
    int key = abs(intercept);
    
    diags[key][idxDiag[key]].st = { -y1, x1 };
    diags[key][idxDiag[key]].ed = { -y2, x2 };
    diags[key][idxDiag[key]].slope = slope;
    diags[key][idxDiag[key]].intercept = intercept;
    idxDiag[key]++;
}



void addHole(int mRow, int mCol, int mID)
{
    holes[mRow][mCol] = mID;
    
    hashRow[mRow].push(mRow, mCol);
    hashCol[mCol].push(mRow, mCol);
}



void eraseHole(int mRow, int mCol)
{
    holes[mRow][mCol] = -1;
}



int hitMarble(int mRow, int mCol, int mK)
{
    nVisited++;
    
    cid = -1;
    
    Coordinate curr = { mRow, mCol };
    
    visited[curr.row][curr.col] = nVisited;
    
    while (mK > 0)
    {
        rNext = MAXN; cNext = MAXN;
        dist = MAXN * 15;
        
        hashRow[curr.row].searchHole(curr.row, curr.col);
        hashCol[curr.col].searchHole(curr.row, curr.col);
        
        checkDiagonal(curr.row, curr.col);
        
        if (dist == MAXN * 15) { break; }
        
        curr = { rNext, cNext };
        
        cid = holes[curr.row][curr.col];
        visited[curr.row][curr.col] = nVisited;
        
        mK--;
    }

    return cid;

}
