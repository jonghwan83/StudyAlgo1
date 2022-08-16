#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <string>

using namespace std;

unordered_map<string, vector<vector<int> > > hotelPrice;
unordered_map<int, unordered_set<string> > priceIdx;
unordered_map<string, set<vector<int> > > reserved;

void init(int N, int mRoomCnt[])
{
	hotelPrice.clear();
	priceIdx.clear();
	reserved.clear();
}

void addRoom(int mHotelID, int mRoomID, int mRoomInfo[])
{
	string keyStr;
	for (int i = 0; i < 4; i++)
	{
		keyStr += to_string(mRoomInfo[i]);
	}

	int price = mRoomInfo[4];

	int temp[] = { -price, -mRoomID, mHotelID };
	vector<int> valueVec(temp, temp+3);

	hotelPrice[keyStr].push_back(valueVec);
	push_heap(hotelPrice[keyStr].begin(), hotelPrice[keyStr].end());

	priceIdx[mHotelID].insert(keyStr);

	return;
}

int findRoom(int mFilter[])
{
	int count;
	int ans = -1;

	int checkIn = mFilter[0];
	int checkOut = mFilter[1];
	string priceKey;
	for (int i = 2; i < 6; i++)
	{
		priceKey += to_string(mFilter[i]);
	}
	
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

		string reservedKey;
		reservedKey += to_string(hotelID);
		reservedKey += to_string(roomID);

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
	string keyStr;
	int ans = 0;
	bool isChanged = false;

	unordered_set<string>::iterator itr;
	for (itr = priceIdx[mHotelID].begin(); itr != priceIdx[mHotelID].end(); itr++)
	{
		keyStr = *itr;
		for (int j=0; j < hotelPrice[keyStr].size(); j++)
		{
			if (hotelPrice[keyStr][j][2] == mHotelID)
			{
				hotelPrice[keyStr][j][0] *= 1.1;
				ans += hotelPrice[keyStr][j][0];
				isChanged = true;
			}
		}

		if (isChanged)
		{
			make_heap(hotelPrice[keyStr].begin(), hotelPrice[keyStr].end());
			isChanged = false;
		}
	}

	return -ans;
}
