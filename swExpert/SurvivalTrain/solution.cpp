#include <string>

#define MAX_PASSENGERS 100000
#define MAX_JOB 1000

class Node
{
    public:
        int data;
        Node* next;
    
    Node(int a)
    {
        data = a;
    }
};

Node* idByJob[1000];

void appendNode(int key, Node* node)
{
    if (idByJob[key] == NULL)
    {
        idByJob[key] = node;
    }
    else
    {
        node->next = idByJob[key];
        idByJob[key] = node;
    }
}

int passengers[MAX_PASSENGERS][3];

void init(int N, int M, int J, int mPoint[], int mJobID[])
{
    for (int i=0; i < N; i++)
    {
        int car = i / M;
        Node* node = new Node(i);
        appendNode(mJobID[i], node);

        passengers[i][0] = mPoint[i];
        passengers[i][1] = car;
        passengers[i][2] = mJobID[i];
    }
}


void swap(int* a, int* b)
    {
        int* t = a;
        *a = *b;
        *b = t;
    }

class Priority_queue
{
    public:
        int node[MAX_PASSENGERS];
        int n;

    Priority_queue() { n = 0; }

    int getParent(int idx) { return (idx - 1) / 2; }
    int getLeft(int idx) { return 2 * idx + 1; }
    int getRight(int idx) { return 2 * idx + 2; }
    
    

    void push(int data)
    {
        node[n] = data;
        int k = n;
        n++;

        while (k > 0)
        {
            int parent = getParent(k);
            if (node[parent] > node[k])
            {
                swap(node[parent], node[k]);
                k = parent;
            }
            else { break; }   
        }
    }
    
    int pop()
    {
        int result = node[0];
        swap(node[0], node[n-1]);
        int k = 0;

        while (k < n-1)
        {
            int leftChild = getLeft(k);
            int rightChild = getRight(k);
            
        }
    }
};

void destroy()
{
}

int update(int mID, int mPoint)
{
    passengers[mID][0] += mPoint;
    return passengers[mID][0];
}

int updateByJob(int mJobID, int mPoint)
{
    int ans = 0;

    Node* node = idByJob[mJobID];
    while (node)
    {
        int mID = node->data;
        passengers[mID][0] += mPoint;
        ans += passengers[mID][0];

        node = node->next;
    }
    return ans;
}

int move(int mNum)
{
    return -3;
}