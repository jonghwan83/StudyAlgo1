#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

#define MAXGENE 20000

class Gene {
public:
    int id;
    string sequence;
    bool isRemoved;
};

Gene genes[MAXGENE];
int gIdx;
unordered_map<string, vector<int> > hashSeq;
unordered_map<int, int> hashID;

void init()
{
    gIdx = 0;
    hashSeq.clear();
    hashID.clear();
}

int addSeq(int mID, int mLen, char mSeq[])
{
    unordered_map<int, int>::iterator itr_id = hashID.find(mID);
    if (itr_id != hashID.end() && !genes[itr_id->second].isRemoved) { return 0; }

    string key = "";
    for (int i = 0; i < 3; i++) { key += mSeq[i]; }
    unordered_map<string, vector<int> >::iterator itr_str = hashSeq.find(key);

    for (int idx : hashSeq[key]) {
        if (genes[idx].sequence == mSeq && !genes[idx].isRemoved) { return 0; }
    }

    hashID[mID] = gIdx;
    hashSeq[key].push_back(gIdx);

    genes[gIdx].id = mID;
    genes[gIdx].sequence = mSeq;
    genes[gIdx].isRemoved = false;

    gIdx++;
    return 1;
}

int searchSeq(int mLen, char mBegin[])
{
    string key = "";
    for (int i = 0; i < 3; i++) { key += mBegin[i]; }
    unordered_map<string, vector<int> >::iterator itr_str = hashSeq.find(key);
    if (itr_str == hashSeq.end()) { return -1; }

    int ans = 0;
    int geneIdx = -1;

    if (mLen == 3) {
        for (int idx : hashSeq[key]) {
            if (genes[idx].isRemoved) { continue; }
            ans++;
            geneIdx = idx;
        }

        if (ans == 0) { return -1; }
        if (ans == 1) { ans = genes[geneIdx].id; }

        return ans;
    }

    bool isContinue;
    for (int idx : hashSeq[key]) {
        if (genes[idx].isRemoved) { continue; }
        isContinue = false;
        for (int i = 3; i < mLen; i++) {
            if (genes[idx].sequence[i] != mBegin[i]) {
                isContinue = true;
                break;
            }
        }
        if (isContinue) { continue; }
        ans++;
        geneIdx = idx;
    }

    if (ans == 0) { return -1; }
    if (ans == 1) { ans = genes[geneIdx].id; }

    return ans;
}

int eraseSeq(int mID)
{
    unordered_map<int, int>::iterator itr_id = hashID.find(mID);
    if (itr_id == hashID.end()) { return 0; }
    if (genes[itr_id->second].isRemoved) { return 0; }

    genes[itr_id->second].isRemoved = true;

    return 1;
}

int changeBase(int mID, int mPos, char mBase)
{
    unordered_map<int, int>::iterator itr_id = hashID.find(mID);
    if (itr_id == hashID.end()) { return 0; }

    if (genes[itr_id->second].isRemoved) { return 0; }
    if (mPos >= genes[itr_id->second].sequence.size()) { return 0; }
    if (mBase == genes[itr_id->second].sequence[mPos]) { return 0; }

    string newSeq = genes[itr_id->second].sequence;
    newSeq[mPos] = mBase;

    string key = "";
    for (int i = 0; i < 3; i++) { key += newSeq[i]; }
    for (int idx : hashSeq[key]) {
        if (genes[idx].sequence == newSeq && !genes[idx].isRemoved) { return 0; }
    }

    genes[gIdx].id = genes[itr_id->second].id;
    genes[gIdx].sequence = newSeq;
    genes[gIdx].isRemoved = false;

    genes[itr_id->second].isRemoved = true;

    hashSeq[key].push_back(gIdx);
    hashID[genes[itr_id->second].id] = gIdx;

    gIdx++;
    return 1;
}