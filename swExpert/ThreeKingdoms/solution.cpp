#include <string>
#include <unordered_map>

#define MAXSIZE 50

using namespace std;

class Monarch {
public:
    string name;
    int gIdx;
    int nSoldiers;
    int r;
    int c;
    bool isRemoved;

    Monarch() {
        name = "";
        gIdx = 0;
        nSoldiers = 0;
        isRemoved = false;
    }
};

class Node {
public:
    int mIdx;
    Node* next;

    Node(int a) {
        mIdx = a;
        next = nullptr;
    }
};

class LinkedList {
public:
    Node* head;
    Node* tail;
    int length;
    int nSoldiers;
    bool enemyIdx[MAXSIZE * MAXSIZE];

    LinkedList() {
        head = nullptr;
        tail = nullptr;
        length = 0;
        nSoldiers = 0;
        for (int i = 0; i < MAXSIZE * MAXSIZE; i++) {
            enemyIdx[i] = false;
        }
    }

    void clear() {
        head = nullptr;
        tail = nullptr;
        length = 0;
        nSoldiers = 0;
    }

    void addNode(int a) {
        Node* newNode = new Node(a);

        newNode->next = head;
        if (!tail) { tail = newNode; }

        head = newNode;
        length++;
    }
};

int monarchIdx;
int nGrid;
Monarch monarch[MAXSIZE * MAXSIZE];
LinkedList alliance[MAXSIZE * MAXSIZE];
int grid[MAXSIZE][MAXSIZE];
unordered_map<string, int> hashMonarch;
int drows[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dcols[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

int attacker, defender;

void addMonarch(char mMonarch[11], int soldiers, int gIdx, int newR, int newC) {
    monarch[monarchIdx].name = mMonarch;
    monarch[monarchIdx].nSoldiers = soldiers;
    monarch[monarchIdx].gIdx = gIdx;
    monarch[monarchIdx].r = newR;
    monarch[monarchIdx].c = newC;
    monarch[monarchIdx].isRemoved = false;

    hashMonarch[mMonarch] = monarchIdx;
    grid[newR][newC] = monarchIdx;

    monarchIdx++;
}

void assebleLink(int gIdxA, int gIdxB) {
    Node* cNode = alliance[gIdxB].head;
    while (cNode) {
        monarch[cNode->mIdx].gIdx = gIdxA;
        monarch[cNode->mIdx].nSoldiers += (alliance[gIdxB].nSoldiers - alliance[gIdxA].nSoldiers);
        cNode = cNode->next;
    }

    for (int i = 0; i < nGrid * nGrid; i++) {
        if (alliance[gIdxB].enemyIdx[i]) {
            alliance[gIdxA].enemyIdx[i] = true;
            alliance[i].enemyIdx[gIdxA] = true;
        }
    }

    alliance[gIdxA].tail->next = alliance[gIdxB].head;
    alliance[gIdxA].tail = alliance[gIdxB].tail;
    alliance[gIdxA].length += alliance[gIdxB].length;

    alliance[gIdxB].clear();
}

void init(int N, int mSoldier[25][25], char mMonarch[25][25][11])
{
    hashMonarch.clear();
    monarchIdx = 0;
    nGrid = N;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            alliance[monarchIdx].clear();
            alliance[monarchIdx].addNode(monarchIdx);
            for (int k = 0; k < nGrid * nGrid; k++) {
                alliance[monarchIdx].enemyIdx[k] = false;
            }
            addMonarch(mMonarch[i][j], mSoldier[i][j], monarchIdx, i, j);
        }
    }
}

void destroy()
{

}

int ally(char mMonarchA[11], char mMonarchB[11])
{
    int mIdxA = hashMonarch[mMonarchA];
    int mIdxB = hashMonarch[mMonarchB];

    int gIdxA = monarch[mIdxA].gIdx;
    int gIdxB = monarch[mIdxB].gIdx;

    if (gIdxA == gIdxB) {
        return -1;
    }

    if (alliance[gIdxB].enemyIdx[gIdxA]) {
        return -2;
    }

    if (alliance[gIdxA].length > alliance[gIdxB].length) {
        assebleLink(gIdxA, gIdxB);
    }
    else {
        assebleLink(gIdxB, gIdxA);
    }
    return 1;
}

bool checkEnemy(int mIdxA, int mIdxB) {
    int gIdxA = monarch[mIdxA].gIdx;
    int gIdxB = monarch[mIdxB].gIdx;

    bool flag = false;

    int dr, dc;
    for (int i = 0; i < 8; i++) {
        dr = monarch[mIdxB].r + drows[i];
        dc = monarch[mIdxB].c + dcols[i];

        if (0 <= dr && dr < nGrid && 0 <= dc && dc < nGrid) {
            if (monarch[grid[dr][dc]].gIdx == gIdxA) {
                flag = true;
                return flag;
            }
        }
    }
    return flag;
}

void deploySoldiers(int mIdxA, int mIdxB) {
    attacker = 0;
    defender = 0;
    int gIdxA = monarch[mIdxA].gIdx;
    int gIdxB = monarch[mIdxB].gIdx;

    alliance[gIdxA].enemyIdx[gIdxB] = true;
    alliance[gIdxB].enemyIdx[gIdxA] = true;

    defender += monarch[mIdxB].nSoldiers + alliance[gIdxB].nSoldiers;
    monarch[mIdxB].nSoldiers -= (monarch[mIdxB].nSoldiers + alliance[gIdxB].nSoldiers);

    int dr, dc;
    for (int i = 0; i < 8; i++) {
        dr = monarch[mIdxB].r + drows[i];
        dc = monarch[mIdxB].c + dcols[i];

        if (0 <= dr && dr < nGrid && 0 <= dc && dc < nGrid) {
            if (!monarch[grid[dr][dc]].isRemoved) {
                if (monarch[grid[dr][dc]].gIdx == gIdxA) {
                    attacker += (monarch[grid[dr][dc]].nSoldiers + alliance[gIdxA].nSoldiers) / 2;
                    monarch[grid[dr][dc]].nSoldiers -= (monarch[grid[dr][dc]].nSoldiers + alliance[gIdxA].nSoldiers) / 2;
                }
                if (monarch[grid[dr][dc]].gIdx == gIdxB) {
                    defender += (monarch[grid[dr][dc]].nSoldiers + alliance[gIdxB].nSoldiers) / 2;
                    monarch[grid[dr][dc]].nSoldiers -= (monarch[grid[dr][dc]].nSoldiers + alliance[gIdxB].nSoldiers) / 2;
                }
            }
        }
    }
}

int attack(char mMonarchA[11], char mMonarchB[11], char mGeneral[11])
{
    int mIdxA = hashMonarch[mMonarchA];
    int mIdxB = hashMonarch[mMonarchB];

    if (monarch[mIdxA].gIdx == monarch[mIdxB].gIdx) { return -1; }
    bool check = checkEnemy(mIdxA, mIdxB);

    if (!check) { return -2; }

    deploySoldiers(mIdxA, mIdxB);
    if (attacker > defender) {
        monarch[mIdxB].isRemoved = true;
        alliance[monarch[mIdxA].gIdx].addNode(monarchIdx);
        addMonarch(mGeneral, attacker - defender - alliance[monarch[mIdxA].gIdx].nSoldiers
            , monarch[mIdxA].gIdx, monarch[mIdxB].r, monarch[mIdxB].c);

        return 1;
    }
    else {
        monarch[mIdxB].nSoldiers = defender - attacker - alliance[monarch[mIdxB].gIdx].nSoldiers;
        return 0;
    }
}

int recruit(char mMonarch[11], int mNum, int mOption)
{
    if (mOption) {
        int mIdx = hashMonarch[mMonarch];
        int gIdx = monarch[mIdx].gIdx;
        alliance[gIdx].nSoldiers += mNum;
        int ans = 0;

        Node* cNode = alliance[gIdx].head;
        while (cNode) {
            if (!monarch[cNode->mIdx].isRemoved) {
                ans += monarch[cNode->mIdx].nSoldiers;
                ans += alliance[gIdx].nSoldiers;
            }
            cNode = cNode->next;
        }

        return ans;
    }
    else {
        int mIdx = hashMonarch[mMonarch];
        monarch[mIdx].nSoldiers += mNum;
        return monarch[mIdx].nSoldiers + alliance[monarch[mIdx].gIdx].nSoldiers;
    }
}