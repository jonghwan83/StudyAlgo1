#include <iostream>


static const int MAX_KITCHEN = 11;
static const int MAX_ORDER = 15001;
static const int MAX_TABLE = 1000;

struct Node
{
    int id;
    int idx;
    Node* next;
};

class LinkedList
{
public:
    int length;
    Node* head;
    
    void init()
    {
        length = 0;
        head = nullptr;
    }
    
    void push(int id, int idx)
    {
        Node* node = new Node();
        node->id = id; node->idx = idx;
        
        node->next = head;
        head = node;
        
        length++;
    }
};

struct Pair
{
    int id, time;
};

class Heap
{
public:
    int length;
    Pair arr[MAX_ORDER];
    
    void init() { length = 0; }
    
    bool compare(int parent, int child)
    {
        if (arr[parent].time > arr[child].time) { return true; }
        return false;
    }
    
    void push(int time, int id)
    {
        Pair node = { id, time };
        
        int idx = length;
        arr[length++] = node;
        
        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx))
        {
            Pair temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }
    
    Pair pop()
    {
        Pair ans = arr[0];
        arr[0] = arr[--length];
        
        int left, right, child;
        int idx = 0;
        
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
                Pair temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }
        
        return ans;
    }
};


class OrderSheet
{
public:
    int id, length;
    int dishes[11];     // 0: not ordered, 1: ordered, 2: ready to served
    bool isFinished;    // true: served
    bool isCanceled;

    void init()
    {
        for (int i = 0; i < 11; i++)
        {
            dishes[i] = 0;
        }
    }
    
    void add(int mId, int M, int mDishes[])
    {
        isCanceled = false;
        isFinished = false;
        id = mId;
        length = M;
        
        for (int i = 0; i < M; i++)
        {
            dishes[mDishes[i]] = 1;
        }
    }
    
    void checkStatus()
    {
        if (length == 0)
        {
            isFinished = true;
        }
    }
    
};


class Kitchen
{
public:
    // order index in queue
    int orderQueue[MAX_ORDER * 10];
    int st, ed, length;
    
    // when remain time = 0, pop queue
    int expectedTime;
    int cookingTime;
    
    void init(int mCookingTime)
    {
        st = ed = length = 0;
        
        cookingTime = mCookingTime;
        
        expectedTime = -1;
    }
    
    void push(int oIdx)
    {
        length++;
        orderQueue[ed++] = oIdx;
    }
    
    int pop()
    {
        length--;
        return orderQueue[st++];
    }
    
    int top()
    {
        return orderQueue[st];
    }
};


int nkitchen;

int orderIdx;
OrderSheet orderlist[MAX_ORDER];

int nfinished;

LinkedList hashID[MAX_TABLE];

Kitchen kitchens[MAX_KITCHEN];

Heap timeSeq;

void readyToServe(int k, bool isCanceled)
{
    if (!isCanceled)
    {
        kitchens[k].expectedTime = -1;
    }
    
    int oIdx = -1;

    while (kitchens[k].length > 0)
    {
        int temp = kitchens[k].pop();

        if (orderlist[temp].isFinished) { continue; }

        oIdx = temp;

        break;
    }

    if (oIdx == -1) { return; }

    orderlist[oIdx].dishes[k]++;
    orderlist[oIdx].length--;

    if (orderlist[oIdx].length == 0)
    {
        orderlist[oIdx].isFinished = true;
        nfinished++;
    }
}


bool startCooking(int mTime, int k)
{
    // check queue
    while (kitchens[k].length > 0 && orderlist[kitchens[k].top()].isFinished)
    {
        kitchens[k].pop();
    }

    if (kitchens[k].length == 0)
    {
        kitchens[k].expectedTime = -1;
        return false;
    }
    else if (kitchens[k].expectedTime > -1)
    {
        return false;
    }

    kitchens[k].expectedTime = mTime + kitchens[k].cookingTime;
    timeSeq.push(kitchens[k].expectedTime, k);

    return true;
}

void updateTime(int mTime)
{
    while (timeSeq.arr[0].time <= mTime && timeSeq.length > 0)
    {
        Pair curr = timeSeq.pop();
        
        if (curr.time != kitchens[curr.id].expectedTime) { continue; }
        
        // complete cooking
        readyToServe(curr.id, false);

        // start cooking on curr.id
        startCooking(curr.time, curr.id);
    }
}

int findByID(int mID)
{
    int key = mID % MAX_TABLE;
    
    Node* node = hashID[key].head;
    
    while (node)
    {
        if (node->id == mID)
        {
            return node->idx;
        }
        node = node->next;
    }
    
    return -1;
}


void init(int N, int mCookingTimeList[])
{
    timeSeq.init();
    
    nkitchen = N;

    orderIdx = nfinished = 0;
    
    for (int i = 0; i < MAX_TABLE; i++)
    {
        hashID[i].init();
    }

    for (int i = 0; i < N; i++)
    {
        kitchens[i + 1].init(mCookingTimeList[i]);
    }
    
    return;
}



int order(int mTime, int mID, int M, int mDishes[])
{
    updateTime(mTime);

    orderlist[orderIdx].init();
    
    for (int i = 0; i < M; i++)
    {
        int dish = mDishes[i];

        kitchens[dish].push(orderIdx);

        orderlist[orderIdx].add(mID, M, mDishes);

        startCooking(mTime, dish);
    }

    int key = mID % MAX_TABLE;
    hashID[key].push(mID, orderIdx);
    orderIdx++;

    return orderIdx - nfinished;
}



int cancel(int mTime, int mID)
{
    updateTime(mTime);
    
    int oIdx = findByID(mID);

    orderlist[oIdx].isCanceled = true;
    orderlist[oIdx].isFinished = true;
    nfinished++;

    for (int i = 1; i <= nkitchen; i++)
    {
        if (orderlist[oIdx].dishes[i] == 2)
        {
            readyToServe(i, true);
            startCooking(mTime, i);
        }

        if (orderlist[oIdx].dishes[i] == 1)
        {
            startCooking(mTime, i);
        }
    }

    return orderIdx - nfinished;
}



int getStatus(int mTime, int mID)
{
    updateTime(mTime);
    
    int oIdx = findByID(mID);
    
    if (orderlist[oIdx].isCanceled) { return -1; }

    return orderlist[oIdx].length;
}
