#include <vector>

using namespace std;

struct Result {
 int finish;
 int param;
};

class PC {  
public: 
    int id;
    int speed;
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

vector<PC> downloads;   // pc id 로 정보를 검색
vector<Clients> serverTree;
vector<int> idxParent;

void updateConnection(int hubID) {
    int parent = idxParent[hubID];
    if (serverTree[hubID].isPC) {
        serverTree[parent].nConnected++;
    } 
    else {
        if (serverTree[hubID].nConnected == 1) {
            serverTree[parent].nConnected++;
        }
    }
    if (parent == 0) { return; }
    updateConnection(parent);
}

void updateSpeed(int hubID) {
    // from root node

    for (int idx: serverTree[hubID].children) {
        if (serverTree[idx].nConnected == 0) { continue; }
        serverTree[idx].speed = serverTree[hubID].speed / serverTree[hubID].nConnected;   
        if (serverTree[idx].isPC) {
            downloads[idx].speed = serverTree[idx].speed;
        } 
        updateSpeed(idx);
    }
}

void init(int mCapa)
{
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
    serverTree[mParentID].children.push_back(mID);
    serverTree[mID].cID = mID;
    serverTree[mID].sTime = mTime;

    idxParent[mID] = mParentID;
}

int removeHub(int mTime, int mID)
{
    return -1;
}

void requestDL(int mTime, int mParentID, int mpcID, int mSize)
{
    serverTree[mParentID].children.push_back(mpcID);
    serverTree[mpcID].cID = mpcID;
    serverTree[mpcID].isPC = true;
    serverTree[mpcID].sTime = mTime;

    idxParent[mpcID] = mParentID;
    updateConnection(mpcID);
    updateSpeed(0);
}

Result checkPC(int mTime, int mpcID)
{
 Result res;
 res.finish = 0;
 res.param = 0;

 return res;
}