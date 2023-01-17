#include <cmath>
#include <unordered_map>

#define MAXFIND 1000
#define MAXROOM 100
#define MAXHOTEL 1001
#define MAXHEAP 600
#define MAXREGION 11
#define MAXBEDS 11
#define MAXTYPE 5
#define MAXVIEW 5

using namespace std;

class Data {
public:
	int price;
	int roomID;
	int hotelID;

	Data() {
		price = 0;
		roomID = 0;
		hotelID = 0;
	}
};

class Heap {
public:
	Data arr[MAXHEAP * MAXROOM];
	int length;

	void init() { length = 0; }

	bool compare(int parent, int child) {
		if (arr[parent].price > arr[child].price) { return true; }
		if (arr[parent].price == arr[child].price && arr[parent].roomID > arr[child].roomID) { return true; }
		return false;
	}

	void push(int price, int roomID, int hotelID) {
		Data last = Data();
		last.price = price; last.roomID = roomID; last.hotelID = hotelID;

		arr[length] = last;
		int idx = length;

		while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
			Data temp = arr[idx];
			arr[idx] = arr[(idx - 1) / 2];
			arr[(idx - 1) / 2] = temp;
			idx = (idx - 1) / 2;
		}
		length++;
	}

	Data pop() {
		Data ans = arr[0];
		length--;
		arr[0] = arr[length];

		int idx = 0;
		int left, right, child;
		while (2 * idx + 1 < length) {
			left = 2 * idx + 1;
			right = 2 * idx + 2;

			if (right < length) {
				if (compare(left, right)) { child = right; }
				else { child = left; }
			}
			else { child = left; }

			if (compare(idx, child)) {
				Data temp = arr[idx];
				arr[idx] = arr[child];
				arr[child] = temp;
				idx = child;
			}
			else { break; }
		}
		return ans;
	}
};

class Hotel {
public:
	int length;
	unordered_map<int, int> hashRoomID;
	int RoomID[MAXROOM];
	int price[MAXROOM];
	int region[MAXROOM];
	int beds[MAXROOM];
	int roomType[MAXROOM];
	int viewType[MAXROOM];
	int cIdx[MAXROOM];
	int checkIn[MAXROOM][MAXFIND];
	int checkOut[MAXROOM][MAXFIND];
};

Hotel hotels[MAXHOTEL];
Heap minPrice[MAXREGION][MAXBEDS][MAXTYPE][MAXVIEW];


bool checkOverlap(int a1, int a2, int b1, int b2) {
	int first = max(a1, b1);
	int last = min(a2, b2);
	return first <= last;
}

void init(int N, int mRoomCnt[]) {
	for (int i = 0; i < N + 1; i++) {
		hotels[i].length = 0;
		hotels[i].hashRoomID.clear();
		for (int k = 0; k < MAXFIND; k++) {
			hotels[i].cIdx[k] = 0;
		}
	}

	for (int i = 0; i < MAXREGION; i++) {
		for (int j = 0; j < MAXBEDS; j++) {
			for (int k = 0; k < MAXTYPE; k++) {
				for (int l = 0; l < MAXVIEW; l++) {
					minPrice[i][j][k][l].init();
				}
			}
		}
	}
}

void addRoom(int mHotelID, int mRoomID, int mRoomInfo[])
{
	int idx = hotels[mHotelID].length;
	hotels[mHotelID].RoomID[idx] = mRoomID;
	hotels[mHotelID].region[idx] = mRoomInfo[0];
	hotels[mHotelID].beds[idx] = mRoomInfo[1];
	hotels[mHotelID].roomType[idx] = mRoomInfo[2];
	hotels[mHotelID].viewType[idx] = mRoomInfo[3];
	hotels[mHotelID].price[idx] = mRoomInfo[4];
	hotels[mHotelID].hashRoomID[mRoomID] = idx;

	minPrice[mRoomInfo[0]][mRoomInfo[1]][mRoomInfo[2]][mRoomInfo[3]].push(mRoomInfo[4], mRoomID, mHotelID);

	hotels[mHotelID].length++;
}

int findRoom(int mFilter[])
{
	int region = mFilter[2];
	int bed = mFilter[3];
	int roomType = mFilter[4];
	int viewType = mFilter[5];

	int ans = -1;

	Data arr[MAXHEAP];
	int arrIdx = 0;

	while (minPrice[region][bed][roomType][viewType].length > 0 && ans == -1) {
		Data curr = minPrice[region][bed][roomType][viewType].pop();

		int idx = hotels[curr.hotelID].hashRoomID[curr.roomID];
		if (curr.price != hotels[curr.hotelID].price[idx]) { continue; }

		arr[arrIdx].price = curr.price;
		arr[arrIdx].hotelID = curr.hotelID;
		arr[arrIdx].roomID = curr.roomID;
		arrIdx++;

		bool isOverlap = false;
		for (int i = 0; i < hotels[curr.hotelID].cIdx[idx]; i++) {
			if (checkOverlap(mFilter[0], mFilter[1] - 1,
				hotels[curr.hotelID].checkIn[idx][i], hotels[curr.hotelID].checkOut[idx][i])) {
				isOverlap = true;
			}
		}
		if (!isOverlap) {
			ans = curr.roomID;
			int cIdx = hotels[curr.hotelID].cIdx[idx];
			hotels[curr.hotelID].checkIn[idx][cIdx] = mFilter[0];
			hotels[curr.hotelID].checkOut[idx][cIdx] = mFilter[1] - 1;
			hotels[curr.hotelID].cIdx[idx]++;
		}
	}

	for (int i = 0; i < arrIdx; i++) {
		minPrice[region][bed][roomType][viewType].push(arr[i].price, arr[i].roomID, arr[i].hotelID);
	}


	return ans;
}

int riseCosts(int mHotelID)
{
	int ans = 0;
	for (int i = 0; i < hotels[mHotelID].length; i++) {
		hotels[mHotelID].price[i] *= 1.1;
		ans += hotels[mHotelID].price[i];
		minPrice[hotels[mHotelID].region[i]][hotels[mHotelID].beds[i]][hotels[mHotelID].roomType[i]][hotels[mHotelID].viewType[i]].push(
			hotels[mHotelID].price[i], hotels[mHotelID].RoomID[i], mHotelID
		);
	}
	return ans;
}
