#include <string>
#include <vector>
#include <queue>

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
int nCar;
int total;


void init(int N, int M, int J, int mPoint[], int mJobID[])
{
    total = N;
    nCar = N / M;
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
    std::priority_queue<std::pair<int, int> > maxHeap[nCar];
    std::priority_queue<std::pair<int, int> > minHeap[nCar];
    for (int i=0; i < total; i++)
    {
        maxHeap[passengers[i][1]].push(std::make_pair(passengers[i][0], i));
        minHeap[passengers[i][1]].push(std::make_pair(passengers[i][0], i));
    }

    return -3;
}