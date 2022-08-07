#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

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

        passengersByJob[car].push_back(i);
        
        vector<int> maxTemp;
        maxTemp.push_back(mPoint[i]);
        maxTemp.push_back(i);
        maxTemp.push_back(0);

        maxHeap[car].push_back(maxTemp);
        push_heap(maxHeap[car].begin(), maxHeap[car].end());

        vector<int> minTemp;
        minTemp.push_back(-mPoint[i]);
        minTemp.push_back(-i);
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
    maxTemp.push_back(mID);
    maxTemp.push_back(passengers[mID][3]);

    maxHeap[car].push_back(maxTemp);
    push_heap(maxHeap[car].begin(), maxHeap[car].end());

    vector<int> minTemp;
    minTemp.push_back(-passengers[mID][0]);
    minTemp.push_back(-mID);
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
        update(mID, mPoint);
        ans += passengers[mID][0];
    }
    
    return ans;
}

int move(int mNum)
{
    int ans = 0;

    map<int, vector<vector<int> > > tempMin;
    map<int, vector<vector<int> > > tempMax;

    for (int c = 0; c < nCars; c++)
    {
        for (int i = 0; i < mNum; i++)
        {
            if (c < nCars - 1)
            {
                while (minHeap[c].size() > 0)
                {
                    int point = -minHeap[c][0][0];
                    int pID = -minHeap[c][0][1];
                    int ver = minHeap[c][0][2];
                    
                    if ((ver == passengers[pID][3]) && (passengers[pID][4]) && (passengers[pID][2] == c))
                    {
                        ans += point;
                        vector<int> minArr;
                        minArr.push_back(-point);
                        minArr.push_back(-pID);
                        minArr.push_back(ver);

                        tempMin[c].push_back(minArr);
                        passengers[pID][4] = 0;

                        pop_heap(minHeap[c].begin(), minHeap[c].end());
                        minHeap[c].pop_back();
                        break;
                    }
                    else
                    {
                        pop_heap(minHeap[c].begin(), minHeap[c].end());
                        minHeap[c].pop_back();
                    }
                }
            }
            
            if (c > 0)
            {
                while (maxHeap[c].size() > 0)
                {
                    int point = maxHeap[c][0][0];
                    int pID = maxHeap[c][0][1];
                    int ver = maxHeap[c][0][2];

                    if ((ver == passengers[pID][3]) && (passengers[pID][4]) && (passengers[pID][2] == c))
                    {
                        ans += point;
                        vector<int> maxArr;
                        maxArr.push_back(point);
                        maxArr.push_back(pID);
                        maxArr.push_back(ver);

                        tempMax[c].push_back(maxArr);
                        passengers[pID][4] = 0;

                        pop_heap(maxHeap[c].begin(), maxHeap[c].end());
                        maxHeap[c].pop_back();
                        break;
                    }
                    else
                    {
                        pop_heap(maxHeap[c].begin(), maxHeap[c].end());
                        maxHeap[c].pop_back();
                    }
                }
            }
        }
    }

    for (int c = 0; c < nCars; c++) 
    {
        if (c < nCars - 1)
        {
            while (tempMin[c].size() > 0)
            {
                int point = -tempMin[c][tempMin[c].size() - 1][0];
                int pID = -tempMin[c][tempMin[c].size() - 1][1];
                int ver = tempMin[c][tempMin[c].size() - 1][2];
                
                passengers[pID][2] = c + 1;

                vector<int> tempArr;
                tempArr.push_back(-point);
                tempArr.push_back(-pID);
                tempArr.push_back(ver);

                minHeap[c+1].push_back(tempArr);
                push_heap(minHeap[c].begin(), minHeap[c].end());

                tempArr[0] = point;
                tempArr[1] = pID;

                maxHeap[c+1].push_back(tempArr);
                push_heap(maxHeap[c].begin(), maxHeap[c].end());

                passengers[pID][4] = 1;

                tempMin[c].pop_back();
            }
        }

        if (c > 0)
        {
            while (tempMax[c].size() > 0)
            {
                int point = tempMax[c][tempMax[c].size() - 1][0];
                int pID = tempMax[c][tempMax[c].size() - 1][1];
                int ver = tempMax[c][tempMax[c].size() - 1][2];

                passengers[pID][2] = c - 1;

                vector<int> tempArr;
                tempArr.push_back(-point);
                tempArr.push_back(-pID);
                tempArr.push_back(ver);

                minHeap[c-1].push_back(tempArr);
                push_heap(minHeap[c].begin(), minHeap[c].end());

                tempArr[0] = point;
                tempArr[1] = pID;

                maxHeap[c-1].push_back(tempArr);
                push_heap(maxHeap[c].begin(), minHeap[c].end());

                passengers[pID][4] = 1;

                tempMax[c].pop_back();
            }
        }
    }

    // cout << ans << endl;
    return ans;
}