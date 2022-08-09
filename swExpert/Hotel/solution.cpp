#include <map>
#include <vector>
#include <iostream>

using namespace std;

map<vector<int>, vector<int> > price;
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
	vector<int> roomInfo;
	roomInfo.assign(begin(mRoomInfo), std::end(mRoomInfo));
}

int findRoom(int mFilter[])
{
	return 0;
}

int riseCosts(int mHotelID)
{
	return 0;
}
