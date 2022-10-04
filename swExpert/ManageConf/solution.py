#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;


unordered_map<string, vector< vector<string> > > project;       // key: branch, value: file, data
unordered_map<string, vector< vector<int> > > projectTime;      // key: branch, vlaue: created time, edited time, vector index
unordered_map<string, unordered_map<string, int> > projectIdx;
map<string, vector<string> > projectTree;         // key: branch, value: child node
unordered_map<string, string> hashTable;        // key: child node, value: parent node


bool compare(const vector<int> a, vector<int> b) { return a[0] < b[0]?false:true; }

void init() {
    project.clear();
    projectTime.clear();

    // add root node
    vector<string> root = { };
    projectTree["root"] = root;
    hashTable.clear();
}

void create(int mTime, char mBranch[], char mFile[], char mData[]) {
    if (project[mBranch].size() == 0) { 
        vector<string> temp = { };
        project[mBranch].push_back(temp); 
    }

    vector<string> file = { mFile, mData, "on" };
    vector<int> time = { mTime, mTime, (int)project[mBranch].size() };

    projectIdx[mBranch][mFile] = (int)project[mBranch].size();
    project[mBranch].push_back(file);

    projectTime[mBranch].push_back(time);
    push_heap(projectTime[mBranch].begin(), projectTime[mBranch].end(), compare);

    if (projectTime[mBranch].size() > 50) {
        int idxFront = projectTime[mBranch][0][2];
        pop_heap(projectTime[mBranch].begin(), projectTime[mBranch].end(), compare);
        project[mBranch][idxFront][2] = "off";
    }

}

void createFromChild(int mTime, string mBranch, string mFile, string mData, int eTime) {
    if (project[mBranch].size() == 0) { 
        vector<string> temp = { };
        project[mBranch].push_back(temp); 
    }

    vector<string> file = { mFile, mData };
    vector<int> time = { mTime, eTime, (int)project[mBranch].size() };

    projectIdx[mBranch][mFile] = (int)project[mBranch].size();
    project[mBranch].push_back(file);

    if (projectTime[mBranch].size() >= 50) {
        pop_heap(projectTime[mBranch].begin(), projectTime[mBranch].end(), compare);
        projectTime[mBranch].pop_back();
    }

    projectTime[mBranch].push_back(time);
    push_heap(projectTime[mBranch].begin(), projectTime[mBranch].end(), compare);

    cout << projectTime[mBranch].size() << endl;
}

void copyFile(string mChild, string mFile, string mData, int eTime, int cTime) {
    string parent = hashTable[mChild];
    int idxParent = -1;
    if (projectIdx[parent][mFile] > 0 && project[parent][projectIdx[parent][mFile]][1] == "on") {
        idxParent = projectIdx[parent][mFile];
    }

    int cTimeParent, eTimeParent;
    if (idxParent == -1) {
        // create from child
        createFromChild(cTime, parent, mFile, mData, eTime);
    }
    else {
        for (int i=0; i < projectTime[parent].size(); i++) {
            if (idxParent == projectTime[parent][i][2]) {
                cTimeParent = projectTime[parent][i][0];
                eTimeParent = projectTime[parent][i][1];
                if (cTime == cTimeParent && eTime > eTimeParent) {
                    project[parent][idxParent][1] = mData;
                    projectTime[parent][i][1] = eTime;
                }
                break;
            }
        }
    }
}

void mergeChild(string mChild) {
    int idxChild, cTime, eTime;
    string fileChild, dataChild;
    while (projectTime[mChild].size() > 0) {
        cTime = projectTime[mChild][0][0];
        eTime = projectTime[mChild][0][1];
        idxChild = projectTime[mChild][0][2];
        fileChild = project[mChild][idxChild][0];
        dataChild = project[mChild][idxChild][1];

        pop_heap(projectTime[mChild].begin(), projectTime[mChild].end(), compare);
        projectTime[mChild].pop_back();

        copyFile(mChild, fileChild, dataChild, eTime, cTime);
    }
    project[mChild].clear();
    projectTree[mChild].clear();
    projectIdx[mChild].clear();
}

void callTree(string mBranch) {
    if (projectTree[mBranch].size() == 0) {
        mergeChild(mBranch);
        return;
    }

    for (int i=0; i < projectTree[mBranch].size(); i++) {
        callTree(projectTree[mBranch][i]);
    }
    mergeChild(mBranch);
    return;
}


void edit(int mTime, char mBranch[], char mFile[], char mData[]) {
    int idx = projectIdx[mBranch][mFile];
    project[mBranch][idx][1] = mData;
    for (int i=0; i < projectTime[mBranch].size(); i++) {
        if (projectTime[mBranch][i][2] == idx) {
            projectTime[mBranch][i][1] = mTime;
        }
    }
}

void branch(int mTime, char mBranch[], char mChild[]) {
    project[mChild] = project[mBranch];
    projectTime[mChild] = projectTime[mBranch];
    projectIdx[mChild] = projectIdx[mBranch];

    projectTree[mBranch].push_back(mChild);
    hashTable[mChild] = mBranch;
}

void merge(int mTime, char mBranch[]) {
    callTree(mBranch);
}

int readfile(int mTime, char mBranch[], char mFile[], char retString[]) {
    int lenString = 0;
    int idx = projectIdx[mBranch][mFile];
    string data = project[mBranch][idx][1];
    for (int i=0; i < data.size(); i++) {
        lenString++;
        retString[i] = data[i];
    }
    retString[data.size()] = '\0';
    cout << idx << " " << retString << endl;
    cout << projectTime[mBranch][0][2] << endl;
    return lenString;
}
