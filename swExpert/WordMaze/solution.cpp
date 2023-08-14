
#define MAX_LEN 12
#define MAXROOM 30001
#define MAXTABLE 1024


int djb2(char source[MAX_LEN]) {
	long long int hash = 5381;

	for (int i = 0; i < MAX_LEN; i++) {
		if (source[i] == '\0') { break; }
		hash = ((hash << 5) + hash) + source[i];
	}

	return (int) hash % MAXTABLE;
}

void strcpy(char dest[MAX_LEN], char source[MAX_LEN]) {
	for (int i = 0; i < MAX_LEN / 2; i++) {
		dest[i] = source[i];
		dest[MAX_LEN - 1 - i] = source[MAX_LEN - 1 - i];
	}
}

int strcmp(char str1[MAX_LEN], char str2[MAX_LEN]) {
	int ans = 0;
	for (int i = 0; i < MAX_LEN; i++) {
		ans += (str1[i] - str2[i]);	
		if (ans != 0) { break; }
	}

	return ans;
}



class HeapNode {
public:
	char word[MAX_LEN];
	int id;
};

class Heap {
public:
	int length;
	HeapNode arr[MAXROOM / 2];

	void init() { length = 0; }

	bool compare(int parent, int child) {
		if (strcmp(arr[parent].word, arr[child].word) > 0) { return true; }
		return false;
	}

	void push(char str[MAX_LEN], int mID) {
		HeapNode node;
		strcpy(node.word, str); node.id = mID;

		int idx = length;
		arr[length++] = node;

		while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
			HeapNode temp = arr[idx];
			arr[idx] = arr[(idx - 1) / 2];
			arr[(idx - 1) / 2] = temp;
			idx = (idx - 1) / 2;
		}
	}

	HeapNode pop() {
		HeapNode ans = arr[0];

		arr[0] = arr[--length];
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
				HeapNode temp = arr[idx];
				arr[idx] = arr[child];
				arr[child] = temp;
				idx = child;
			}
			else { break; }
		}

		return ans;
	}
};

class Node {
public:
	char word[MAX_LEN];
	int id;
	Node* next;
};


class LinkedList {
public:
	int length;
	Node* head;

	void init() {
		length = 0;
		head = nullptr;
	}

	void push(char w[MAX_LEN], int i) {
		Node* node = new Node();
		strcpy(node->word, w); node->id = i;

		node->next = head;
		head = node;

		length++;
	}
};



class Room {
public:
	char word[MAX_LEN];
	int dirLen[3];

	void add(char w[MAX_LEN], int d[3]) {
		strcpy(word, w);
		dirLen[0] = d[0]; dirLen[1] = d[1]; dirLen[2] = d[2];
	}

	
};

int order[3] = { 0, 4, 7 }; // front, center, back


Room rooms[MAXROOM];
LinkedList hashRoom[MAXTABLE];
int currID;
Heap minHeap[3][26][26];


bool compareWord(int curr, int next, int dir) {

	int dir_len = rooms[curr].dirLen[dir];

	if (dir == 0) {
		for (int i = 0; i < dir_len; i++) {
			if (rooms[curr].word[i] != rooms[next].word[MAX_LEN - dir_len - 1 + i]) { return false; }
		}
		
	}
	else if (dir == 1) {
		for (int i = order[dir]; i < order[dir] + dir_len; i++) {
			if (rooms[curr].word[i] != rooms[next].word[i]) { return false; }
		}
		
	}
	else {
		for (int i = 0; i < dir_len; i++) {
			if (rooms[curr].word[MAX_LEN - dir_len - 1 + i ] != rooms[next].word[i]) { return false; }
		}
		
	}

	return true;
}


int findByWord(char str[MAX_LEN]) {
	int key = djb2(str);
	
	Node* node = hashRoom[key].head;

	while (node) {
		if (strcmp(str, node->word) == 0) { return node->id; }
		node = node->next;
	}

	return -1;
}

void init()
{

	for (int i = 0; i < MAXTABLE; i++) {
		hashRoom[i].init();
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 26; j++) {
			for (int k = 0; k < 26; k++) {

				minHeap[i][j][k].init();
			}
		}
	}
}



void addRoom(int mID, char mWord[], int mDirLen[])
{

	rooms[mID].add(mWord, mDirLen);

	int key = djb2(mWord);
	hashRoom[key].push(mWord, mID);

	int k[3] = { 7, 4, 0 };

	for (int i = 0; i < 3; i++) {
		minHeap[i][mWord[k[i]] - 97][mWord[k[i] + 1] - 97].push(mWord, mID);
	}
	minHeap[0][mWord[9] - 97][mWord[9 + 1] - 97].push(mWord, mID);
}


void setCurrent(char mWord[])
{
	currID = findByWord(mWord);
}



int moveDir(int mDir)
{
	int ans = 0;

	int dirLen = rooms[currID].dirLen[mDir];

	int loc[3] = { 0, 4, 7 };
	if (dirLen == 2 && mDir == 2) { loc[2] += 2; }

	int k = rooms[currID].word[loc[mDir]] - 97;
	int k2 = rooms[currID].word[loc[mDir] + 1] - 97;

	HeapNode temp[minHeap[mDir][k][k2].length];
	int tIdx = 0;

	while (minHeap[mDir][k][k2].length > 0) {
		HeapNode curr = minHeap[mDir][k][k2].pop();

		if (strcmp(curr.word, rooms[curr.id].word) != 0) { continue; }
		
		temp[tIdx++] = curr;
		
		if (currID == curr.id) { continue; }

		if (compareWord(currID, curr.id, mDir)) {
			currID = ans = curr.id;
			break;
		}
	}

	for (int i = 0; i < tIdx; i++) {
		minHeap[mDir][k][k2].push(temp[i].word, temp[i].id);
	}

	return ans;
}



void changeWord(char mWord[], char mChgWord[], int mChgLen[])

{
	int mID = findByWord(mWord);
	rooms[mID].add(mChgWord, mChgLen);

	int key = djb2(mChgWord);
	hashRoom[key].push(mChgWord, mID);

	int k[3] = { 7, 4, 0 };

	for (int i = 0; i < 3; i++) {
		minHeap[i][mChgWord[k[i]] - 97][mChgWord[k[i] + 1] - 97].push(mChgWord, mID);
	}
	minHeap[0][mChgWord[9] - 97][mChgWord[9 + 1] - 97].push(mChgWord, mID);
}
