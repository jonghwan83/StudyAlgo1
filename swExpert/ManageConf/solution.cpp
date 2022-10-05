#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <iostream>

using namespace std;


class FileDetail {
public:
    int cTime;
    int eTime;
    string fname;
    string data;
    bool isExist;

    FileDetail(int ctime, int etime, string filename, string content) {
        cTime = ctime;
        eTime = etime;
        fname = filename;
        data = content;
        isExist = true;
    }
};

bool compare(pair<int, int> a, pair<int, int> b) { return a.first < b.first ? false : true; }

unordered_map<string, vector< pair<int, int> > > pQueue;    // key: branch, filename value: cTime, index
unordered_map<string, vector<FileDetail> > project;		// key: Branch, value: info
unordered_map<string, unordered_map<string, int > > dict;        // value: index
map<string, vector<string> > projectTree;
unordered_map<string, string> hashParent;

void init() {
    pQueue.clear();
    project.clear();
    dict.clear();
    projectTree.clear();
    hashParent.clear();

    vector<string> temp;
    projectTree["root"] = temp;

    return;
}

void create(int mTime, char mBranch[], char mFile[], char mData[]) {
    FileDetail temp(mTime, mTime, mFile, mData);

    if (pQueue[mBranch].size() >= 50) {
        int idxFront = pQueue[mBranch][0].second;
        project[mBranch][idxFront].isExist = false;

        pop_heap(pQueue[mBranch].begin(), pQueue[mBranch].end(), compare);
        pQueue[mBranch].pop_back();
    }

    project[mBranch].push_back(temp);
    pQueue[mBranch].push_back(make_pair(mTime, (int)project[mBranch].size() - 1));
    push_heap(pQueue[mBranch].begin(), pQueue[mBranch].end(), compare);
    dict[mBranch][mFile] = (int)project[mBranch].size() - 1;

    return;
}

void createFromChild(int mTime, string mBranch, string mFile, string mData, int eTime) {
    FileDetail temp(mTime, eTime, mFile, mData);

    project[mBranch].push_back(temp);
    pQueue[mBranch].push_back(make_pair(mTime, (int)project[mBranch].size() - 1));

    push_heap(pQueue[mBranch].begin(), pQueue[mBranch].end(), compare);
    dict[mBranch][mFile] = (int)project[mBranch].size() - 1;

    return;
}

void checkBranch(string mBranch) {
    // check if branch size > 50 after merging
    int idxFront;
    while (pQueue[mBranch].size() > 50) {
        idxFront = pQueue[mBranch][0].second;
        project[mBranch][idxFront].isExist = false;

        pop_heap(pQueue[mBranch].begin(), pQueue[mBranch].end(), compare);
        pQueue[mBranch].pop_back();
    }
    return;
}

void copyFile(string mChild, string mFile, string mData, int cTime, int eTime) {
    string parent = hashParent[mChild];
    int idx = dict[parent][mFile];

    if ((project[parent][idx].isExist) && (project[parent][idx].fname == mFile)) {
        if (cTime == project[parent][idx].cTime && eTime > project[parent][idx].eTime) {
            project[parent][idx].eTime = eTime;
            project[parent][idx].data = mData;
        }
    }
    else {
        createFromChild(cTime, parent, mFile, mData, eTime);
    }
    return;
}

void mergeToParent(string mBranch) {

    int idx;
    for (int i = 0; i < pQueue[mBranch].size(); i++) {
        idx = pQueue[mBranch][i].second;
        copyFile(mBranch, project[mBranch][idx].fname, project[mBranch][idx].data, project[mBranch][idx].cTime, project[mBranch][idx].eTime);
    }

    // check parent node size
    string parent = hashParent[mBranch];
    checkBranch(parent);

    pQueue[mBranch].clear();
    project[mBranch].clear();
    dict[mBranch].clear();
    projectTree[mBranch].clear();

    return;
}

void callTree(string mBranch) {

    if (projectTree[mBranch].size() == 0) {
        mergeToParent(mBranch);
        return;
    }
    int idx;
    for (int i = 0; i < projectTree[mBranch].size(); i++) {
        callTree(projectTree[mBranch][i]);
    }

    mergeToParent(mBranch);
    return;
}

void edit(int mTime, char mBranch[], char mFile[], char mData[]) {
    int idx = dict[mBranch][mFile];
    project[mBranch][idx].data = mData;
    project[mBranch][idx].eTime = mTime;

    return;
}

void branch(int mTime, char mBranch[], char mChild[]) {
    project[mChild] = project[mBranch];
    pQueue[mChild] = pQueue[mBranch];
    dict[mChild] = dict[mBranch];

    projectTree[mBranch].push_back(mChild);
    hashParent[mChild] = mBranch;

    return;
}

void merge(int mTime, char mBranch[]) {
    callTree(mBranch);
}

int readfile(int mTime, char mBranch[], char mFile[], char retString[]) {
    int lenString = 0;
    int idx = dict[mBranch][mFile];
    string data = project[mBranch][idx].data;

    for (int i = 0; i < data.size(); i++) {
        lenString++;
        retString[i] = data[i];
    }
    retString[data.size()] = '\0';

    return lenString;
}
