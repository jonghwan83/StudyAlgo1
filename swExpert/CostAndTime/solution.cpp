#define MAXNEXT 1000
#define MAXCITY 100
#define INF 1'000'000'000


int min(int a, int b) { return a < b ? a : b; }

struct Node
{
	int city, cost, time;
	Node* next;

	Node(int ct, int c, int t)
	{
		city = ct;
		cost = c;
		time = t;
		next = nullptr;
	}
};

class LinkedList
{
public:
	int length;
	Node* head;

	void init()
	{
		length = 0;
		head = nullptr;
	}

	void push(int city, int cost, int time)
	{
		Node* node = new Node(city, cost, time);

		node->next = head;
		head = node;

		length++;
	}
};

struct HeapNode
{
	int cost;
	int time;
	int city;
};

class Heap
{
public:
	int length;
	HeapNode arr[MAXNEXT * 50];

	void init() { length = 0; }

	bool compare(int parent, int child)
	{
		if (arr[parent].time > arr[child].time)
			return true;
		if (arr[parent].time == arr[child].time && arr[parent].cost > arr[parent].cost)
			return true;
		return false;
	}

	void push(int cost, int time, int city)
	{
		HeapNode node = { cost, time, city };

		int idx = length;
		arr[length++] = node;

		while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx))
		{
			HeapNode temp = arr[idx];
			arr[idx] = arr[(idx - 1) / 2];
			arr[(idx - 1) / 2] = temp;
			idx = (idx - 1) / 2;
		}
	}

	HeapNode pop()
	{
		HeapNode ans = arr[0];

		int idx = 0;
		arr[0] = arr[--length];

		int left, right, child;

		while (2 * idx + 1 < length)
		{
			left = 2 * idx + 1;
			right = 2 * idx + 2;

			if (right < length)
				if (compare(left, right))
					child = right;
				else
					child = left;
			else
				child = left;

			if (compare(idx, child))
			{
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



LinkedList grid[MAXCITY];
Heap pQueue;
int N;

void init(int _N, int K, int sCity[], int eCity[], int mCost[], int mTime[]) {

	N = _N;

	for (int i = 0; i < N; i++)
		grid[i].init();

	for (int i = 0; i < K; i++)
		grid[sCity[i]].push(eCity[i], mCost[i], mTime[i]);

	return;
}


void add(int sCity, int eCity, int mCost, int mTime) {

	grid[sCity].push(eCity, mCost, mTime);

	return;
}


int cost(int M, int sCity, int eCity) {

	pQueue.init();
	pQueue.push(0, 0, sCity);

	while (pQueue.length > 0)
	{
		HeapNode curr = pQueue.pop();

		if (M - curr.cost < 0) { continue; }

		if (curr.city == eCity) { return curr.time; }

		Node* node = grid[curr.city].head;
		while (node)
		{
			pQueue.push(curr.cost + node->cost, curr.time + node->time, node->city);

			node = node->next;
		}
	}

	return -1;
}
