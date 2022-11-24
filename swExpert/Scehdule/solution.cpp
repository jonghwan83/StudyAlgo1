#include <vector>
#include <queue>
#include <string>
#include <cstring>
#include <algorithm>
#include <unordered_map>

#define MAXDAY (1000000)
#define SUBSIZE (1000)
#define MAXSCHEDULE (30001)
#define MAXL (11)

using namespace std;

bool checkOverlapping(int st1, int ed1, int st2, int ed2) { return (min(ed1, ed2) - max(st1, st2) + 1) > 0; }

struct RESULT {
char mTitle[MAXL];
int mStartDay;
int mEndDay;
};

class Schedule {
public:
    int startDay;
    int endDay;
    bool isErased;
    string title;
};

class SubCalendar {
public:
    int start;
    int end;
    vector<int> sIndices;

    SubCalendar() {};
};

int totalDays, sIdx;
SubCalendar calendar[SUBSIZE];
Schedule schedule[MAXSCHEDULE];
unordered_map<string, int> hashSchedule;

bool checkOverlap(int mStartDay, int mEndDay) {
    int subStart = mStartDay / SUBSIZE;
    int subEnd = mEndDay / SUBSIZE;

    for (int i = subStart; i <= subEnd; i++) {
        for (int idx: calendar[i].sIndices) {
                if (!schedule[idx].isErased) {
                    if (checkOverlapping(mStartDay, mEndDay, schedule[idx].startDay, schedule[idx].endDay)) { return true; }
                }
        }
    }
    return false;
}

void writeSchedule(char mTitle[], int mStartDay, int mEndDay, int mForced) {
    
    int subStart = mStartDay / SUBSIZE;
    int subEnd = mEndDay / SUBSIZE;

    for (int i = subStart; i <= subEnd; i++) {
        if (mForced) {
            for (int idx : calendar[i].sIndices) {
                if (checkOverlapping(mStartDay, mEndDay, schedule[idx].startDay, schedule[idx].endDay) && !schedule[idx].isErased) {
                    schedule[idx].isErased = true;
                }
            }
        }

        if (mStartDay <= calendar[i].start && mEndDay >= calendar[i].end) {
            calendar[i].sIndices.clear();
        }

        calendar[i].sIndices.push_back(sIdx);
    }

    schedule[sIdx].startDay = mStartDay;
    schedule[sIdx].endDay = mEndDay;
    schedule[sIdx].title = mTitle;
    schedule[sIdx].isErased = false;

    hashSchedule[mTitle] = sIdx;
    sIdx++;
}

int searchSchedule(int mDay) {
    int subDay = mDay / SUBSIZE;
    for (int idx: calendar[subDay].sIndices) {
        if (!schedule[idx].isErased) {
            if (schedule[idx].startDay <= mDay && mDay <= schedule[idx].endDay) {
                return idx;
            }
        }
    }
    return 0;
}

void init(int N) {
    hashSchedule.clear();
    totalDays = N;
    sIdx = 1;
    for (int i = 0; i <= totalDays/ SUBSIZE; i++) {
        calendar[i].sIndices.clear();
        calendar[i].start = i * SUBSIZE;
        calendar[i].end = (i + 1) * SUBSIZE;
    }
}

int addSchedule(char mTitle[], int mStartDay, int mEndDay, int mForced) {
    if (mForced) {
        writeSchedule(mTitle, mStartDay, mEndDay, mForced);
        return 1;
    }
    else {
        if (!checkOverlap(mStartDay, mEndDay)) {
            writeSchedule(mTitle, mStartDay, mEndDay, mForced);
            return 1;
        }
    }

    return 0;
}

RESULT getSchedule(int mDay) {
    RESULT ret;
    ret.mTitle[0] = '\0';


    int idx = searchSchedule(mDay);

    if (!schedule[idx].isErased && idx > 0) {
        strcpy(ret.mTitle, schedule[idx].title.c_str());
        ret.mStartDay = schedule[idx].startDay;
        ret.mEndDay = schedule[idx].endDay;
    }

    return ret;
}

int deleteSchedule(char mTitle[]) {
    int idx = hashSchedule[mTitle];
    if (idx == 0) { return 0; }

    if (schedule[idx].isErased) { return 0; }
    else { 
        schedule[idx].isErased = true;
        return 1;
    }
}

int findEmptySchedule() {
    priority_queue< pair<int, int> > pq;
    
    pq.push(make_pair(0, 0));
    pq.push(make_pair(totalDays + 1, totalDays + 1));

    for (int i = 1; i < sIdx; i++) {
        if (!schedule[i].isErased) {
            pq.push(make_pair(schedule[i].startDay, schedule[i].endDay));
        }
    }

    int ans = 0;
    while (pq.size() > 1) {
        pair<int, int> p = pq.top();
        pq.pop();

        int dist = p.first - pq.top().second - 1;
        if (dist > ans) {
            ans = dist;
        }
    }

    return ans;
}

