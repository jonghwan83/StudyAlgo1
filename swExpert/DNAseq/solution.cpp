#include <cstring>

#define MAXDNA 25000
#define MAXL 61
#define TABLESIZE 4096

class DNA {
public:
    int id;
    bool isRemoved;
    char sequence[MAXL];
};

int char2key(char mSeq[], int n) {
    unsigned long hash = 5381;

    for (int i = 0; i < n; i++) {
        hash = (((hash << 5) + hash) + mSeq[i]) % TABLESIZE;
    }

    return hash;
}

class Node {
public:
    int idx;
    Node* next;
};

class LinkedList {
public:
    int length;
    Node* head;

    void init() {
        length = 0;
        head = nullptr;
    }

    void push(int a) {
        Node* node = new Node();
        node->idx = a;

        node->next = head;
        head = node;
        length++;
    }
};

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

bool compare(char a[], char b[]) {
    int n = min(strlen(a), strlen(b));

    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) { return false;}
    }

    return true;
}


int idx;
DNA genes[MAXDNA];
LinkedList hashSeq[TABLESIZE];
LinkedList hashID[TABLESIZE];
LinkedList hashSeq3[TABLESIZE];

int findByID(int mID) {
    int key = mID % TABLESIZE;

    Node* node = hashID[key].head;

    while (node) {
        if (genes[node->idx].isRemoved) {
            node = node->next;
            continue;
        }

        if (genes[node->idx].id == mID) {
            return node->idx;
        }

        node = node->next;
    }

    return -1;
}

int findBySeq(char mSeq[]) {
    int key = char2key(mSeq, strlen(mSeq));

    Node* node = hashSeq[key].head;

    while (node) {
        if (genes[node->idx].isRemoved) {
            node = node->next;
            continue;
        }

        if (strcmp(mSeq, genes[node->idx].sequence) == 0) {
            return node->idx;
        }

        node = node->next;
    }

    return -1;
}

int findBySeq3(char mSeq[]) {
    int key = char2key(mSeq, 3);

    Node* node = hashSeq3[key].head;

    while (node) {
        if (genes[node->idx].isRemoved) {
            node = node->next;
            continue;
        }

        if (compare(mSeq, genes[node->idx].sequence)) {
            return node->idx;
        }

        node = node->next;
    }

    return -1;
}


void init()
{
    idx = 0;

    for (int i = 0; i < TABLESIZE; i++) {
        hashSeq[i].init();
        hashID[i].init();
        hashSeq3[i].init();
    }

}


int addSeq(int mID, int mLen, char mSeq[])
{
    int idxByID = findByID(mID);
    int idxBySeq = findBySeq(mSeq);

    if (idxByID != -1 || idxBySeq != -1) { return 0; }


    genes[idx].id = mID;
    genes[idx].isRemoved = false;
    strcpy(genes[idx].sequence, mSeq);

    int keySeq = char2key(mSeq, mLen);
    int keySeq3 = char2key(mSeq, 3);
    int keyID = mID % TABLESIZE;

    hashSeq[keySeq].push(idx);
    hashSeq3[keySeq3].push(idx);
    hashID[keyID].push(idx);

    idx++;
    return 1;

}


int searchSeq(int mLen, char mBegin[])
{
    int key = char2key(mBegin, 3);

    int cnt = 0;
    int id = -1;

    Node* node = hashSeq3[key].head;

    while (node) {
        if (genes[node->idx].isRemoved) {
            node = node->next;
            continue;
        }

        if (strlen(genes[node->idx].sequence) < mLen) {
            node = node->next;
            continue;
        }

        if (compare(mBegin, genes[node->idx].sequence)) {
            id = genes[node->idx].id;
            cnt++;
        }

        node = node->next;
    }

    if (cnt > 1) { return cnt; }
    if (cnt == 1) { return id; }

    return -1;
}


int eraseSeq(int mID)
{
    int idxByID = findByID(mID);

    if (idxByID == -1) { return 0; }
    if (genes[idxByID].isRemoved) { return 0; }

    genes[idxByID].isRemoved = true;

    return 1;
}

int changeBase(int mID, int mPos, char mBase)
{
    int idxByID = findByID(mID);

    if (idxByID == -1) { return 0; }
    if (strlen(genes[idxByID].sequence) <= mPos) { return 0; }
    if (genes[idxByID].sequence[mPos] == mBase) { return 0; }

    char temp[MAXL];
    strcpy(temp, genes[idxByID].sequence);
    temp[mPos] = mBase;

    int tIdx = findBySeq(temp);
    if (tIdx != -1) { return 0; }

    genes[idxByID].isRemoved = true;

    addSeq(genes[idxByID].id, strlen(temp), temp);

    return 1;
}

