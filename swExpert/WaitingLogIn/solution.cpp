#include <map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

int queueIdx;
int queueSt;
vector<pair<string, int> > loginQueue;
map<string, int> idxDict;
map<string, vector<pair<string, int> > > charDict;

void init()
{
    queueIdx = 0;
    queueSt = 0;

    loginQueue.clear();
    idxDict.clear();
    charDict.clear();
}

void loginID(char mID[10])
{
    map<string, int>::iterator it;
    it = idxDict.find(mID);
    if (it != idxDict.end())
    {
        loginQueue[idxDict[mID]].second = 1;
    }
    
    loginQueue.push_back(make_pair(mID, 0));
    idxDict[mID] = queueIdx;

    string s = mID;
    string subS = s.substr(0, 3);
    charDict[subS].push_back(make_pair(s, queueIdx));

    queueIdx++;
    return;
}

int closeIDs(char mStr[10])
{
    int count = 0;
    string s = mStr;
    int le = s.length();
    string subS = s.substr(0, 3);

    int k = charDict[subS].size();
    for (int i=0; i < k; i++)
    {
        int qIdx = charDict[subS][i].second;
        if ((s == charDict[subS][i].first.substr(0, le)) && (!loginQueue[qIdx].second))
        {
            count++;
            loginQueue[qIdx].second = 1;
        }
    }
    return count;
}

void connectCnt(int mCnt)
{
    int k = 0;
    while (k < mCnt)
    {
        if (loginQueue[queueSt].second)
        {
            loginQueue[queueSt].second = 1;
            queueSt++;
        }
        else
        {
            loginQueue[queueSt].second = 1;
            queueSt++;
            k += 1;
        }
    }
    
    return;
}

int waitOrder(char mID[10])
{
    int count = 0;
    map<string, int>::iterator it;
    
    it = idxDict.find(mID);
    if (it == idxDict.end())
    {
        return 0;
    }

    int idx = idxDict[mID];
    if (loginQueue[idx].second)
    {
        return 0;
    }

    for (int i=queueSt; i < queueIdx; i++)
    {
        if (!loginQueue[i].second)
        {
            count++;
        }
        if ((loginQueue[i].first == mID) && (!loginQueue[i].second))
        {
            break;
        }
    }
    return count;
}

