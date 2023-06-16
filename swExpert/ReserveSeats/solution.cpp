struct Result {
 int id;
 int num;
};


#include <vector>

using namespace std;

#define MAXSEAT 100
#define MAXTHEATER 2000
#define MAXRESERVE 50001

int visited[10][10];
int nVisited;

class HeapNode {
public:
    int seatID;
    int idx;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXSEAT];

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].seatID > arr[child].seatID) { return true; }
        return false;
    }

    void push(int s, int i) {
        HeapNode last;
        last.seatID = s; last.idx = i;

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

int drows[4] = { -1, 0, 0, 1 };
int dcols[4] = { 0, -1, 1, 0 };

class Reservations {
public:
    int id;
    int theater;
    vector<int> seatIDs;

    void init() { seatIDs.clear(); }
};

Reservations reservations[MAXRESERVE];


class Seat {
public:
    int row;
    int col;
};

Seat id2coor(int id) {
    Seat ans; id--;
    ans.row = id / 10;
    ans.col = id % 10;
    return ans;
}

int coor2id(int row, int col) {
    return row * 10 + col + 1;
}

class Vacancy {
public:
    bool isMerged;
    int minID;
    int nSeats;

    void init() {
        minID = 0;
        isMerged = false;
        nSeats = 0;
    }
};

class Theater {
public:
    Heap minIDs;
    Vacancy vacancies[MAXSEAT * 2];
    
    int rootVacancy[MAXSEAT * 2];
    int seatMap[10][10];
    int vIdx;
    int maxVacancy;

    void init() {
        for (int i = 0; i < MAXSEAT * 2; i++) {
            vacancies[i].init();
            rootVacancy[i] = i;
        }

        minIDs.init();
        minIDs.push(1, 1);

        vacancies[1].nSeats = 100;
        vacancies[1].minID = 1;

        vIdx = 2;

        maxVacancy = 100;

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                seatMap[i][j] = 1;
            }
        }
        
    }

    void updateMax() {
        maxVacancy = 0;
        for (int i = 1; i < vIdx; i++) {
            if (!vacancies[i].isMerged) {
                maxVacancy = max(maxVacancy, vacancies[i].nSeats);
            }
        }
    }

    int findVacancy(int x) {
        if (rootVacancy[x] == x) { return x; }
        else { return findVacancy(rootVacancy[x]); } 
    }

    void unionVacancy(int x, int y) {
        x = findVacancy(x);
        vacancies[x].isMerged = true;
        rootVacancy[x] = y;
    }

    void checkRemain(int cID, int idx, int newIdx) {
        
        Heap ids; ids.init();

        Heap queue; queue.init();
        queue.push(cID, idx);

        Seat c = id2coor(cID);
        visited[c.row][c.col] = nVisited;

        HeapNode curr;
        while (queue.length > 0) {
            curr = queue.pop();

            c = id2coor(curr.seatID);
            ids.push(curr.seatID, curr.idx);
            seatMap[c.row][c.col] = newIdx;

            for (int i = 0; i < 4; i++) {
                int dr = c.row + drows[i];
                int dc = c.col + dcols[i];

                if (dr < 0 || dc < 0 || dr >= 10 || dc >= 10) { continue; }
                if (findVacancy(seatMap[dr][dc]) != idx) { continue; }

                if (visited[dr][dc] < nVisited) {
                    visited[dr][dc] = nVisited;
                    queue.push(coor2id(dr, dc), idx);
                }
            }
        }

        vacancies[newIdx].init();
        vacancies[newIdx].nSeats = ids.length;
        vacancies[newIdx].minID = ids.arr[0].seatID;
        minIDs.push(vacancies[newIdx].minID, newIdx);
    }

    void reserve(int k, int rIdx) {
        int cnt = 0;

        HeapNode temp[MAXSEAT * 2];
        int tIdx = 0;

        HeapNode curr;

        while (minIDs.length > 0) {
            curr = minIDs.pop(); 
        
            if (vacancies[curr.idx].isMerged) { continue; }

            if (vacancies[curr.idx].nSeats < k) {
                temp[tIdx++] = curr;
                continue;
            }
            else { break; }
        }

        for (int i = 0; i < tIdx; i++) {
            minIDs.push(temp[i].seatID, temp[i].idx);
        }

        Heap pQueue; pQueue.init();
        Heap pQueue2; pQueue2.init();
        nVisited++;

        pQueue.push(curr.seatID, curr.idx);
        Seat c = id2coor(curr.seatID);
        visited[c.row][c.col] = nVisited;
        vacancies[curr.idx].nSeats--;

        while (pQueue.length > 0) {
            HeapNode node = pQueue.pop();

            c = id2coor(node.seatID);
            if (cnt < k) {
                seatMap[c.row][c.col] = 0; cnt++;
                vacancies[node.idx].nSeats--;
                reservations[rIdx].seatIDs.push_back(node.seatID);
            }
            else {
                pQueue2.push(node.seatID, node.idx);
            }

            for (int i = 0; i < 4; i++) {
                int dr = c.row + drows[i];
                int dc = c.col + dcols[i];

                if (dr < 0 || dc < 0 || dr >= 10 || dc >= 10) { continue; }
                if (findVacancy(seatMap[dr][dc]) != node.idx) { continue; }

                if (visited[dr][dc] < nVisited) {
                    visited[dr][dc] = nVisited;
                    pQueue.push(coor2id(dr, dc), node.idx);
                }
            }
        }

        int newIdx = curr.idx;
        nVisited++;
        bool idxIncr = false;
        while (pQueue2.length > 0) {
            HeapNode node = pQueue2.pop();

            c = id2coor(node.seatID);
            if (visited[c.row][c.col] >= nVisited) { continue; }


            if (idxIncr) {
                newIdx = vIdx++; idxIncr = false;
            }
            checkRemain(node.seatID, curr.idx, newIdx);
            idxIncr = true;
        }

        updateMax();

    }

};

int total;
Theater theaters[MAXTHEATER];

void init(int N)
{

    for (int i = 1; i <= N; i++) {
        theaters[i].init();
    }

    total = N;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            visited[i][j] = 0;
        }
    }

    nVisited = 0;
}

Result reserveSeats(int mID, int K)
{
    Result res;
    res.id = 0;
    res.num = 0;

    for (int i = 1; i <= total; i++) {
        if (theaters[i].maxVacancy < K) { continue; }
        res.id = i; break;
    }

    if (res.id == 0) { 
        return res; 
    }

    reservations[mID].init();
    reservations[mID].theater = res.id;

    theaters[res.id].reserve(K, mID);
    
    res.num = reservations[mID].seatIDs.front();


    return res;
}

Result cancelReservation(int mID)
{
    Result res;
    res.id = 0;
    res.num = 0;

    res.id = reservations[mID].theater;

    int vacIdx = theaters[res.id].vIdx;

    theaters[res.id].vacancies[vacIdx].init();
    theaters[res.id].vacancies[vacIdx].nSeats = reservations[mID].seatIDs.size();
    theaters[res.id].vacancies[vacIdx].minID = reservations[mID].seatIDs.front();

    for (int s : reservations[mID].seatIDs) {
        res.num += s;
        Seat c = id2coor(s);
        theaters[res.id].seatMap[c.row][c.col] = vacIdx;

        for (int i = 0; i < 4; i++) {
            int dr = c.row + drows[i];
            int dc = c.col + dcols[i];

            if (dr < 0 || dc < 0 || dr >= 10 || dc >= 10) { continue; }
            if (theaters[res.id].seatMap[dr][dc] == 0 || 
                theaters[res.id].findVacancy(theaters[res.id].seatMap[dr][dc]) == vacIdx) { continue; }
            
            int v = theaters[res.id].findVacancy(theaters[res.id].seatMap[dr][dc]);
            theaters[res.id].unionVacancy(v, vacIdx);
            theaters[res.id].vacancies[vacIdx].nSeats += theaters[res.id].vacancies[v].nSeats;
            theaters[res.id].vacancies[vacIdx].minID = min(theaters[res.id].vacancies[vacIdx].minID, theaters[res.id].vacancies[v].minID);
        }
    }

    theaters[res.id].minIDs.push(theaters[res.id].vacancies[vacIdx].minID, vacIdx);

    theaters[res.id].maxVacancy = max(theaters[res.id].maxVacancy, theaters[res.id].vacancies[vacIdx].nSeats);

    theaters[res.id].vIdx++;

    return res;
}
