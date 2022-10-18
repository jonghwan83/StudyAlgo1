#include <vector>
#include <unordered_map>

using namespace std;


struct Result {
    int finish;
    int param;
};

class PC {
public:
    int id;
    int capacity;
    int sTime;
    int speed;
    int eTime;
    bool isComplete;
    bool isRemoved;

    PC (int pcID, int capa, int start, int sp) {
        id = pcID;
        capacity = capa;
        sTime = start;
        speed = sp;
        isComplete = false;
        isRemoved = false;
    }
};

class Hub {
public:
    int id;
    int speed;
    int nClient;
    vector<int> children;
    vector<PC> clients;
};

unordered_map<int, Hub> fileTree;
unordered_map<int, int> idParent;
unordered_map<int, pair<int, int> > idxPC;

void updatePC(int parent, int cTime) {
    int tStart;
    for (int i=0; i < fileTree[parent].clients.size(); i++) {
        tStart = fileTree[parent].clients[i].sTime;
        fileTree[parent].clients[i].capacity -= (cTime - tStart) * fileTree[parent].clients[i].speed;
        fileTree[parent].clients[i].sTime = cTime;
        fileTree[parent].clients[i].speed = (int)fileTree[parent].speed / fileTree[parent].nClient;
        if (fileTree[parent].clients[i].capacity <= 0) {
            fileTree[parent].clients[i].eTime = cTime + fileTree[parent].clients[i].capacity / fileTree[parent].clients[i].speed;
        }
    }
}

void updateHub(int hubID, int cTime) {
    updatePC(hubID, cTime);
    for (int i=0; i < fileTree[hubID].children.size(); i++) {
        updateHub(fileTree[hubID].children[i], cTime);
    }
}

int sumClinets(int parent) {
    int ans = 0;
    for (int i=0; i < fileTree[parent].children.size(); i++) {
        if (fileTree[fileTree[parent].children[i]].nClient > 0) {
            ans++;
        }
    }
    return ans + fileTree[parent].clients.size();
}

void updateClients(int node) {
    if (node == 0) {
        fileTree[node].nClient = sumClinets(node);
        return;
    }
    fileTree[node].nClient = sumClinets(node);
    updateClients(idParent[node]);
}

void updateSpeed(int node, int cTime) {
    int childNode;
    if (node == 0) {
        updatePC(node, cTime);
    }
    for (int i=0; i < (int)fileTree[node].children.size(); i++) {
        childNode = fileTree[node].children[i];
        if (fileTree[node].nClient < 1) {
            continue;
        }
        fileTree[childNode].speed = (int)(fileTree[node].speed / fileTree[node].nClient);
        updatePC(childNode, cTime);
        updateSpeed(childNode, cTime);
    }
}

void init(int mCapa)
{
    fileTree.clear();
    idParent.clear();
    idxPC.clear();

    fileTree[0].id = 0;
    fileTree[0].speed = mCapa;
}

void addHub(int mTime, int mParentID, int mID)
{
    fileTree[mParentID].id = mParentID;
    fileTree[mParentID].children.push_back(mID);
    fileTree[mID].id = mID;

    idParent[mID] = mParentID;
}

int removeHub(int mTime, int mID)
{
    updateHub(mID, mTime);
    return -1;
}

void requestDL(int mTime, int mParentID, int mpcID, int mSize)
{
    PC newPC(mpcID, mSize, mTime, fileTree[mParentID].speed);
    fileTree[mParentID].clients.push_back(newPC);

    idxPC[mpcID] = make_pair(mParentID, fileTree[mParentID].clients.size() - 1);

    updateClients(mParentID);
    updateSpeed(0, mTime);
}

Result checkPC(int mTime, int mpcID)
{
    Result res;
    res.finish = 0;
    res.param = 0;

    int parent = idxPC[mpcID].first;
    int idx = idxPC[mpcID].second;
    updatePC(parent, mTime);

    res.finish = fileTree[parent].clients[idx].isComplete;
    if (res.finish) {
        res.param = fileTree[parent].clients[idx].eTime;
    }
    else {
        res.param = fileTree[parent].clients[idx].capacity;
    }
    return res;
}