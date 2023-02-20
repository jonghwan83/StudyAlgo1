#include <string>
#include <vector>
#include <unordered_map>
#include <deque>

using namespace std;

#define MAXACCOUNT 50000
#define ALPHABET 27

class Account {
public:
    string id;
    bool isWaiting;
    int cnt;

    Account() {
        id = "";
        isWaiting = false;
        cnt = 0;
    }

    void init(char mID[10]) {
        id = mID;
        isWaiting = true;
        cnt++;
    }
};

int idIdx;
Account accounts[MAXACCOUNT];
deque< pair<int, int> > queue;
unordered_map<string, int> hashAccount;
int hashSubId[ALPHABET][ALPHABET][MAXACCOUNT / 2];
int hIdx[ALPHABET][ALPHABET];

void init()
{
    idIdx = 0;
    queue.clear();
    hashAccount.clear();
    for (int i = 0; i < ALPHABET; i++) {
        for (int j = 0; j < ALPHABET; j++) {
            hIdx[i][j] = 0;
        }
    }
}

void loginID(char mID[10])
{
    unordered_map<string, int>::iterator it = hashAccount.find(mID);

    if (it == hashAccount.end()) {
        hashAccount[mID] = idIdx;
        int key1 = (int) mID[0] - 96; int key2 = (int) mID[1] - 96;
        hashSubId[key1][key2][hIdx[key1][key2]++] = idIdx;
        hashSubId[key1][0][hIdx[key1][0]++] = idIdx;

        accounts[idIdx].init(mID);
        queue.push_back(make_pair(idIdx, accounts[idIdx].cnt));
        idIdx++;
    }
    else {
        int idx = hashAccount[mID];
        accounts[idx].isWaiting = true;
        queue.push_back(make_pair(idx, ++accounts[idx].cnt));
    }
}

bool checkStr(char mStr[10], string id) {
    string str = mStr;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != id[i]) {
            return false;
        }
    }
    return true;
}

int closeIDs(char mStr[10])
{
    int ans = 0;
    int key1 = (int) mStr[0] - 96;
    int key2 = 0;
    if (mStr[1] != '\0') { key2 = (int) mStr[1] - 96; }

    for (int i = 0; i < hIdx[key1][key2]; i++) {
        int idx = hashSubId[key1][key2][i];
        if (checkStr(mStr, accounts[idx].id) && accounts[idx].isWaiting) {
            ans++;
            accounts[idx].isWaiting = false;
            accounts[idx].cnt++;
        }
    }

    return ans;
}

void connectCnt(int mCnt)
{
    while (mCnt > 0) {
        pair<int, int> idx = queue.front();
        queue.pop_front();
        if (accounts[idx.first].isWaiting && idx.second == accounts[idx.first].cnt) {
            accounts[idx.first].isWaiting = false;
            mCnt--;
        }
    }
}

int waitOrder(char mID[10])
{
    unordered_map<string, int>::iterator it = hashAccount.find(mID);
    if (it == hashAccount.end()) {
        return 0;
    }

    int idx = hashAccount[mID];
    if (!accounts[idx].isWaiting) { return 0; }

    int ans = 0;
    for (int i = 0; i < queue.size(); i++) {
        if (accounts[queue[i].first].isWaiting && accounts[queue[i].first].cnt == queue[i].second) { ans++; }

        if (accounts[queue[i].first].id == mID && accounts[queue[i].first].isWaiting) {
            if (accounts[queue[i].first].cnt == queue[i].second) {
                break;
            }
        }
    }
    return ans;
}