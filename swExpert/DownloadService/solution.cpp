#include <vector>
#include <iostream>

using namespace std;

struct Result {
 int finish;
 int param;
};

class PC {  
public: 
    int id;
    int capacity;
    int eTime;
};

class Clients {
public:
    vector<int> children;
    int cID;
    int speed;
    bool isPC;
    int nConnected;
    int sTime;
};

vector<PC> downloads;
vector<Clients> serverTree;
vector<int> idxParent;
int currentTime;

void updateConnection(int hubID) {
    int parent = idxParent[hubID];
    if (serverTree[hubID].isPC) {
        serverTree[parent].nConnected++;
    }
    else {
        if (serverTree[hubID].nConnected == 1) {
        serverTree[parent].nConnected++;
        } 
        else { return; }    
    }
    
    if (parent == 0) { return; }
    updateConnection(parent);
}

void executeDownload(int pcID) {
    downloads[pcID].capacity -= serverTree[pcID].speed * (currentTime - serverTree[pcID].sTime);
    if (downloads[pcID].capacity <= 0 ) {
        downloads[pcID].eTime = currentTime + (downloads[pcID].capacity / serverTree[pcID].sTime);
    }
    serverTree[pcID].sTime = currentTime;
}

void updateSpeed(int hubID) {
    // from root node
    for (int idx: serverTree[hubID].children) {
        if (serverTree[idx].isPC) {
            executeDownload(idx);
            serverTree[idx].speed = serverTree[hubID].speed / serverTree[hubID].nConnected;
        }
        if (serverTree[idx].nConnected == 0) { continue; }
        serverTree[idx].speed = serverTree[hubID].speed / serverTree[hubID].nConnected;
        updateSpeed(idx);
    }
}

void updateHub(int hubID) {
    if (serverTree[hubID].children.empty()) { return; }
    for (int idx: serverTree[hubID].children) {
        if (serverTree[idx].isPC) { executeDownload(idx); }
        updateHub(idx);
    }
}

void init(int mCapa)
{
    currentTime = 0;

    downloads.clear();
    serverTree.clear();
    idxParent.clear();

    serverTree.resize(50001);
    downloads.resize(50001);
    idxParent.resize(50001);

    serverTree[0].cID = 0;
    serverTree[0].speed = mCapa;
    serverTree[0].sTime = 0;
}

void addHub(int mTime, int mParentID, int mID)
{
    currentTime = mTime;
    serverTree[mParentID].children.push_back(mID);
    serverTree[mID].cID = mID;
    serverTree[mID].sTime = mTime;

    idxParent[mID] = mParentID;
}

int removeHub(int mTime, int mID)
{
    currentTime = mTime;
    updateHub(mID);
    return -1;
}

void requestDL(int mTime, int mParentID, int mpcID, int mSize)
{
    currentTime = mTime;
    serverTree[mParentID].children.push_back(mpcID);
    serverTree[mpcID].cID = mpcID;
    serverTree[mpcID].isPC = true;
    serverTree[mpcID].sTime = mTime;

    idxParent[mpcID] = mParentID;

    downloads[mpcID].capacity = mSize;

    updateConnection(mpcID);
    updateSpeed(0);

    // printf("speed: %d %d %d\n", serverTree[50].speed, serverTree[0].nConnected, downloads[50].capacity);
}

Result checkPC(int mTime, int mpcID)
{
 Result res;
 res.finish = 0;
 res.param = 0;

 return res;
}
