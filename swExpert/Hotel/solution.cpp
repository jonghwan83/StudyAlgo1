#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;

map<vector<int>, vector<vector<int> > > hotelPrice;
map<int, set<vector<int> > > priceIdx;
map<vector<int>, set<vector<int> > > reserved;

void init(int N, int mRoomCnt[])
{
	hotelPrice.clear();
	priceIdx.clear();
	reserved.clear();
}

void addRoom(int mHotelID, int mRoomID, int mRoomInfo[])
{
	vector<int> keyVec(mRoomInfo, mRoomInfo+4);
	int price = mRoomInfo[4];

	int temp[] = { -price, -mRoomID, mHotelID };
	vector<int> valueVec(temp, temp+3);

	hotelPrice[keyVec].push_back(valueVec);
	push_heap(hotelPrice[keyVec].begin(), hotelPrice[keyVec].end());

	priceIdx[mHotelID].insert(keyVec);

	return;
}

int findRoom(int mFilter[])
{
	int count;
	int ans = -1;

	int checkIn = mFilter[0];
	int checkOut = mFilter[1];
	vector<int> priceKey(mFilter + 2, mFilter + 6);
	
	bool isComplete = false;

	vector<vector<int> > tempVec;
	while (hotelPrice[priceKey].size() > 0)
	{
		if (isComplete)
		{
			break;
		}
		int price = -hotelPrice[priceKey].front()[0];
		int roomID = -hotelPrice[priceKey].front()[1];
		int hotelID = hotelPrice[priceKey].front()[2];

		int pushed[] = { -price, -roomID, hotelID };
		vector<int> pushedVec(pushed, pushed + 3);
		tempVec.push_back(pushedVec);

		pop_heap(hotelPrice[priceKey].begin(), hotelPrice[priceKey].end());
		hotelPrice[priceKey].pop_back();

		int tempKey[] = { hotelID, roomID };
		vector<int> reservedKey(tempKey, tempKey + 2);

		count = 0;
		set<vector<int> >::iterator itr;
		for (itr = reserved[reservedKey].begin(); itr != reserved[reservedKey].end(); itr++)
		{
			vector<int> myVec = *itr;
			int reservedIn = myVec[0];
			int reservedOut = myVec[1];

			if ((reservedIn <= checkIn) && (checkIn < reservedOut)) { break; }
			else if ((reservedIn < checkOut) && (checkOut <= reservedOut)) { break; }
			else if ((checkIn < reservedIn) && (checkOut > reservedOut)) { break; }
			else { count++; }
		}
		if (count == reserved[reservedKey].size())
		{
			int checkInOut[] = { checkIn, checkOut };
			vector<int> checkInOutVec(checkInOut, checkInOut + 2);
			reserved[reservedKey].insert(checkInOutVec);
			ans = roomID;
			isComplete = true;
			break;
		}
	}

	while (tempVec.size() > 0)
	{
		hotelPrice[priceKey].push_back(tempVec.back());
		push_heap(hotelPrice[priceKey].begin(), hotelPrice[priceKey].end());
		tempVec.pop_back();
	}

	return ans;
}

int riseCosts(int mHotelID)
{
	vector<int> keyVec;
	int ans = 0;
	bool isChanged = false;

	set<vector<int> >::iterator itr;
	for (itr = priceIdx[mHotelID].begin(); itr != priceIdx[mHotelID].end(); itr++)
	{
		keyVec = *itr;
		for (int j=0; j < hotelPrice[keyVec].size(); j++)
		{
			if (hotelPrice[keyVec][j][2] == mHotelID)
			{
				hotelPrice[keyVec][j][0] *= 1.1;
				ans += hotelPrice[keyVec][j][0];
				isChanged = true;
			}
		}

		if (isChanged)
		{
			make_heap(hotelPrice[keyVec].begin(), hotelPrice[keyVec].end());
			isChanged = false;
		}
	}

	return -ans;
}
