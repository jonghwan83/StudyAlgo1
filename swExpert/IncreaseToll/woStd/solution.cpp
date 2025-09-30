#define MAX_CITY 200
#define MAX_INCREASE 40001

struct Road
{
	int to, toll, when;

	Road() {}
	Road(int eCity, int mToll, int inc)
	{
		to = eCity; toll = mToll; when = inc;
	}
};

struct Node
{
	int cost, city;

	Node() {}
	Node(int mToll, int mCity)
	{
		cost = mToll; city = mCity;
	}
};



struct pQueue
{
	int length;
	Node arr[MAX_CITY * 2];

	void init()
	{
		length = 0;
	}

	bool compare(int parent, int child)
	{
		if(arr[parent].cost > arr[child].cost) return true;
		return false;
	}

	void push(int mCost, int mCity)
	{
		Node newNode = Node(mCost, mCity);
		int idx = length;
		arr[length++] = newNode;

		while((idx-1)/2 >= 0 && compare((idx-1)/2, idx))
		{
			Node temp = arr[idx];
			arr[idx] = arr[(idx-1)/2];
			arr[(idx-1)/2] = temp;
			idx = (idx-1)/2;
		}
	}

	Node pop()
	{
		Node res = arr[0];
		arr[0] = arr[--length];

		int left, right, child;
		int idx = 0;

		while(2*idx + 1 < length)
		{
			int left = 2 * idx + 1;
			int right = 2 * idx + 2;

			if(right < length)
				if(compare(left, right)) child = right;
				else child = left;
			else
				child = left;

			if(compare(idx, child))
			{
				Node temp = arr[idx];
				arr[idx] = arr[child];
				arr[child] = temp;
				idx = child;
			}
			else
				break;
		}

		return res;
	}
};

int nCity;
int len_next[MAX_CITY];
Road roads[MAX_CITY][MAX_CITY];

int nIncr;
int increase[MAX_INCREASE];

int minCost[MAX_CITY];
pQueue pq;


void getMinCost(int sCity, int eCity)
{
	for(int i = 0; i < nCity; i++)
	{
		minCost[i] = 1e9;
	}

	pq.init();
	pq.push(0, sCity);
	minCost[sCity] = 0;

	while(pq.length > 0)
	{
		Node curr = pq.pop();

		if(curr.city == eCity) return;

		for(int i = 0; i < len_next[curr.city]; i++)
		{
			Road r = roads[curr.city][i];
			int increased = increase[nIncr-1] - increase[r.when-1];
			int cost = curr.cost + r.toll + increased;
			if(cost >= minCost[r.to]) continue;
			minCost[r.to] = cost;
			pq.push(cost, r.to);
		}
	}

}

void add(int sCity, int eCity, int mToll) {
	roads[sCity][len_next[sCity]] = Road(eCity, mToll, nIncr);
	len_next[sCity]++;

	roads[eCity][len_next[eCity]] = Road(sCity, mToll, nIncr);
	len_next[eCity]++;

	return;
}

void init(int N, int K, int sCity[], int eCity[], int mToll[]) {
	for(int i = 0;i < N; i++)
	{
		len_next[i] = 0;
	}

	nCity = N;
	increase[0] = 0;
	nIncr = 1;

	for(int i = 0; i < K; i++)
	{
		add(sCity[i], eCity[i], mToll[i]);
	}

	return;
}


void calculate(int sCity, int eCity, int M, int mIncrease[], int mRet[]) {
	getMinCost(sCity, eCity);

	mRet[0] = minCost[eCity];
	
	for(int i = 0;i < M; i++)
	{
		increase[nIncr] = mIncrease[i];
		if(nIncr > 0) increase[nIncr] += increase[nIncr-1];
		nIncr++;

		getMinCost(sCity, eCity);
		mRet[i+1] = minCost[eCity];
	}

	return;
}