#include <string>

#define MAXARR 1001

using namespace std;

class Node {
public:
    int idx;
    Node* next;

    Node() {
        idx = -1;
        next = nullptr;
    }
};

class LinkedList {
public:
    Node* head;
    Node* tail;
    int length;

    LinkedList() { 
        head = nullptr;
        tail = nullptr;
        length = 0; 
    }

    void push(int a) {
        Node* node = new Node();
        node->idx = a;
        node->next = head;

        if (!tail) { tail = node; }

        head = node;
        length++;
    }
};

class Equation {
public:
    string eqn;
    int result;
    bool isExist;

    Equation() {
        eqn = "";
        result = 0;
        isExist = false;
    }

    void init(char mLink, char mSubexp[]) {
        eqn = mLink;
        eqn += mSubexp;
        isExist = true;
        result = calculate(0, eqn.size());
    }

    int calculate(int st, int end) {
        int ans = 0;
        int arr[100];
        int idx = -1;
        arr[0] = 0;
        int op = 0;
        if (eqn[st] == '+') { op = 0; }
        if (eqn[st] == '-') { op = 1; }
        if (eqn[st] == '*') { op = 2; }

        string s = "";
        for (int i = st; i < end; i++) {
            if (eqn[i] != '+' && eqn[i] != '-' && eqn[i] != '*') {
                s += eqn[i];
            }

            if ((i > 0) && (eqn[i] == '+' || eqn[i] == '-' || eqn[i] == '*' || i == end - 1)) {
                int v = stoi(s);
                if (op == 0) {
                    arr[++idx] = v;
                }
                else if (op == 1) {
                    arr[++idx] = -v;
                }
                else if (op == 2) {
                    arr[idx] *= v;
                }

                if (eqn[i] == '+') { op = 0; }
                if (eqn[i] == '-') { op = 1; }
                if (eqn[i] == '*') { op = 2; }

                s.clear();
            }
        }

        for (int i = 0; i <= idx; i++) {
            ans += arr[i];
        }

        return ans;
    }

    void update() {
        result = calculate(0, eqn.size());
    }
};

int eIdx, eqnLen, sumEqn;
Equation equations[MAXARR];
LinkedList group;

void init(int mLen, char mSubexp[])
{
    eqnLen = mLen + 1;
    group = LinkedList();

    eIdx = 0;
    equations[eIdx].init('+', mSubexp);
    sumEqn = equations[eIdx].result;

    eIdx++;
}

int append(char mLink, char mSubexp[])
{  
    equations[eIdx].init(mLink, mSubexp);
    sumEqn += equations[eIdx].result;

    eIdx++;
    return sumEqn;
}

int erase(int mFrom)
{
    int idx = 0;
    while (mFrom > eqnLen || !equations[idx].isExist) {
        if (equations[idx].isExist) { mFrom -= eqnLen; }
        idx++;
    }

    int idx2 = idx + 1;
    while (!equations[idx2].isExist) { idx2++; }

    sumEqn -= equations[idx].result;
    sumEqn -= equations[idx2].result;

    for (int i = mFrom; i < eqnLen; i++) {
        equations[idx].eqn[i] = equations[idx2].eqn[i];
    }

    equations[idx].result = equations[idx].calculate(0, eqnLen);
    equations[idx2].isExist = false;

    sumEqn += equations[idx].result;

    return sumEqn;
}

int select(int mFrom, int mTo)
{
    int ans = 0;
    int k = mTo - mFrom + 1;

    int idx = 0;
    while (mFrom > eqnLen || !equations[idx].isExist) {
        if (equations[idx].isExist) { mFrom -= eqnLen; }
        idx++;
    }

    if (mFrom + k < eqnLen) {
        return equations[idx].calculate(mFrom, mFrom + k);
    }
    else { ans += equations[idx].calculate(mFrom, eqnLen); }
    idx++;
    k -= eqnLen - mFrom;

    while (k > eqnLen || !equations[idx].isExist) {
        if (equations[idx].isExist) {
            ans += equations[idx].result;
            k -= eqnLen;
        }
        idx++;
    }

    ans += equations[idx].calculate(0, k);
    
    return ans;
}
