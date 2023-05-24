#include <cstring>

using namespace std;

#define MAXMICROBE 12001
#define SUBSET 1000
#define TABLESIZE 4096


class Microbe {
public:
    char name[12];
    int parent;
    int dist;
};

class Grid {
public:
    int cnt[SUBSET];
    int subtotal;

    void init() {
        subtotal = 0;
        for (int i = 0; i < SUBSET; i++) {
            cnt[i] = 0;
        }
    }

    void push(int st, int ed) {
        for (int i = st; i <= ed; i++) {
            cnt[i]++;
        }
    }
};

int char2key(const char *str){
    unsigned long hash = 5381;
    int c;
 
    while (c = *str++)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLESIZE;
}

class Node {
public:
    char name[12];
    int data;
    Node* next;
};

Node* hashTable[TABLESIZE];

void initHash() {
    for (int i = 0; i < TABLESIZE; i++) {
        hashTable[i] = nullptr;
    }
}

void addHash(char mName[], int idx) {
    int key = char2key(mName);

    Node* node = new Node();
    node->data = idx;
    strcpy(node->name, mName);

    node->next = hashTable[key];
    hashTable[key] = node;
}

int findHash(char mName[]) {
    int key = char2key(mName);

    Node* node = hashTable[key];

    while (node != nullptr) {
        if (strcmp(node->name, mName) == 0) {
            return node->data;
        }
        node = node->next;
    }

    return -1;
}


int mIdx;
Microbe microbes[MAXMICROBE];
Grid grid[SUBSET + 1];

int commonAncestor;


void getLowestCommon(int idx1, int idx2) {
    if (idx1 == 0 || idx2 == 0) {
        commonAncestor = 0;
        return;
    }
    if (idx1 == idx2) {
        commonAncestor = idx1;
        return;
    }

    if (microbes[idx1].dist > microbes[idx2].dist) {
        getLowestCommon(microbes[idx1].parent, idx2);
    }
    else {
        getLowestCommon(idx1, microbes[idx2].parent);
    }
}

void addGrid(int mFirstDay, int mLastDay) {
    int sIdx = mFirstDay / SUBSET;
    int eIdx = mLastDay / SUBSET;

    int st = mFirstDay % SUBSET;
    int ed = mLastDay % SUBSET;

    for (int i = sIdx + 1; i < eIdx; i++) {
        grid[i].subtotal++;
    }

    if (sIdx == eIdx) {
        grid[sIdx].push(st, ed);
    }
    else {
        grid[sIdx].push(st, SUBSET-1);
        grid[eIdx].push(0, ed);
    }
}


void init(char mAncestor[], int mLastday)
{
    initHash();

    mIdx = 0;

    strcpy(microbes[mIdx].name, mAncestor);
    microbes[mIdx].dist = 0;
    microbes[mIdx].parent = -1;

    addHash(mAncestor, 0);

    mIdx++;

    for (int i = 0; i < SUBSET + 1; i++) {
        grid[i].init();
    }

    addGrid(0, mLastday);


    return;
}



int add(char mName[], char mParent[], int mFirstday, int mLastday)
{
    addHash(mName, mIdx);
    
    int parent = findHash(mParent);

    strcpy(microbes[mIdx].name, mName);
    microbes[mIdx].parent = parent;
    microbes[mIdx].dist = microbes[parent].dist + 1;

    mIdx++;

    addGrid(mFirstday, mLastday);

    return microbes[mIdx - 1].dist;
}



int distance(char mName1[], char mName2[])
{
    int idx1 = findHash(mName1);
    int idx2 = findHash(mName2);

    getLowestCommon(idx1, idx2);


    return microbes[idx1].dist + microbes[idx2].dist - 2 * microbes[commonAncestor].dist;
}



int count(int mDay)
{
    int idx = mDay / SUBSET;
    int subIdx = mDay % SUBSET;

    return grid[idx].subtotal + grid[idx].cnt[subIdx];
}
