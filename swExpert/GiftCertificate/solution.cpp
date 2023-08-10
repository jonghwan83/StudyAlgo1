#define MAXN 18001
#define TABLESIZE 1024


int min(int a, int b) {
	if (a < b) { return a; }
	return b;
}

struct HeapNode {
	int sIdx;
	int num;
};

class Heap {
public:
	int length;
	HeapNode arr[MAXN];

	void init() { length = 0; }

	bool compare(int parent, int child) {
		if (arr[parent].num > arr[child].num) { return true; }
		return false;
	}

	void push(int mNum, int i) {
		HeapNode node;
		node.sIdx = i; node.num = mNum;

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


struct Node {
	int value;
	int sIdx;
	Node* next;
};

class LinkedList {
public:
	int length;
	Node* head;

	void init() {
		length = 0; head = nullptr;
	}

	void push(int val, int idx) {
		Node* node = new Node();
		node->value = val; node->sIdx = idx;

		node->next = head;
		head = node;
		length++;
	}
};


class Department {
public:
	int parent;
	int id;
	int num;
	int nSub;

	void init(int mId, int mNum) {
		parent = -1;
		id = mId; num = mNum;
		nSub = 0;
	}
};

int nGroup;
int sIdx;
Department sections[MAXN];
LinkedList hashSections[TABLESIZE];
Heap minHeap;

bool isRemoved(int idx) {
	
	if (sections[idx].num == 0) { return true; }

	while (sections[idx].parent > -1) {
		idx = sections[idx].parent;
		if (sections[idx].num == 0) { return true; }
	}

	return false;
}

int findByID(int mId) {
	int key = mId % TABLESIZE;

	Node* node = hashSections[key].head;

	while (node) {
		if (node->value == mId) {
			return node->sIdx;
		}
		node = node->next;
	}

	return -1;
}

void init(int N, int mId[], int mNum[]) {

	for (int i = 0; i < TABLESIZE; i++) {
		hashSections[i].init();
	}


	for (int i = 0; i < N; i++) {
		sections[i].init(mId[i], mNum[i]);

		int key = mId[i] % TABLESIZE;
		hashSections[key].push(mId[i], i);
	}

	sIdx = N;

	nGroup = N;

	return;
}



int add(int mId, int mNum, int mParent) {

	int parent = findByID(mParent);
	if (sections[parent].nSub >= 3) { return -1; }

	sections[sIdx].init(mId, mNum);
	sections[sIdx].parent = parent;
	sections[parent].nSub++;

	int key = mId % TABLESIZE;
	hashSections[key].push(mId, sIdx);

	int idx = sIdx;
	while (sections[idx].parent > -1) {
		idx = sections[idx].parent;
		sections[idx].num += mNum;
	}

	sIdx++;

	return sections[parent].num;
}



int remove(int mId) {

	int idx = findByID(mId);
	if (idx == -1) { return -1; }

	if (isRemoved(idx)) { return -1; }

	int pIdx = idx;
	int ans = sections[idx].num;

	while (sections[pIdx].parent > -1) {
		pIdx = sections[pIdx].parent;
		sections[pIdx].num -= ans;
	}

	minHeap.push(sections[pIdx].num, pIdx);

	sections[idx].num = 0;
	sections[sections[idx].parent].nSub--;

	return ans;
}



int distribute(int K) {
	minHeap.init();
	int total = 0;

	for (int i = 0; i < nGroup; i++) {
		minHeap.push(sections[i].num, i);
		total += sections[i].num;
	}


	int ans = 0;

	while (minHeap.length > 0) {
		HeapNode curr = minHeap.pop();

		if (K / (minHeap.length + 1) > ans) {
			ans = K / (minHeap.length + 1);
		}

		K -= min(ans, curr.num);
		if (ans < curr.num) { break; }
		
	}

	return ans;
}
