#define MAXL 10
#define MAXTABLE 1024
#define MAXUSER 10000
#define MAXMESSAGES 50000

#include <cstring>

int djb2(char source[MAXL + 1]) {
    long long int hash = 5381;

    for (int i = 0; i < MAXL + 1; i++) {
        if (source[i] == '\0') { break; }
        hash = ((hash << 5) + hash) + source[i];
    }

    return (int) hash % MAXTABLE;
}


struct Node {
    int value;
    Node* next;
};

struct User {
    char name[MAXL + 1];
    int point;
};



class LinkedList {
public:
    int length;
    Node* head;

    void init() {
        length = 0;
        head = nullptr;
    }

    void push(int idx) {

        Node* node = new Node;
        node->value = idx;

        node->next = head;
        head = node;
        length++;
    }


};

struct Message {
    int userIdx;
    int initPoint;
    int parent;
    int id;
    int point;

    LinkedList children;
};

int uIdx, mIdx;
User users[MAXUSER];

Message messages[MAXMESSAGES];

LinkedList hashUser[MAXTABLE];
LinkedList hashMessage[MAXTABLE];

struct HeapNode {
    int point;
    int userIdx;
    int id;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXMESSAGES];

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].point < arr[child].point) { return true; }
        if (arr[parent].point == arr[child].point &&
            strcmp(users[arr[parent].userIdx].name, users[arr[child].userIdx].name) > 0) { return true; }
        if (arr[parent].point == arr[child].point &&
            arr[parent].id > arr[child].id) {
            return true;
        }
        return false;
    }

    void push(int point, int userIdx, int id) {
        HeapNode node = { point, userIdx, id };

        int idx = length;
        arr[length++] = node;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    HeapNode pop() {
        HeapNode ans = arr[0];

        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;

        while (2 * idx + 1 < length) {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                HeapNode temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }

        return ans;
    }
};


Heap queueUser;
int visitedUser[MAXUSER];
int nVisitedUser;

Heap queueMessage;
int visitedMSG[MAXMESSAGES];
int nVisitedMSG;


void pushByName(int idx, char mUser[MAXL + 1]) {
    int key = djb2(mUser);

    hashUser[key].push(idx);
}

int findByName(char mUser[MAXL + 1]) {
    int key = djb2(mUser);

    Node* node = hashUser[key].head;

    while (node)
    {
        if (strcmp(users[node->value].name, mUser) == 0) {
            return node->value;
        }
        node = node->next;
    }

    return -1;
}

void pushByID(int idx, int mID) {
    int key = mID % MAXTABLE;

    hashMessage[key].push(idx);
}

int findByID(int mID) {
    int key = mID % MAXTABLE;

    Node* node = hashMessage[key].head;

    while (node)
    {
        if (messages[node->value].id == mID) {
            return node->value;
        }
        
        node = node->next;
    }

    return -1;
}


int upward(int idx, int mPoint, bool isErase) {
    
    while (messages[idx].parent > -1) {

        if (isErase) {
            messages[idx].point -= mPoint;
        }
        else {
            messages[idx].point += mPoint;
        }
        
        

        idx = messages[idx].parent;

    }

    if (isErase) {
        messages[idx].point -= mPoint;
    }
    else {
        messages[idx].point += mPoint;
    }

    return idx;
}

void downward(int idx) {

    Node* node = messages[idx].children.head;

    while (node) {

        users[messages[node->value].userIdx].point -= messages[node->value].initPoint;
        messages[node->value].point = messages[node->value].initPoint = 0;
        queueUser.push(users[messages[node->value].userIdx].point, messages[node->value].userIdx, 0);

        downward(node->value);

        node = node->next;
    }

}


void init()
{
    queueUser.init();
    queueMessage.init();

    nVisitedUser = 0;
    nVisitedMSG = 0;

    mIdx = 0; uIdx = 0;

    for (int i = 0; i < MAXTABLE; i++) {
        hashUser[i].init();
        hashMessage[i].init();
    }

    return;

}



int writeMessage(char mUser[], int mID, int mPoint)
{
    int userIdx = findByName(mUser);

    if (userIdx == -1) {
        userIdx = uIdx++;

        users[userIdx].point = mPoint;
        strcpy(users[userIdx].name, mUser);

        pushByName(userIdx, mUser);
        visitedUser[userIdx] = 0;
    }

    else {
        users[userIdx].point += mPoint;
    }

    messages[mIdx].id = mID;
    messages[mIdx].initPoint = messages[mIdx].point = mPoint;
    messages[mIdx].parent = -1;
    messages[mIdx].children.init();

    messages[mIdx].userIdx = userIdx;


    pushByID(mIdx, mID);
    visitedMSG[mIdx] = 0;

    queueUser.push(users[userIdx].point, userIdx, 0);

    queueMessage.push(messages[mIdx].point, 0, mID);

    mIdx++;

    return users[userIdx].point;
}




int commentTo(char mUser[], int mID, int mTargetID, int mPoint)
{
    int userIdx = findByName(mUser);

    if (userIdx == -1) {
        userIdx = uIdx++;

        users[userIdx].point = mPoint;
        strcpy(users[userIdx].name, mUser);

        pushByName(userIdx, mUser);
        visitedUser[userIdx] = 0;
    }
    else {
        users[userIdx].point += mPoint;
    }

    queueUser.push(users[userIdx].point, userIdx, 0);

    int parent = findByID(mTargetID);

    messages[parent].children.push(mIdx);

    messages[mIdx].id = mID;
    messages[mIdx].initPoint = mPoint;
    messages[mIdx].point = 0;
    messages[mIdx].parent = parent;
    messages[mIdx].children.init();

    messages[mIdx].userIdx = userIdx;

    pushByID(mIdx, mID);

    

    int ans = upward(mIdx, mPoint, false);

    queueMessage.push(messages[ans].point, 0, messages[ans].id);

    mIdx++;

    
    return messages[ans].point;
}


int erase(int mID)
{

    int idx = findByID(mID);

    int root = upward(idx, messages[idx].point, true);

    users[messages[idx].userIdx].point -= messages[idx].initPoint;
    messages[idx].point = messages[idx].initPoint = 0;

    queueUser.push(users[messages[idx].userIdx].point, messages[idx].userIdx, 0);
    

    downward(idx);

    if (messages[idx].parent == -1) {
        queueMessage.push(messages[idx].point, 0, messages[idx].id);
        return users[messages[idx].userIdx].point;
    }

    queueMessage.push(messages[root].point, 0, messages[root].id);
    return messages[root].point;
}



void getBestMessages(int mBestMessageList[])
{
    nVisitedMSG++;

    HeapNode temp[5];
    int tIdx = 0;

    while (queueMessage.length > 0 && tIdx < 5) {
        HeapNode curr = queueMessage.pop();

        int idx = findByID(curr.id);
        if (curr.point == 0) { continue; }
        if (curr.point != messages[idx].point) { continue; }
        if (visitedMSG[idx] >= nVisitedMSG) { continue; }

        temp[tIdx++] = curr;
        visitedMSG[idx] = nVisitedMSG;
    }

    for (int i = 0; i < tIdx; i++) {
        queueMessage.push(temp[i].point, 0, temp[i].id);
        mBestMessageList[i] = temp[i].id;
    }

    return;
}



void getBestUsers(char mBestUserList[][MAXL + 1])
{
    nVisitedUser++;

    HeapNode temp[5];
    int tIdx = 0;

    while (queueUser.length > 0 && tIdx < 5) {
        HeapNode curr = queueUser.pop();

        if (visitedUser[curr.userIdx] >= nVisitedUser) { continue; }
        if (users[curr.userIdx].point != curr.point) { continue; }

        temp[tIdx++] = curr;
        visitedUser[curr.userIdx] = nVisitedUser;
    }

    for (int i = 0; i < tIdx; i++) {
        queueUser.push(temp[i].point, temp[i].userIdx, 0);
        strcpy(mBestUserList[i], users[temp[i].userIdx].name);
    }

    return;
}
