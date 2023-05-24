#include <unordered_map>

using namespace std;

#define MAXEMPLOYEE 8000

class Employee {
public:
    int id;
    int start;
    int end;
    bool isRemoved;
};


class HeapNode {
public:
    int first;
    int second;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXEMPLOYEE];

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (arr[parent].first > arr[child].first) { return true; }
        if (arr[parent].first == arr[child].first && arr[parent].second > arr[child].second) { return true; }
        
        return false;
    }

    void push (int f, int s) {
        HeapNode last;
        last.first = f; last.second = s;

        int idx = length;
        arr[length++] = last;

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

        while (idx * 2 + 1 < length) {
            left = idx * 2 + 1;
            right = idx * 2 + 2;

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

int eIdx;
int total;
Employee employees[MAXEMPLOYEE];
unordered_map<int, int> hashID;


void init() {

    hashID.clear();
    eIdx = 0;
    total = 0;

    return;
}



int add(int mId, int mStart, int mEnd) {
    mEnd++;

    unordered_map<int, int>::iterator itr = hashID.find(mId);

    if (itr == hashID.end()) {
        hashID[mId] = eIdx;

        employees[eIdx].id = mId;
        employees[eIdx].start = mStart;
        employees[eIdx].end = mEnd;
        employees[eIdx].isRemoved = false;

        eIdx++; total++;
    }

    else {
        employees[itr->second].start = mStart;
        employees[itr->second].end = mEnd;

        if (employees[itr->second].isRemoved) {
            employees[itr->second].isRemoved = false;
            total++;
        }
    }


    return total;
}



int remove(int mId) {

    unordered_map<int, int>::iterator itr = hashID.find(mId);

    if (itr != hashID.end()) {
        if (!employees[itr->second].isRemoved) {
            employees[itr->second].isRemoved = true;
            total--;
        }
    }

    return total;
}


int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

int announce(int mDuration, int M) {
    Heap checkIn;
    Heap inOffice;

    checkIn.init();
    inOffice.init();

    for (int i = 0; i < eIdx; i++) {
        if (!employees[i].isRemoved) {
            checkIn.push(employees[i].start, i);
        }
    }

    int end;
    while (checkIn.length > 0) {
        HeapNode curr = checkIn.pop();

        inOffice.push(employees[curr.second].end, curr.second);

        end = curr.first + mDuration;

        while (inOffice.length > 0 && employees[inOffice.arr[0].second].end < end) {
            inOffice.pop();
        }

        if (inOffice.length >= M) {
            return curr.first;
        }
    }


    return -1;
}
