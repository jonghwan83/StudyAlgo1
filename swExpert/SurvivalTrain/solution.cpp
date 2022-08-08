#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

using namespace std;

int nCars;
vector< vector<int> > passengers;  // index: passenger ID, value [point, jobID, car, updateVer, isInCar]
map<int, vector<int> > passengersByJob;  // key: Job ID, value: passenger ID

// heap structure
map<int, vector<vector<int> > > minHeap;  // key: car, value: [point, passenger ID, updateVer]
map<int, vector<vector<int> > > maxHeap;  // key: car, value: [point, passenger ID, updateVer]

void init(int N, int M, int J, int mPoint[], int mJobID[])
{
    passengers.clear();
    passengersByJob.clear();
    minHeap.clear();
    maxHeap.clear();
    
    nCars = N / M;

    for (int i = 0; i < N; i++)
    {
        int car = i / M;

        vector<int> temp;
        temp.push_back(mPoint[i]);
        temp.push_back(mJobID[i]);
        temp.push_back(car);
        temp.push_back(0);
        temp.push_back(1);

        passengers.push_back(temp);

        passengersByJob[mJobID[i]].push_back(i);
        
        vector<int> maxTemp;
        maxTemp.push_back(mPoint[i]);
        maxTemp.push_back(-i);
        maxTemp.push_back(0);

        maxHeap[car].push_back(maxTemp);
        push_heap(maxHeap[car].begin(), maxHeap[car].end());

        vector<int> minTemp;
        minTemp.push_back(-mPoint[i]);
        minTemp.push_back(i);
        minTemp.push_back(0);

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

    vector<int> maxTemp;
    maxTemp.push_back(passengers[mID][0]);
    maxTemp.push_back(-mID);
    maxTemp.push_back(passengers[mID][3]);

    maxHeap[car].push_back(maxTemp);
    push_heap(maxHeap[car].begin(), maxHeap[car].end());

    vector<int> minTemp;
    minTemp.push_back(-passengers[mID][0]);
    minTemp.push_back(mID);
    minTemp.push_back(passengers[mID][3]);

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
        ans += update(mID, mPoint);
    }
    
    return ans;
}

int move(int mNum)
{
    int ans = 0;
    map<int, vector<vector<int> > > tempMin;
    map<int, vector<vector<int> > > tempMax;

    vector<int> arrMin;
    vector<int> arrMax;

    for (int i = 0; i < 3; i++)
    {
        arrMin.push_back(0);
        arrMax.push_back(0);
    }

    for (int car = 0; car < nCars; car++)
    {
        for (int i = 0; i < mNum; i++)
        {
            if (car < nCars - 1)
            {
                while (true)
                {
                    pop_heap(minHeap[car].begin(), minHeap[car].end());

                    int point = -minHeap[car][minHeap[car].size()-1][0];
                    int pID = minHeap[car][minHeap[car].size()-1][1];
                    int ver = minHeap[car][minHeap[car].size()-1][2];

                    minHeap[car].pop_back();

                    if ((passengers[pID][3] == ver) && (passengers[pID][4]) && (passengers[pID][2] == car))
                    {
                        arrMin[0] = -point;
                        arrMin[1] = pID;
                        arrMin[2] = ver;

                        tempMin[car].push_back(arrMin);
                        passengers[pID][4] = 0;

                        ans += point;
                        break;
                    }
                }
            }

            if (car > 0)
            {
                while (true)
                {
                    pop_heap(maxHeap[car].begin(), maxHeap[car].end());

                    int point = maxHeap[car][maxHeap[car].size()-1][0];
                    int pID = -maxHeap[car][maxHeap[car].size()-1][1];
                    int ver = maxHeap[car][maxHeap[car].size()-1][2];

                    maxHeap[car].pop_back();
                    
                    if ((passengers[pID][3] == ver) && (passengers[pID][4]) && (passengers[pID][2] == car))
                    {
                        arrMax[0] = point;
                        arrMax[1] = -pID;
                        arrMax[2] = ver;

                        tempMax[car].push_back(arrMax);
                        passengers[pID][4] = 0;

                        ans += point;
                        break;
                    }
                }
            }
        }
    }

    for (int car = 0; car < nCars; car++)
    {
        if (car < nCars - 1)
        {
            while (tempMin[car].size() > 0)
            {
                int point = -tempMin[car][tempMin[car].size()-1][0];
                int pID = tempMin[car][tempMin[car].size()-1][1];
                int ver = tempMin[car][tempMin[car].size()-1][2];

                arrMin[0] = -point;
                arrMin[1] = pID;
                arrMin[2] = ver;

                arrMax[0] = point;
                arrMax[1] = -pID;
                arrMax[2] = ver;

                tempMin[car].pop_back();

                minHeap[car + 1].push_back(arrMin);
                push_heap(minHeap[car + 1].begin(), minHeap[car + 1].end());

                maxHeap[car + 1].push_back(arrMax);
                push_heap(maxHeap[car + 1].begin(), maxHeap[car + 1].end());

                passengers[pID][2] = car + 1;
                passengers[pID][4] = 1;
            }
        }


        if (car > 0)
        {
            while (tempMax[car].size() > 0)
            {
                int point = tempMax[car][tempMax[car].size()-1][0];
                int pID = -tempMax[car][tempMax[car].size()-1][1];
                int ver = tempMax[car][tempMax[car].size()-1][2];

                arrMin[0] = -point;
                arrMin[1] = pID;
                arrMin[2] = ver;

                arrMax[0] = point;
                arrMax[1] = -pID;
                arrMax[2] = ver;

                tempMax[car].pop_back();

                minHeap[car - 1].push_back(arrMin);
                push_heap(minHeap[car - 1].begin(), minHeap[car - 1].end());

                maxHeap[car - 1].push_back(arrMax);
                push_heap(maxHeap[car - 1].begin(), maxHeap[car - 1].end());

                passengers[pID][2] = car - 1;
                passengers[pID][4] = 1;
            }
        }
    }

    return ans;
}