#include <queue>

#define MAX_CITY 200
#define MAX_INCREASE 40001

struct Road
{
	int to, toll, when;

	Road() {}
	Road(int t, int p, int w)
	{
		to = t; toll = p; when = w;
	}
};

struct Node
{
	int cost, city;

	Node(int mToll, int mCity)
	{
		cost = mToll; city = mCity;
	}
};

struct compare
{
	bool operator()(Node &a, Node &b)
	{
		if(a.cost > b.cost) return true;
		return false;
	}
};

int nCity;
std::vector<std::vector<Road>> roads(MAX_CITY);

int nIncr;
int cumsum;
int increase[MAX_INCREASE];

int minCost[MAX_CITY];

void getMinCost(int sCity, int eCity)
{
	for(int i = 0; i < nCity; i++)
		minCost[i] = 1e9;

	std::priority_queue<Node, std::vector<Node>, compare> pq;
	pq.push(Node(0, sCity));

	while(!pq.empty())
	{
		Node curr = pq.top();
		pq.pop();

		if(eCity == curr.city) return;

		for(Road next: roads[curr.city])
		{
			int offset = increase[next.when - 1];
			int cost = curr.cost + next.toll + cumsum - offset;

			if(cost >= minCost[next.to]) continue;

			minCost[next.to] = cost;
			pq.push(Node(cost, next.to));
		}
	}
}

void add(int sCity, int eCity, int mToll) {
	roads[sCity].push_back(Road(eCity, mToll, nIncr));
	roads[eCity].push_back(Road(sCity, mToll, nIncr));

	return;
}

void init(int N, int K, int sCity[], int eCity[], int mToll[]) {
	nCity = N;
	nIncr = 1;
	increase[0] = 0;
	cumsum = 0;

	for(int i = 0;i < N; i++)
	{
		roads[i].clear();
	}

	for(int i = 0; i < K; i++)
	{
		add(sCity[i], eCity[i], mToll[i]);
	}

	return;
}


void calculate(int sCity, int eCity, int M, int mIncrease[], int mRet[]) {
	getMinCost(sCity, eCity);
	mRet[0] = minCost[eCity];

	for(int i = 0; i < M; i++)
	{	
		cumsum += mIncrease[i];
		increase[nIncr] = mIncrease[i];
		if(nIncr > 0) increase[nIncr] += increase[nIncr-1];
		nIncr++;

		getMinCost(sCity, eCity);
		mRet[i+1] = minCost[eCity];
	}

	return;
}