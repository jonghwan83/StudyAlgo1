#include <string>
#include <unordered_map>

using namespace std;

#define MAXL 11

#define MAXSPECIES 50000
#define MAXDISTANCE 5

class Species {
public:
    int parent;
    int distance;
    int children[400];
    int length;
    int nChildren[MAXDISTANCE];

    void init() {
        length = 0;
        parent = -1;
        distance = 0;

        for (int i = 0; i < MAXDISTANCE; i++) {
            nChildren[i] = 0;
        }
        nChildren[0] = 1;
    }

    void push_child(int a) {
        children[length++] = a;
    }
};

int sIdx;
unordered_map<string, int> hashSpecies;
Species species[MAXSPECIES];
int cnt;

int LCA(int idx1, int idx2) {
    while (idx1 != idx2) {
        if (idx1 > idx2) {
            idx1 = species[idx1].parent;
        }
        else {
            idx2 = species[idx2].parent;
        }
    }

    return idx1;
}

void init(char mRootSpecies[MAXL])
{
    sIdx = 0;
    hashSpecies.clear();

    species[sIdx].init();
    hashSpecies[mRootSpecies] = sIdx;
    sIdx++;
}

void add(char mSpecies[MAXL], char mParentSpecies[MAXL])
{
    int parent = hashSpecies[mParentSpecies];

    species[sIdx].init();
    species[sIdx].distance = species[parent].distance + 1;
    species[parent].push_child(sIdx);
    species[sIdx].parent = parent;

    hashSpecies[mSpecies] = sIdx;

    int curr = sIdx;
    int k = 1;
    while (k < MAXDISTANCE) {
        curr = species[curr].parent;
        if (curr == -1) { break; }
        species[curr].nChildren[k]++;
        k++;
    }

    sIdx++;
}

int getDistance(char mSpecies1[MAXL], char mSpecies2[MAXL])
{
    int idx1 = hashSpecies[mSpecies1];
    int idx2 = hashSpecies[mSpecies2];

    int ancestor = LCA(idx1, idx2);

    return species[idx1].distance + species[idx2].distance - 2 * species[ancestor].distance;
}

void countNeighbor(int parent, int excludeIdx, int dist) {
    if (parent == -1) { return; }
    if (dist == 0) {
        cnt++;
        return;
    }

    for (int i = 0; i < species[parent].length; i++) {
        if (species[parent].children[i] != excludeIdx) {
            cnt += species[species[parent].children[i]].nChildren[dist - 1];
        }
    }

    countNeighbor(species[parent].parent, parent, dist - 1);
}

int getCount(char mSpecies[MAXL], int mDistance)
{
    cnt = 0;
    int idx = hashSpecies[mSpecies];

    countNeighbor(species[idx].parent, idx, mDistance - 1);

    return cnt + species[idx].nChildren[mDistance];
}
