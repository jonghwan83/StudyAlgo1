#define DSIZE 1000001
#define MAXNODE 50001

#include <vector>
#include <algorithm>

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
    bool isComplete;
    PC() {
        id = -1;
        eTime = -1;
        isComplete = false;
        capacity = 0;
    }
};

class Clients {
public:
    vector<int> children;
    int cID;
    int speed;
    bool isPC;
    int nConnected;
    int sTime;
    Clients() {
        cID = -1;
        speed = 0;
        isPC = false;
        nConnected = 0;
        sTime = 0;
        children.clear();
    }
};

vector<PC> downloads(MAXNODE);
vector<Clients> serverTree(MAXNODE);
vector<int> idxParent(MAXNODE);
vector<int> pclist(MAXNODE);
vector<int> subPCs;
pair<int, int> firstFinTime;
int currentTime;

void updateConnection(int hubID) {
    int parent = idxParent[hubID];
    if (serverTree[hubID].isPC) { serverTree[parent].nConnected++; }
    else {
        if (serverTree[hubID].nConnected == 1) { serverTree[parent].nConnected++; }
        else { return; }
    }
    if (parent == 0) { return; }
    updateConnection(parent);
}

void cutConnection(int hubID) {
    int parent = idxParent[hubID];
    if (serverTree[hubID].isPC) { serverTree[parent].nConnected--; }
    else {
        if (serverTree[hubID].nConnected == 0) { serverTree[parent].nConnected--; }
        else { return; }
    }
    if (parent == 0) { return; }
    cutConnection(parent);
}

void getFirstFinTime() {
    firstFinTime = make_pair(DSIZE, -1);
    int capa, fTime;
    vector<pair<int, int> > templist;
    for (int pcID : pclist) {
        capa = downloads[pcID].capacity - (serverTree[pcID].speed * (currentTime - serverTree[pcID].sTime));
        if (capa <= 0 && !downloads[pcID].isComplete) {
            fTime = currentTime + (capa / serverTree[pcID].speed);
            templist.push_back(make_pair(-fTime, pcID));
            push_heap(templist.begin(), templist.end());
        }
    }
    if (!templist.empty()) {
        firstFinTime.first = -templist.front().first;
        firstFinTime.second = templist.front().second;
    }
}

void executeDownload(int pcID, int cTime) {
    downloads[pcID].capacity -= serverTree[pcID].speed * (cTime - serverTree[pcID].sTime);
    if (downloads[pcID].capacity <= 0) {
        downloads[pcID].eTime = cTime + (downloads[pcID].capacity / serverTree[pcID].speed);
        downloads[pcID].isComplete = true;
        serverTree[pcID].speed = 0;
    }
    serverTree[pcID].sTime = cTime;
}

void updateSpeed(int hubID, int cTime) {
    for (int child : serverTree[hubID].children) {
        if (serverTree[child].isPC && !downloads[child].isComplete) {
            executeDownload(child, cTime);
            if (downloads[child].isComplete) {
                cutConnection(child);
                updateSpeed(0, cTime);
            }
            else {
                serverTree[child].speed = serverTree[hubID].speed / serverTree[hubID].nConnected;
            }
        }
        if (serverTree[child].nConnected == 0) { continue; }
        serverTree[child].speed = serverTree[hubID].speed / serverTree[hubID].nConnected;
        updateSpeed(child, cTime);
    }
}

void updateNet() {
    getFirstFinTime();
    while (firstFinTime.first < DSIZE) {
        executeDownload(firstFinTime.second, firstFinTime.first);
        cutConnection(firstFinTime.second);
        updateSpeed(0, firstFinTime.first);
        getFirstFinTime();
    }
}

void checkSubPC(int hubID) {
    for (int child : serverTree[hubID].children) {
        if (serverTree[child].isPC && !downloads[child].isComplete) {
            subPCs.push_back(child);
        }
        checkSubPC(child);
    }
}

void initVar(int hubID, Clients client, PC pc) {
    for (int child : serverTree[hubID].children) {
        initVar(child, client, pc);
        serverTree[child] = client;
        downloads[child] = pc;
    }
    serverTree[hubID] = client;
    downloads[hubID] = pc;
}

void init(int mCapa)
{
    currentTime = 0;
    firstFinTime = make_pair(DSIZE, -1);

    pclist.clear();
    subPCs.clear();
    Clients client;
    PC pc;
    initVar(0, client, pc);

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
    subPCs.clear();

    updateNet();
    updateSpeed(0, currentTime);
    checkSubPC(mID);

    if (subPCs.empty()) {
        return 1;
    }
    return 0;
}

void requestDL(int mTime, int mParentID, int mpcID, int mSize)
{

    currentTime = mTime - 1;
    updateNet();
    updateSpeed(0, currentTime);

    pclist.push_back(mpcID);
    currentTime = mTime;
    serverTree[mParentID].children.push_back(mpcID);
    serverTree[mpcID].cID = mpcID;
    serverTree[mpcID].isPC = true;
    serverTree[mpcID].sTime = mTime;

    idxParent[mpcID] = mParentID;
    downloads[mpcID].capacity = mSize;

    updateConnection(mpcID);
    updateSpeed(0, currentTime);
}

Result checkPC(int mTime, int mpcID)
{
    Result res;
    res.finish = 0;
    res.param = 0;

    currentTime = mTime;

    updateNet();
    updateSpeed(0, currentTime);

    res.finish = downloads[mpcID].isComplete;
    if (res.finish) {
        res.param = downloads[mpcID].eTime;
    }
    else {
        res.param = downloads[mpcID].capacity;
    }
    return res;
}