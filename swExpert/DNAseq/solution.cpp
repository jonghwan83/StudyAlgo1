#include <cstring>

#define MAXGENE 20000
#define MAXL 61
#define TABLESIZE 4096

class Gene {
public:
    int id;
    char name[MAXL];
    bool isRemoved;
};


int hash(char mSeq[], int n) {
    unsigned long long hash = 5381;

    for (int i = 0; i < n; i++) {
        hash = (((hash << 5) + hash) + mSeq[i]) % TABLESIZE;
    }

    return hash % TABLESIZE;
}


class NodeChar {
public:
    char data[MAXL];
    int idx;
    NodeChar* next;
};

class NodeID {
public:
    int id;
    int idx;
    NodeID* next;
};

class HashChain {
public:
    NodeChar* heads[TABLESIZE];

    void init() {
        for (int i = 0; i < TABLESIZE; i++) {
            heads[i] = nullptr;
        }
    }

    void push(int n, char mSeq[], int idx) {
        int key = hash(mSeq, n);

        NodeChar* node = new NodeChar();
        node->idx = idx;
        strcpy(node->data, mSeq);

        node->next = heads[key];
        heads[key] = node;
    }

    int find(int n, char mSeq[]) {
        int key = hash(mSeq, n);

        NodeChar* node = heads[key];

        while (node) {
            if (strcmp(mSeq, node->data) == 0) {
                return node->idx;
            }
            node = node->next;
        }

        return -1;
    }
};

class HashChainID {
public:
    NodeID* heads[TABLESIZE];

    void init() {
        for (int i = 0; i < TABLESIZE; i++) {
            heads[i] = nullptr;
        }
    }

    void push(int id, int idx) {
        int key = id % TABLESIZE;

        NodeID* node = new NodeID();
        node->id = id;
        node->idx = idx;

        node->next = heads[key];
        heads[key] = node;
    }

    int find(int id) {
        int key = id % TABLESIZE;

        NodeID* node = heads[key];

        while (node) {
            if (node->id == id) {
                return node->idx;
            }
            node = node->next;
        }

        return -1;
    }
};


Gene genes[MAXGENE];
HashChain hashSeq;
HashChain hashSeq3;
HashChainID hashID;

int gIdx;

void init()
{
    gIdx = 0;

    hashSeq.init();
    hashSeq3.init();
    hashID.init();
}


int addSeq(int mID, int mLen, char mSeq[])
{
    int idx = hashID.find(mID);
    if (idx > -1 && !genes[idx].isRemoved) {
        return 0;
    }

    idx = hashSeq.find(mLen, mSeq);
    if (idx > -1 && !genes[idx].isRemoved) {
        return 0;
    }


    genes[gIdx].id = mID;
    genes[gIdx].isRemoved = false;
    strcpy(genes[gIdx].name, mSeq);

    hashSeq.push(mLen, mSeq, gIdx);
    hashSeq3.push(3, mSeq, gIdx);
    hashID.push(mID, gIdx);

    gIdx++;
    return 1;
}



int searchSeq(int mLen, char mBegin[])
{
    char base[3];
    for (int i = 0; i < 3; i++) {
        base[i] = mBegin[i];
    }

    int ans = 0;
    int id = 0;

    int key = hash(base, 3);
    NodeChar* node = hashSeq3.heads[key];

    while (node) {
        if (genes[node->idx].isRemoved) {
            node = node->next;
            continue;
        }

        if (strlen(genes[node->idx].name) < mLen) {
            node = node->next;
            continue;
        }

        char temp[MAXL];
        for (int i = 0; i < mLen; i++) {
            temp[i] = genes[node->idx].name[i];
        }
        temp[mLen] = '\0';

        if (strcmp(temp, mBegin) == 0) {
            id = genes[node->idx].id;
            ans++;
        }
        node = node->next;
    }

    if (ans == 0) { return -1; }
    if (ans == 1) { return id; }
    return ans;

}



int eraseSeq(int mID)
{
    int idx = hashID.find(mID);

    if (idx == -1) { return 0; }
    if (genes[idx].isRemoved) { return 0; }

    genes[idx].isRemoved = true;

    return 1;
}



int changeBase(int mID, int mPos, char mBase)
{
    int idx = hashID.find(mID);

    if (idx == -1) { return 0; }
    if (genes[idx].isRemoved) { return 0; }
    if (strlen(genes[idx].name) <= mPos) { return 0; }

    if (genes[idx].name[mPos] == mBase) { return 0; }

    char temp[MAXL];
    strcpy(temp, genes[idx].name);
    temp[mPos] = mBase;

    int tIdx = hashSeq.find((int) strlen(temp), temp);
    if (tIdx > 0 && !genes[tIdx].isRemoved) { return 0; }

    genes[idx].isRemoved = true;
    addSeq(genes[idx].id, (int) strlen(temp), temp);

    return 1;
}