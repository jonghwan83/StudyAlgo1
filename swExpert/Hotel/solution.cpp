#include <map>
#include <vector>
#include <iostream>

using namespace std;

map<vector<int>, vector<vector<int> > > price;
map<int, vector<int> > priceIdx;
map<int, vector<int> > reserved;

void init(int N, int mRoomCnt[])
{
	price.clear();
	priceIdx.clear();
	reserved.clear();
}

void addRoom(int mHotelID, int mRoomID, int mRoomInfo[])
{
	vector<int> keyVec(mRoomInfo, mRoomInfo+4);
	int price = mRoomInfo[4];

	int temp[] = { price, mRoomID, mHotelID };
	vector<int> valueVec(temp, temp+3);

	price[keyVec] = valueVec;
	return;
}

int findRoom(int mFilter[])
{
	return 0;
}

int riseCosts(int mHotelID)
{
	return 0;
}
