#define TABLE 100

#define MAXHOUSE 25000

#define MAPSIZE 201

int abs(int a) { return a < 0? -a : a; }

int min(int a, int b) { return a < b? a: b; }

int getDistance(int x1, int y1, int x2, int y2)
{
    return abs(x2 - x1) + abs(y2 - y1);
}

struct Node
{
    Node* next;
    int id;
    int index;
};

class LinkedList
{
public:
    int length;
    Node* head;
    Node* tail;
    
    void init()
    {
        length = 0;
        head = nullptr;
        tail = nullptr;
    }
    
    void push(int id, int index)
    {
        Node* node = new Node();
        
        node->id = id; node->index = index;
        node->next = head;
        
        if (!tail) { tail = node; }
        
        head = node;
        
        length++;
    }
};


struct House {
    int id, x, y;
    bool isRemoved;
    int town;
};

int drows[9] = { 0, -1, 0, 1, 0, -1, -1, 1, 1 };
int dcols[9] = { 0, 0, -1, 0, 1, -1, 1, -1, 1 };

int range;

int townIdx;

LinkedList towns[MAXHOUSE];

int nHouse;

House houses[MAXHOUSE];

LinkedList hashID[TABLE];

int emptyInTown[MAXHOUSE];

LinkedList map[MAPSIZE][MAPSIZE];


int mergeTown(int a, int b)
{
    if (a == b) { return -1; }
    if (towns[b].length == 0 || towns[a].length == 0) { return -1; }
    
    if (towns[b].length > towns[a].length)
    {
        int temp = a;
        a = b;
        b = temp;
    }
    
    int pop = towns[b].length - emptyInTown[b];
    
    towns[a].tail->next = towns[b].head;
    towns[a].tail = towns[b].tail;
    
    towns[a].length += towns[b].length;
    
    Node* node = towns[b].head;
    
    while (node)
    {
        houses[node->index].town = a;
        node = node->next;
    }
    
    towns[b].init();
    
    emptyInTown[a] += emptyInTown[b];
    
    emptyInTown[b] = 0;
    
    if (pop <= 0) { return false; }
    
    return a;
}

int findByID(int mId)
{
    int key = mId % TABLE;
    
    Node* node = hashID[key].head;
    
    while (node)
    {
        if (node->id == mId)
        {
            return node->index;
        }
        node = node->next;
    }
    
    return -1;
}

void init(int L, int R) {
    
    range = L;
    
    nHouse = 0;
    
    townIdx = 0;
    
    for (int i = 0; i < TABLE; i++)
    {
        hashID[i].init();
    }
    
    for (int town = 0; town < MAXHOUSE; town++)
    {
        towns[town].init();
        emptyInTown[town] = 0;
    }
    
    for (int row = 0; row < MAPSIZE; row++)
        for (int col = 0; col < MAPSIZE; col++)
        {
            map[row][col].init();
        }
    
    
    return;
}



int add(int mId, int mX, int mY) {
    
    int key = mId % TABLE;
    
    hashID[key].push(mId, nHouse);
    
    houses[nHouse] = { mId, mX, mY };
    
    houses[nHouse].isRemoved = false;
    
    
    int nClose = 0;
    int closeTown[100];
    
    int row = mX / range;
    int col = mY / range;
    
    
    for (int i = 0; i < 9; i++)
    {
        int dr = row + drows[i];
        int dc = col + dcols[i];
        
        if (dr < 0 || dc < 0) { continue; }
        if (dr >= MAPSIZE || dc >= MAPSIZE) { continue; }
        
        
        Node* node = map[dr][dc].head;
        
        while (node)
        {
            int neighbor = node->index;
            int distance = getDistance(mX, mY, houses[neighbor].x, houses[neighbor].y);
            
            if (distance <= range && !houses[neighbor].isRemoved )
            {
                closeTown[nClose++] = houses[neighbor].town;
            }
            
            node = node->next;
        }
    }
    
    
    
    if (nClose == 0)
    {
        houses[nHouse].town = townIdx;
        towns[townIdx].push(mId, nHouse);
        townIdx++;
    }
    else
    {
        int mainTown = closeTown[0];
        
        for (int i = 1; i < nClose; i++)
        {
            int res = mergeTown(mainTown, closeTown[i]);
            if (res > -1)
            {
                mainTown = res;
            }
        }
        
        towns[mainTown].push(mId, nHouse);
        houses[nHouse].town = mainTown;
    }
    
    
    map[row][col].push(mId, nHouse);
    
    int resTown = houses[nHouse].town;
    
    nHouse++;
    
    return towns[resTown].length - emptyInTown[resTown];
}



int remove(int mId) {
    
    int idx = findByID(mId);
    
    if (idx == -1) { return -1; }
    if (houses[idx].isRemoved) { return -1;}
    
    houses[idx].isRemoved = true;
    
    int currTown = houses[idx].town;
    
    emptyInTown[currTown]++;
    
    int res = towns[currTown].length - emptyInTown[currTown];
    
    return res;
}



int check(int mId) {
    
    int idx = findByID(mId);
    
    if (idx == -1) { return -1; }
    
    if (houses[idx].isRemoved) { return -1; }
    
    int town = houses[idx].town;
    
    
    Node* node = towns[town].head;
    
    int minID = 1'000'000'001;
    
    while (node)
    {
        if (!houses[node->index].isRemoved)
        {
            minID = min(minID, node->id);
        }
        node = node->next;
    }
    
    
    return minID;
}



int count() {
    
    int sum = 0;
    
    for (int i = 0; i < townIdx; i++)
    {
        if (towns[i].length - emptyInTown[i] > 0)
        {
            sum++;
        }
    }
    
    return sum;
}
