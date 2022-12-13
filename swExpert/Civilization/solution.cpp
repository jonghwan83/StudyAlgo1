#include <unordered_map>

#define MAXCIVILIZATION 60000
#define MAXGRID 1000

using namespace std;

class Node {
public:
	int row;
	int col;
	Node* next;
};

class LinkedList {
public:
	bool isRemoved;
	int id;
	Node* head;
	Node* tail;
	int length;

	LinkedList() {
		isRemoved = false;
		id = -1;
		length = 0;
		head = nullptr;
		tail = nullptr;
	}

	void clear() {
		isRemoved = false;
		id = -1;
		length = 0;
		head = nullptr;
		tail = nullptr;
	}

	void addNode(int r, int c) {
		Node* newNode = new Node();
		newNode->row = r;
		newNode->col = c;

		newNode->next = head;
		if (!tail) { tail = newNode; }
		head = newNode;

		length++;
	}
};


int drows[4] = { -1, 1, 0, 0 };
int dcols[4] = { 0, 0, -1, 1 };
int totalGrid, cIdx;
LinkedList civilization[MAXCIVILIZATION];
int grid[MAXGRID][MAXGRID];
unordered_map<int, int> hashCivilization;

void assembleLink(int cIdx1, int cIdx2) {
	Node* cNode = civilization[cIdx2].head;
	while (cNode) {
		grid[cNode->row][cNode->col] = cIdx1;
		cNode = cNode->next;
	}

	civilization[cIdx1].tail->next = civilization[cIdx2].head;
	civilization[cIdx1].tail = civilization[cIdx2].tail;
	civilization[cIdx1].length += civilization[cIdx2].length;

	civilization[cIdx2].isRemoved = true;
}

int checkNeighbor(int r, int c) {
	int dr, dc;
	int neighbors[4][2];
	int nIdx = 0;
	for (int i = 0; i < 4; i++) {
		dr = r + drows[i];
		dc = c + dcols[i];

		bool newFlag = true;

		if (0 < dr && dr < totalGrid && 0 < dc && dc < totalGrid) {
			if (grid[dr][dc] > -1 && !civilization[grid[dr][dc]].isRemoved) {
				if (nIdx) {
					for (int k = 0; k < nIdx; k++) {
						if (neighbors[k][0] == grid[dr][dc]) { 
							neighbors[k][1]++;
							newFlag = false;
							break;
						}
					}
					if (newFlag) {
						neighbors[nIdx][0] = grid[dr][dc];
						neighbors[nIdx][1] = 1;
						nIdx++;
					}
				}
				else {
					neighbors[nIdx][0] = grid[dr][dc];
					neighbors[nIdx][1] = 1;
					nIdx++;
				}
			}
		}
	}

	int ans = 0;
	int numAns = 0;
	if (nIdx) {
		for (int i = 0; i < nIdx; i++) {
			if (neighbors[i][1] > numAns) {
				ans = neighbors[i][0];
				numAns = neighbors[i][1];
			}
			else if (neighbors[i][1] == numAns && civilization[ans].id > civilization[neighbors[i][0]].id) {
				ans = neighbors[i][0];
			}
		}
		return ans;
	}
	return -1;
	
}

void init(int N) {
	totalGrid = N + 1;
	cIdx = 1;

	for (int i = 0; i < totalGrid; i++) {
		for (int j = 0; j < totalGrid; j++) {
			grid[i][j] = -1;
		}
	}

	hashCivilization.clear();
}

int newCivilization(int r, int c, int mID) {
	int neighbor = checkNeighbor(r, c);
	if (neighbor == -1) {
		civilization[cIdx].clear();

		grid[r][c] = cIdx;
		hashCivilization[mID] = cIdx;

		civilization[cIdx].id = mID;
		civilization[cIdx].addNode(r, c);
		
		cIdx++;
	}
	else {
		grid[r][c] = neighbor;
		civilization[neighbor].addNode(r, c);
	}
	return civilization[grid[r][c]].id;
}

int removeCivilization(int mID) {
	int cIdx = hashCivilization[mID];
	int ans = 0;
	if (!civilization[cIdx].isRemoved) {
		ans += civilization[cIdx].length;
		civilization[cIdx].isRemoved = true;
	}
	return ans;
}

int getCivilization(int r, int c) {
	int cIdx = grid[r][c];
	if (cIdx == -1) { return 0; }
	if (!civilization[cIdx].isRemoved) {
		return civilization[cIdx].id;
	}
	return 0;
}

int getCivilizationArea(int mID) {
	int cIdx = hashCivilization[mID];
	if (!civilization[cIdx].isRemoved) {
		return civilization[cIdx].length;
	}
	return 0;
}

int mergeCivilization(int mID1, int mID2) {
	int cIdx1 = hashCivilization[mID1];
	int cIdx2 = hashCivilization[mID2];
	assembleLink(cIdx1, cIdx2);

	return civilization[cIdx1].length;
}