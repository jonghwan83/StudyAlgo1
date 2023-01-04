#include <deque>
#include <vector>

#define MAXLINE 5
#define MAXSTATIONS 40000

using namespace std;

class Node {
public:
	int id;
	Node* next;
	Node* prev;

	Node(int a) {
		id = a;
		next = nullptr;
		prev = nullptr;
	}
};

class LinkedList {
public:
	Node* line[MAXLINE];

	void init() {
		for (int i = 0; i < MAXLINE; i++) {
			line[i] = nullptr;
		}
	}
};

int totalStation;
LinkedList stations[MAXSTATIONS];
bool transfer[MAXLINE][MAXLINE];

void setTransfer(int id, int line) {
	for (int i = 0; i < MAXLINE; i++) {
		if (stations[id].line[i] == nullptr || i == line) {
			continue;
		}
		transfer[i][line] = true;
		transfer[line][i] = true;
	}
}

void init(int N, int mLastStation1[5], int mLastStation2[5]) {
	totalStation = N;

	for (int i = 0; i < MAXLINE; i++) {
		for (int j = 0; j < MAXLINE; j++) {
			transfer[i][j] = false;
		}
	}

	for (int i = 0; i < N; i++) {
		stations[i].init();
	}

	for (int i = 0; i < MAXLINE; i++) {
		Node* station1 = new Node(mLastStation1[i]);
		Node* station2 = new Node(mLastStation2[i]);
		station1->next = station2;
		station2->prev = station1;
		
		stations[mLastStation1[i]].line[i] = station1;
		stations[mLastStation2[i]].line[i] = station2;

		setTransfer(mLastStation1[i], i);
		setTransfer(mLastStation2[i], i);
	}
}

void add(int mLine, int mPrevStation, int mStation) {
	Node* station = new Node(mStation);
	
	station->prev = stations[mPrevStation].line[mLine];
	station->next = stations[mPrevStation].line[mLine]->next;

	stations[mPrevStation].line[mLine]->next->prev = station;
	stations[mPrevStation].line[mLine]->next = station;
	stations[mStation].line[mLine] = station;

	setTransfer(mStation, mLine);
}

int minTravelTime(int mStartStation, int mEndStation)
{
	deque<pair<int, int> > queue;
	int visited[MAXSTATIONS][MAXLINE];

	for (int i = 0; i < totalStation; i++) {
		for (int j = 0; j < MAXLINE; j++) {
			visited[i][j] = MAXSTATIONS;
		}
	}

	for (int i = 0; i < MAXLINE; i++) {
		if (stations[mStartStation].line[i]) {
			queue.push_back(make_pair(mStartStation, i));
			visited[mStartStation][i] = 0;
		}
	}

	while (!queue.empty()) {
		pair<int, int> curr = queue.front();
		queue.pop_front();

		for (int i = 0; i < MAXLINE; i++) {
			if (stations[curr.first].line[i]) {
				if (stations[curr.first].line[i]->next) {
					if (visited[stations[curr.first].line[i]->next->id][i] > visited[curr.first][curr.second] + 1) {
						visited[stations[curr.first].line[i]->next->id][i] = visited[curr.first][curr.second] + 1;
						if (i != curr.second) {
							visited[stations[curr.first].line[i]->next->id][i]++;
						}
						queue.push_back(make_pair(stations[curr.first].line[i]->next->id, i));
					}
				}
				
				if (stations[curr.first].line[i]->prev) {
					if (visited[stations[curr.first].line[i]->prev->id][i] > visited[curr.first][curr.second] + 1) {
						visited[stations[curr.first].line[i]->prev->id][i] = visited[curr.first][curr.second] + 1;
						if (i != curr.second) {
							visited[stations[curr.first].line[i]->prev->id][i]++;
						}
						queue.push_back(make_pair(stations[curr.first].line[i]->prev->id, i));
					}
				}
			}
		}
	}

	int minTime = MAXSTATIONS;
	for (int i = 0; i < MAXLINE; i++) {
		if (minTime > visited[mEndStation][i]) {
			minTime = visited[mEndStation][i];
		}
	}

	if (minTime < MAXSTATIONS) {
		return minTime;
	}

	return -1;
}

int minTransfer(int mStartStation, int mEndStation)
{
	deque<int> queue;
	int visited[MAXLINE];
	
	for (int i = 0; i < MAXLINE; i++) {
		if (stations[mStartStation].line[i]) {
			queue.push_back(i);
			visited[i] = 0;
		}
		else {
			visited[i] = MAXSTATIONS;
		}
	}

	while (!queue.empty()) {
		int curr = queue.front();
		queue.pop_front();

		for (int nextLine = 0; nextLine < MAXLINE; nextLine++) {
			if (transfer[curr][nextLine] && visited[curr] + 1 <  visited[nextLine]) {
				visited[nextLine] = visited[curr] + 1;
				queue.push_back(nextLine);
			}
		}
	}

	int minTransfer = MAXSTATIONS;
	for (int i = 0; i < MAXLINE; i++) {
		if (stations[mEndStation].line[i] && visited[i] < minTransfer) {
			minTransfer = visited[i];
		}
	}
	
	if (minTransfer < MAXSTATIONS) {
		return minTransfer;
	}
	return -1;
}