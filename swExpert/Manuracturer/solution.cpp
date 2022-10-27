#define MAXTABLE 100000

#include <vector>
#include <algorithm> 

using namespace std;

vector<pair<int, int> > machines;
vector< vector< pair<int, bool> > > machineList(MAXTABLE);

void changeMachine(int mId, int mTime) {
	int cId, cTime;
	vector< pair<int, int> > temp;
	while (!machines.empty()) {
		cId = machines.front().second;
		cTime = machines.front().first;
		pop_heap(machines.begin(), machines.end());
		machines.pop_back();

		if (cId == mId) {
			cTime = mTime;
			temp.push_back(make_pair(cTime, cId));
			break;
		}
		temp.push_back(make_pair(cTime, cId));
	}

	for (int i = 0; i < temp.size(); i++) {
		machines.push_back(temp[i]);
		push_heap(machines.begin(), machines.end());
	}
}

void removeMachine() {
	int cId, cTime;
	cId = machines.front().second;
	cTime = machines.front().first;

	pop_heap(machines.begin(), machines.end());
	machines.pop_back();
	
	int key = cId % MAXTABLE;
	for (int i = 0; i < machineList[key].size(); i++) {
		if (machineList[key][i].first == cId) {
			machineList[key][i].second = false;
		}
	}
}

void insertMachineList(int mId) {
	int key = mId % MAXTABLE;
	for (int i = 0; i < machineList[key].size(); i++) {
		if (machineList[key][i].first == mId) {
			machineList[key][i].second = true;
			return;
		}
	}
	machineList[key].push_back(make_pair(mId, true));
}

int getProduced(int t) {
	int ans = 0;

	for (auto machine : machines) {
		ans += t / machine.first;
	}
	return ans;
}

void init(int N, int mId[], int mTime[]) {
	machines.clear();

	for (int i = 0; i < MAXTABLE; i++) {
		machineList[i].clear();
	}
	
	int key;
	for (int i = 0; i < N; i++) {
		machines.push_back(make_pair(mTime[i], mId[i]));
		push_heap(machines.begin(), machines.end());
		key = mId[i] % MAXTABLE;
		machineList[key].push_back(make_pair(mId[i], true));
	}

	return;
}

int add(int mId, int mTime) {
	int key = mId % MAXTABLE;
	for (int i = 0; i < machineList[key].size(); i++) {
		if (machineList[key][i].first == mId) {
			changeMachine(mId, mTime);
			return machines.size();
		}
	}

	machines.push_back(make_pair(mTime, mId));
	push_heap(machines.begin(), machines.end());
	machineList[key].push_back(make_pair(mId, true));

	return machines.size();
}

int remove(int K) {
	int id, key;
	for (int i = 0; i < K; i++) {
		removeMachine();
	}

	return machines.front().second;
}

int produce(int M) {
	int k = 0;
	int total = 0;

	int cTime = 0;
	while (total < M) {
		k++;
		cTime = k * machines.front().first;
		total = getProduced(cTime);
	}

	if (total > M) {
		k--;
		cTime = k * machines.front().first;
		total = getProduced(cTime);
	}

	while (total < M) {
		cTime++;
		total = getProduced(cTime);
	}

	return cTime;
}