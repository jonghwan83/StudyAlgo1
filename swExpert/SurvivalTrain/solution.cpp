#include <queue>
#include <vector>

#define MAXPASSENGERS 100000
#define MAXCAR 10
#define MAXJOB 1000

using namespace std;

class Passengers {
public:
    int point;
    int job;
    int car;
    int version;
};

int totalPassenger, totalCar;
Passengers passengers[MAXPASSENGERS];
priority_queue< vector<int> > maxQueue[MAXCAR];
priority_queue< vector<int> > minQueue[MAXCAR];
vector< vector<int> > hashJob(MAXJOB);

void init(int N, int M, int J, int mPoint[], int mJobID[])
{
    for (int i = 0; i < J; i++) {
        hashJob[i].clear();
    }

    totalPassenger = N;
    totalCar = N / M;

    for (int i = 0; i < totalCar; i++) {
        maxQueue[i] = priority_queue< vector<int> >();
        minQueue[i] = priority_queue< vector<int> >();
    }

    for (int i = 0; i < N; i++) {
        passengers[i].point = mPoint[i];
        passengers[i].job = mJobID[i];
        passengers[i].car = i / M;
        passengers[i].version = 0;
        hashJob[mJobID[i]].push_back(i);

        maxQueue[i / M].push( { mPoint[i], -i, 0 });
        minQueue[i / M].push({ -mPoint[i], i, 0 });
    }
}

void destroy()
{
}

int update(int mID, int mPoint)
{
    passengers[mID].point += mPoint;
    passengers[mID].version++;

    int car = passengers[mID].car;
    maxQueue[car].push({ passengers[mID].point, -mID, passengers[mID].version });
    minQueue[car].push({ -passengers[mID].point, mID, passengers[mID].version });

    return passengers[mID].point;
}

int updateByJob(int mJobID, int mPoint)
{
    int ans = 0;
    for (int idx : hashJob[mJobID]) {
        passengers[idx].point += mPoint;
        ans += passengers[idx].point;

        passengers[idx].version++;
        int car = passengers[idx].car;
        maxQueue[car].push({ passengers[idx].point, -idx, passengers[idx].version });
        minQueue[car].push({ -passengers[idx].point, idx, passengers[idx].version });
    }
    return ans;
}

int move(int mNum)
{
    int ans = 0;
    vector< vector< vector<int> > > maxTemp(totalCar);
    vector< vector< vector<int> > > minTemp(totalCar);

    for (int car = 0; car < totalCar; car++) {
        if (car > 0) {
            int k = 0;
            vector<int> curr;
            while (k < mNum) {
                curr = maxQueue[car].top();
                maxQueue[car].pop();
                int mID = -curr[1];
                int version = curr[2];

                if (passengers[mID].version == version && passengers[mID].car == car) {
                    ans += passengers[mID].point;
                    passengers[mID].car = car - 1;
                    passengers[mID].version++;

                    maxTemp[car - 1].push_back({ passengers[mID].point, -mID, passengers[mID].version });
                    minTemp[car - 1].push_back({ -passengers[mID].point, mID, passengers[mID].version });

                    k++;
                }
            }
        }
        if (car < totalCar - 1) {
            int k = 0;
            vector<int> curr;
            while (k < mNum) {
                curr = minQueue[car].top();
                minQueue[car].pop();
                int mID = curr[1];
                int version = curr[2];

                if (passengers[mID].version == version && passengers[mID].car == car) {
                    ans += passengers[mID].point;
                    passengers[mID].car = car + 1;
                    passengers[mID].version++;

                    maxTemp[car + 1].push_back({ passengers[mID].point, -mID, passengers[mID].version });
                    minTemp[car + 1].push_back({ -passengers[mID].point, mID, passengers[mID].version });

                    k++;
                }
            }
        }
    }

    for (int car = 0; car < totalCar; car++) {
        for (auto data : maxTemp[car]) {
            maxQueue[car].push(data);
        }
        for (auto data : minTemp[car]) {
            minQueue[car].push(data);
        }
    }

    return ans;
}