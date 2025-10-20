#define MAX_N 101
#define MAX_M 50
#define MAX_K 1001
#define INF 1000000000

struct Code
{
    long long attr;
    int idx;

    Code()
    {
        attr = 0; idx = -1;
    }
    
    Code(long long a, int i)
    {
        attr = a; idx = i;
    }
};

int countSetBits(long long n)
{
    int count = 0;
    while (n > 0) {
        count += (n & 1);
        n >>= 1;
    }
    return count;
}


struct Pipe
{
    Code code;
    bool isRemoved;
    
    Pipe() {}
    
    Pipe(Code c)
    {
        code = c;
        isRemoved = false;
    }
};

struct Node
{
    int id;
    int idx;
    Node* next;
    
    Node(int mId, int index)
    {
        id = mId; idx = index;
        next = nullptr;
    }
};

struct LinkedList
{
    int length;
    Node* head;
    
    void init()
    {
        length = 0;
        head = nullptr;
    }
    
    void insert(int mId, int index)
    {
        Node* node = new Node(mId, index);
        node->next = head;
        
        head = node;
        length++;
    }
    
    int findById(int mId)
    {
        Node* node = head;
        
        while(node)
        {
            if(node->id == mId)
            {
                return node->idx;
            }
            node = node->next;
        }
        
        return -1;
    }
};

struct Storage
{
    int length;
    int next[MAX_N];
    int idx[MAX_N];
    
    void init()
    {
        length = 0;
    }
    
    void push(int bStorage, int index)
    {
        next[length] = bStorage;
        idx[length] = index;
        length++;
    }
};

struct HeapNode
{
    int cost;
    int storage;
    bool isTransformed;
    int pIdx;
    
    HeapNode() {}
    HeapNode(int c, int s, bool trans, int pipe)
    {
        cost = c; storage = s;
        isTransformed = trans;
        pIdx = pipe;
    }
};

struct priorityQueue
{
    int length;
    HeapNode arr[MAX_N * MAX_K];
    
    void init()
    {
        length = 0;
    }
    
    bool compare(int parent, int child)
    {
        if(arr[parent].cost > arr[child].cost) return true;
        return false;
    }
    
    void swap(int &a, int &b)
    {
        HeapNode temp = arr[a];
        arr[a] = arr[b];
        arr[b] = temp;
    }
    
    void push(HeapNode node)
    {
        int idx = length;
        arr[length++] = node;
        
        while((idx-1)/2 >= 0 && compare((idx-1)/2, idx))
        {
            int parent = (idx-1)/2;
            swap(parent, idx);
            idx = (idx-1)/2;
        }
    }
    
    HeapNode pop()
    {
        HeapNode res = arr[0];
        arr[0] = arr[--length];
        
        int idx = 0;
        int left, right, child;
        
        while(2*idx+1 < length)
        {
            left = 2*idx + 1;
            right = 2*idx + 2;
            
            if(right < length)
                if(compare(left, right))
                    child = right;
                else child = left;
            else child = left;
            
            if(compare(idx, child))
            {
                swap(idx, child);
                idx = child;
            }
            else { break; }
        }
        
        return res;
    }
};


int m, n, k;
LinkedList hashID;
Pipe pipes[MAX_K];
Storage storages[MAX_N];
int minCost[MAX_N][MAX_K];
priorityQueue pq;

Code attrToCode(char mAttr[])
{
    long long attr = 0;
    int idx = -1;
    for(int i = 0; i < m; i++)
    {
        if(mAttr[i] == 'W')
        {
            long long temp = 1;
            temp <<= i;
            attr |= temp;
        }
        
        if(mAttr[i] == 'D')
        {
            idx = i;
        }
    }
    
    return Code(attr, idx);
}

int cmpCode(Code a, Code b)
{
    long long res = a.attr ^ b.attr;
    
    int cnt = countSetBits(res);
    
    if(a.idx == b.idx)
    {
        cnt++;
    }
    
    else
    {
        long long aOnA = a.attr >> a.idx;
        long long bOnA = b.attr >> a.idx;
        
        cnt += (aOnA & 1) == (bOnA & 1);
        
        long long aOnB = a.attr >> b.idx;
        long long bOnB = b.attr >> b.idx;
        
        cnt += (aOnB & 1) == (bOnB & 1);
    }
    
    return cnt;
}

void getMinCost(int st, int ed)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < k; j++)
        {
            minCost[i][j] = INF;
        }
    }
    
    pq.init();
    pq.push(HeapNode(0, st, false, 0));
    minCost[st][0] = 0;
    
    while(pq.length > 0)
    {
        HeapNode curr = pq.pop();
        
        if(curr.storage == ed) return;
        
        for(int i = 0; i < storages[curr.storage].length; i++)
        {
            int to = storages[curr.storage].next[i];
            int p = storages[curr.storage].idx[i];
            
            if(pipes[p].isRemoved) continue;
            
            int cost = curr.cost + cmpCode(pipes[p].code, pipes[curr.pIdx].code);
                
            if(cost < minCost[to][curr.pIdx])
            {
                minCost[to][curr.pIdx] = cost;
                pq.push(HeapNode(cost, to, curr.isTransformed, curr.pIdx));
            }
            
            if(curr.isTransformed) continue;
            
            int cost2 = curr.cost + cmpCode(pipes[p].code, pipes[p].code);
            if(cost2 < minCost[to][p])
            {
                minCost[to][p] = cost2;
                pq.push(HeapNode(cost2, to, true, p));
            }
        }
    }
}


void add(int mID, int aStorage, int bStorage, char mAttr[])
{
    Code c = attrToCode(mAttr);
    hashID.insert(mID, k);
    
    storages[aStorage].push(bStorage, k);
    storages[bStorage].push(aStorage, k);
    
    pipes[k++] = Pipe(c);
}



void init(int N, int M, int K, int mID[], int aStorage[], int bStorage[], char mAttr[][MAX_M])
{
    m = M;
    n = N+1;
    k = 1;
    
    hashID.init();
    
    for(int i = 0; i <= N; i++)
    {
        storages[i].init();
    }
    
    for(int i = 0; i < K; i++)
    {
        add(mID[i], aStorage[i], bStorage[i], mAttr[i]);
    }
}


void remove(int mID)
{
    int idx = hashID.findById(mID);
    
    pipes[idx].isRemoved = true;
}


int transport(int sStorage, int eStorage, char mAttr[])
{
    Code c = attrToCode(mAttr);
    pipes[0] = Pipe(c);
    
    getMinCost(sStorage, eStorage);
    
    int ans = INF;
    for(int i = 0; i < k; i++)
    {
        if(minCost[eStorage][i] < ans)
        {
            ans = minCost[eStorage][i];
        }
    }
    
    if(ans == INF) return -1;
    
    return ans;
}
