#include <string>
#include <unordered_map>

using namespace std;

#define MAXNODE 201
#define MAXREQS 5001

class File {
public:
    string name;
    int filesize;
};

int fIdx;
File files[MAXREQS];

class Node {
public:
    int dist;
    int idx;
};

class Queue {
public:
    Node arr[MAXNODE];
    int front, back, length;

    void init() {
        front = 0;
        back = 0;
        length = 0;
    }

    void push(int i, int d) {
        Node last = Node();
        last.dist = d; last.idx = i;
        length++;
        arr[back++] = last;
    }

    Node pop() {
        length--;
        return arr[front++];
    }
};

class User {
public:
    int adjacent[MAXNODE];
    int length;
    bool files[MAXREQS];
    int nFiles;
    int prev;

    void init() {
        length = 0;
        nFiles = 0;
        prev = -1;

        for (int i = 0; i < MAXREQS; i++) {
            files[i] = false;
        }
    }

    void push_adjacent(int a) {
        adjacent[length++] = a;
    }

    void add_file(int idx) {
        if (!files[idx]) {
            files[idx] = true;
            nFiles++;
        }
    }
};

class Req {
public:
    int routes[MAXNODE];
    int length;
    int requestUser;
    int sharedIdx;

    void init() {
        length = 0;
    }
};

Req reqs[MAXREQS];
int bandwidth[MAXNODE][MAXNODE];
User users[MAXNODE];
unordered_map<string, int> hashWord;

int visited[MAXNODE];
int nBfs;
Queue queue;

void init(int N, int mUID1[], int mUID2[], int mBandwidth[])
{
    hashWord.clear();
    nBfs = 1;
    fIdx = 0;

    for (int i = 0; i < MAXNODE; i++) {
        users[i].init();
        visited[i] = 0;
    }

    for (int i = 0; i < N; i++) {
        users[mUID1[i]].push_adjacent(mUID2[i]);
        users[mUID2[i]].push_adjacent(mUID1[i]);
    }

    for (int i = 0; i < N; i++) {
        bandwidth[mUID1[i]][mUID2[i]] = mBandwidth[i];
        bandwidth[mUID2[i]][mUID1[i]] = mBandwidth[i];
    }
}

int share(int mUID, char mFilename[], int mFilesize)
{
    unordered_map<string, int>::iterator itr = hashWord.find(mFilename);
    if (itr == hashWord.end()) {
        files[fIdx].name = mFilename;
        files[fIdx].filesize = mFilesize;

        hashWord[mFilename] = fIdx;
        users[mUID].add_file(fIdx);

        fIdx++;
        return users[mUID].nFiles;
    }

    users[mUID].add_file(itr->second);
    return users[mUID].nFiles;
}

int request(int mUID, char mFilename[], int mTID)
{
    reqs[mTID].init();
    queue.init();

    unordered_map<string, int>::iterator itr = hashWord.find(mFilename);
    if (itr == hashWord.end()) { return -1; }

    queue.push(mUID, 0);
    visited[mUID] = nBfs;
    users[mUID].prev = -1;

    Node curr;
    Node dest;
    dest.idx = -1; dest.dist = MAXREQS;

    while (queue.length > 0) {
        curr = queue.pop();
        
        if (users[curr.idx].files[itr->second]) {
            if (curr.dist < dest.dist || (curr.dist == dest.dist && curr.idx < dest.idx)) {
                dest = curr;
            }
        }

        if (curr.dist > dest.dist) { continue; }
        for (int i = 0; i < users[curr.idx].length; i++) {
            if (visited[users[curr.idx].adjacent[i]] < nBfs &&
                bandwidth[curr.idx][users[curr.idx].adjacent[i]] >= files[itr->second].filesize) {

                users[users[curr.idx].adjacent[i]].prev = curr.idx;
                visited[users[curr.idx].adjacent[i]] = nBfs;
                queue.push(users[curr.idx].adjacent[i], curr.dist + 1);
            }
        }
    }

    nBfs++;

    if (dest.idx == -1) { return -1; }
    int ans = dest.idx;
    curr = dest;

    reqs[mTID].length = curr.dist + 1;
    reqs[mTID].requestUser = mUID;
    reqs[mTID].sharedIdx = itr->second;
    while (users[curr.idx].prev > -1) {
        reqs[mTID].routes[curr.dist] = curr.idx;
        bandwidth[curr.idx][users[curr.idx].prev] -= files[itr->second].filesize;
        bandwidth[users[curr.idx].prev][curr.idx] -= files[itr->second].filesize;

        curr.idx = users[curr.idx].prev;
        curr.dist--;
    }
    reqs[mTID].routes[curr.dist] = curr.idx;

    return ans;
}

int complete(int mTID)
{
    for (int i = 0; i < reqs[mTID].length; i++) {
        users[reqs[mTID].routes[i]].add_file(reqs[mTID].sharedIdx);

        if (i == reqs[mTID].length - 1) { continue; }
        bandwidth[reqs[mTID].routes[i]][reqs[mTID].routes[i + 1]] += files[reqs[mTID].sharedIdx].filesize;
        bandwidth[reqs[mTID].routes[i + 1]][reqs[mTID].routes[i]] += files[reqs[mTID].sharedIdx].filesize;
    }

    return users[reqs[mTID].requestUser].nFiles;
} 