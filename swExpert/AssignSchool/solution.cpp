#include <unordered_map>

using namespace std;

#define MAXSTUDENT 7500
#define MAXSCHOOL 10

class HeapNode {
public:
    int distance;
    int id;
};

class Heap {
public:
    HeapNode arr[MAXSTUDENT];
    int length;
    bool isMaxDist;
    bool isMaxID;

    void init(bool isMaxDistPriority, bool isMaxIDPriority) {
        length = 0;
        isMaxDist = isMaxDistPriority;
        isMaxID = isMaxIDPriority;
    }
    
    bool compare(int parent, int child) {
        if (isMaxDist) {
            if (arr[parent].distance < arr[child].distance) { return true; }
        }
        else {
            if (arr[parent].distance > arr[child].distance) { return true; }
        }
        if (isMaxID) {
            if (arr[parent].distance == arr[child].distance && arr[parent].id < arr[child].id) { return true; }
        }
        else {
            if (arr[parent].distance == arr[child].distance && arr[parent].id > arr[child].id) { return true; }
        }
        return false;
    }

    void push(int dist, int mID) {
        HeapNode last; last.distance = dist; last.id = mID;
        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2 , idx)) {
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

int abs(int a) {
    if (a < 0) { return -a; }
    return a;
}

class School {
public:
    int nStudent;
    int maxCapa;
    int x;
    int y;
    Heap assigned;  // dist: min priority, id: max priority

    void init() {
        nStudent = 0;
        maxCapa = 0;
        x = 0; y = 0;
        assigned.init(false, true);
    }
};

class Student {
public:
    int id;
    int x;
    int y;
    int school;
    int maxDistance;
    int minDistance;
    bool isRemoved;
    int closest;

    void init() {
        isRemoved = false;
        school = -1;
        closest = -1;
        maxDistance = 0;
        minDistance = 100000000;
    }
};

Heap closest;   // dist: min priority, id: min priority
Heap waiting;   // dist: max priority, id: min priority
HeapNode temp[MAXSTUDENT];
int totalSchool;
int studentIdx;
unordered_map<int, int> hashStudent;
int visited[MAXSCHOOL];
int nDfs;
School schools[MAXSCHOOL];
Student students[MAXSTUDENT];

int getDistance(int mStudent, int schoolID) {
    int sIdx = hashStudent[mStudent];
    int dist = abs(schools[schoolID].x - students[sIdx].x) + abs(schools[schoolID].y - students[sIdx].y);;
    return dist;
}

void init(int C, int N, int mX[], int mY[]) {
    hashStudent.clear();
    waiting.init(true, false);
    totalSchool = N;
    studentIdx = 0;

    for (int i = 0; i < N; i++) {
        schools[i].init();
        schools[i].maxCapa = C;
        schools[i].x = mX[i];
        schools[i].y = mY[i];
        visited[i] = 0;
    }
    nDfs = 1;

    return;
}

void push_student(int mStudent) {
    int idx = hashStudent[mStudent];
    closest.init(false, false);

    for (int i = 0; i < totalSchool; i++) {
        if (visited[i] < nDfs) {
            int dist = getDistance(mStudent, i);
            if (students[idx].maxDistance < dist) {
                students[idx].maxDistance = dist;
            }
            if (students[idx].minDistance > dist) {
                students[idx].minDistance  = dist;
                students[idx].closest = i;
            } 
            closest.push(dist, i);
        }
    }

    int schoolID = closest.arr[0].id;
    visited[schoolID] = nDfs;

    schools[schoolID].assigned.push(students[idx].maxDistance, mStudent);
    schools[schoolID].nStudent++;
    students[idx].school = schoolID;

    if (schools[schoolID].nStudent > schools[schoolID].maxCapa) {
        HeapNode newStudent = schools[schoolID].assigned.pop();
        idx = hashStudent[newStudent.id];
        while (students[idx].isRemoved || students[idx].school != schoolID) {
            newStudent = schools[schoolID].assigned.pop();
            idx = hashStudent[newStudent.id];
        }
        schools[schoolID].nStudent--;
        
        if (students[idx].closest == schoolID) {
            waiting.push(students[idx].maxDistance, newStudent.id);
        }
        
        push_student(newStudent.id);
    }
}

int add(int mStudent, int mX, int mY) {
    students[studentIdx].init();
    hashStudent[mStudent] = studentIdx;
    students[studentIdx].x = mX;
    students[studentIdx].y = mY;
    students[studentIdx].id = mStudent;

    push_student(mStudent);
    nDfs++;

    if (students[studentIdx].school != students[studentIdx].closest) {
        waiting.push(students[studentIdx].maxDistance, mStudent);
    }
    
    studentIdx++;
    return students[studentIdx - 1].school;
}

void pop_student(int schoolID) {
    int tIdx = 0;

    HeapNode studentInfo;
    int sIdx;
    bool addNewStudent = false;

    while (waiting.length > 0) {
        studentInfo = waiting.pop();
        sIdx = hashStudent[studentInfo.id];

        if (students[sIdx].isRemoved) { continue; }
        if (students[sIdx].school == students[sIdx].closest) { continue; }
        temp[tIdx++] = studentInfo;
        if (students[sIdx].school == schoolID) { continue; }

        int newDist = getDistance(studentInfo.id, schoolID);
        int oldDist = getDistance(studentInfo.id, students[sIdx].school);
        if (newDist < oldDist) {
            addNewStudent = true;
            break;
        }       
        else if (newDist == oldDist && schoolID < students[sIdx].school) {
            addNewStudent = true;
            break;
        }
    }

    for (int i = 0; i < tIdx; i++) { waiting.push(temp[i].distance, temp[i].id); }

    if (addNewStudent) {
        schools[schoolID].nStudent++;
        int oldSchool = students[sIdx].school;
        schools[schoolID].assigned.push(students[sIdx].maxDistance, students[sIdx].id);
        students[sIdx].school = schoolID;

        schools[oldSchool].nStudent--;
        pop_student(oldSchool);
    }
}

int remove(int mStudent) {
    int idx = hashStudent[mStudent];
    students[idx].isRemoved = true;
    schools[students[idx].school].nStudent--;

    pop_student(students[idx].school);

    return students[idx].school;
}

int status(int mSchool) {

    return schools[mSchool].nStudent;
}
