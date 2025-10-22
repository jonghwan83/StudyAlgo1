#include <vector>
#include <unordered_map>
#include <cmath>

#define MAX_N 10
#define MAX_CAR 50000

struct Car
{
    int date, company, period;
    bool isRemoved;

    Car() {}
    Car(int mDate, int mCompany, int mPeriod)
    {
        date = mDate; company = mCompany; period = mPeriod;
        isRemoved = false;
    }
};

struct Company
{
    int occupied, maxCapa;
    std::vector<int> arr;

    Company() {}
    Company(int mCapacity)
    {
        maxCapa = mCapacity; occupied = 0;
        arr.clear();
    }
};

int towed;
int c;
std::vector<Company> companies(MAX_N);
std::unordered_map<int, int> hashID;
std::vector<Car> parked(MAX_CAR);

void init(int N, int mCapacity[])
{
    towed = 10;

    hashID.clear();
    for(int i = 0; i < N; i++)
        companies[i] = Company(mCapacity[i]);

    c = 0;
    return;
}

int park(int mDate, int mID, int mCompany, int mPeriod)
{
    if(companies[mCompany].occupied >= companies[mCompany].maxCapa) return -1;

    parked[c] = Car(mDate, mCompany, mPeriod);

    hashID.insert(std::make_pair(mID, c));

    companies[mCompany].arr.push_back(c);
    companies[mCompany].occupied++;

    c++;
    return companies[mCompany].occupied;
}

int retrieve(int mDate, int mID)
{
    int idx = hashID.find(mID)->second;

    parked[idx].isRemoved = true;

    int cIdx = parked[idx].company;

    companies[cIdx].occupied = std::max(0, companies[cIdx].occupied - 1);

    int res = parked[idx].date + parked[idx].period - mDate;

    if(res <= 0) return std::abs(res) + 1;
    return res -1;
}

void buy(int mID, int mPeriod)
{
    int idx = hashID.find(mID)->second;

    if(parked[idx].isRemoved) return;
    if(parked[idx].company == towed) return;

    parked[idx].period += mPeriod;
    return;
}

void event(int mCompany, int mPeriod)
{
    for(int idx: companies[mCompany].arr)
    {
        if(parked[idx].isRemoved) continue;
        if(parked[idx].company == towed) continue;

        parked[idx].period += mPeriod;
    }

    return;
}

int inspect(int mDate, int mCompany)
{
    int cnt = 0;

    for(int idx: companies[mCompany].arr)
    {
        if(parked[idx].isRemoved) continue;
        if(parked[idx].date + parked[idx].period > mDate) continue;

        int cIdx = parked[idx].company;
        if(cIdx == towed) continue;

        companies[cIdx].occupied = std::max(0, companies[cIdx].occupied - 1);
        parked[idx].company = towed;

        cnt++;
    }
    return cnt;
}
