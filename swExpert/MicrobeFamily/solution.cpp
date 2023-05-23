#include <unordered_map>
#include <cstring>
#include <string>

using namespace std;

#define MAXMICROBE 12001
#define SUBSET 1000

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

int mIdx;
Microbe microbes[MAXMICROBE];
unordered_map<string, int> hashMicrobe;
Grid grid[SUBSET + 1];

int commonAncestor;
int visited[MAXMICROBE];
int nVisited;

void getLowestCommon(int idx) {
    if (idx == 0) {
        commonAncestor = 0;
        return;
    }

    if (visited[idx] < nVisited) {
        visited[idx] = nVisited;
    }

    else {
        commonAncestor = idx;
        return;
    }

    getLowestCommon(microbes[idx].parent);
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
    nVisited = 0;
    hashMicrobe.clear();

    mIdx = 0;

    strcpy(microbes[mIdx].name, mAncestor);
    microbes[mIdx].dist = 0;
    microbes[mIdx].parent = -1;

    hashMicrobe[mAncestor] = mIdx;
    visited[mIdx] = 0;

    mIdx++;

    for (int i = 0; i < SUBSET + 1; i++) {
        grid[i].init();
    }

    addGrid(0, mLastday);

    return;
}



int add(char mName[], char mParent[], int mFirstday, int mLastday)
{
    hashMicrobe[mName] = mIdx;
    
    unordered_map<string, int>::iterator itr = hashMicrobe.find(mParent);

    strcpy(microbes[mIdx].name, mName);
    microbes[mIdx].parent = itr->second;
    microbes[mIdx].dist = microbes[itr->second].dist + 1;

    visited[mIdx] = 0;

    mIdx++;

    addGrid(mFirstday, mLastday);

    return microbes[mIdx - 1].dist;
}



int distance(char mName1[], char mName2[])
{
    unordered_map<string, int>::iterator itr1 = hashMicrobe.find(mName1);
    unordered_map<string, int>::iterator itr2 = hashMicrobe.find(mName2);

    nVisited++;

    getLowestCommon(itr1->second);
    getLowestCommon(itr2->second);

    return microbes[itr1->second].dist + microbes[itr2->second].dist - 2 * microbes[commonAncestor].dist;
}


int count(int mDay)
{
    int idx = mDay / SUBSET;
    int subIdx = mDay % SUBSET;

    return grid[idx].subtotal + grid[idx].cnt[subIdx];
}
