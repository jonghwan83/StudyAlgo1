#include <vector>
#include <algorithm>
#include <map>

using namespace std;

int nCars;
vector< vector<int> > passengers;  // index: passenger ID, value [point, jobID, car, updateVer, isInCar]
map<int, vector<int> > passengersByJob;  // key: Job ID, value: passenger ID

// heap structure
map<int, vector<vector <int> > > minHeap;  // key: car, value: [point, passenger ID, updateVer]
map<int, vector<vector <int> > > maxHeap;  // key: car, value: [point, passenger ID, updateVer]

void init(int N, int M, int J, int mPoint[], int mJobID[])
{
    passengers.clear();
    passengersByJob.clear();
    
    nCars = N / M;

    for (int i = 0; i < N; i++)
    {
        int car = i / M;
        vector<int> temp = { mPoint[i], mJobID[i], car, 0, 1 };
        passengers.push_back(temp);

        passengersByJob[car].push_back(i);
        
        vector<int> maxTemp = { mPoint[i], i, 0 };
        maxHeap[car].push_back(maxTemp);
        push_heap(maxHeap[car].begin(), maxHeap[car].end());

        vector<int> minTemp = { -mPoint[i], -i, 0 };
        minHeap[car].push_back(minTemp);
        push_heap(minHeap[car].begin(), minHeap[car].end());
    }
    return;
}

void destroy()
{
}

int update(int mID, int mPoint)
{
    passengers[mID][0] += mPoint;
    passengers[mID][3]++;
    
    int car = passengers[mID][2];

    vector<int> maxTemp = { passengers[mID][0], mID, passengers[mID][3] };
    maxHeap[car].push_back(maxTemp);
    push_heap(maxHeap[car].begin(), maxHeap[car].end());

    vector<int> minTemp = { -passengers[mID][0], -mID, passengers[mID][3] };
    minHeap[car].push_back(minTemp);
    push_heap(minHeap[car].begin(), minHeap[car].end());
    
    return passengers[mID][0];
}

int updateByJob(int mJobID, int mPoint)
{
    int ans = 0;
    
    for (int i = 0; i < passengersByJob[mJobID].size(); i++)
    {
        int mID = passengersByJob[mJobID][i];
        update(mID, mPoint);
        ans += passengers[mID][0];
    }
    
    return ans;
}

int move(int mNum)
{
    int ans = 0;
    vector<vector<int> > tempMin;
    vector<vector<int> > tempMax;

    for (int c = 0; c < nCars; c++) {
        tempMin.clear();
        tempMax.clear();
        for (int i = 0; i < mNum; i++)
        {
            if (c < nCars - 1)
            {
                while (true)
                {
                    int point = minHeap[0][0];
                    int pID = minHeap[0][1];
                    int ver = minHeap[0][2];
                    if ((ver == passengers[pID][3]) && (passengers[pID][4]) && (passengers[pID][2] == c))
                    {
                        ans += point;
                        vector<int> arrMin = {-passengers[pID][0], -pID, passengers[pID][3]}
                        tempMin.push_back(arrMin);
                        passengers[pID][4] = 0;
                        break;
                    }
                }
            }
        }
    }
    return -3;
}