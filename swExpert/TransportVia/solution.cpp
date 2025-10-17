#include <vector>
#include <queue>

#define MAX_N 1000
#define INF 1000000000

int n;
std::vector<std::vector<std::pair<int, int>>> edges(MAX_N);
std::vector<int> maxCost(MAX_N);

struct Node
{
	int cost, city;

	Node(int mLimit, int mCity)
	{
		cost = mLimit; city = mCity;
	}
};

void add(int sCity, int eCity, int mLimit)
{
	edges[sCity].push_back(std::make_pair(eCity, mLimit));
	edges[eCity].push_back(std::make_pair(sCity, mLimit));		

	
	return;
}


void init(int N, int K, int sCity[], int eCity[], int mLimit[]) 
{
	n = N;

	for(int i = 0; i < N; i++)
	{
		edges[i].clear();
	}

	for(int i = 0; i < K; i++)
	{
		add(sCity[i], eCity[i], mLimit[i]);
	}
	
	return;
}


struct compare
{
	bool operator()(Node &a, Node &b)
	{
		return a.cost < b.cost;
	}
};

void getMaxCost(int st, int ed)
{
	for(int i = 0; i < n; i++)
	{
		maxCost[i] = 0;
	}
	
	std::priority_queue<Node, std::vector<Node>, compare> pq;

	pq.push(Node(INF, st));
	maxCost[st] = INF;		
	
	while(!pq.empty())
	{
		Node curr = pq.top();
		pq.pop();

		//if(curr.city == ed) return;

		for(std::pair<int, int> next: edges[curr.city])
		{
			int to = next.first;
			int lim = next.second;

			int load = std::min(curr.cost, lim);

			if(load > maxCost[to])
			{
				maxCost[to] = load;
				pq.push(Node(load, to));
			}
		}	
	}
}


int calculate(int sCity, int eCity, int M, int mStopover[]) {

	int res = INF;

	for(int i = 0; i < M; i++)
	{
		getMaxCost(sCity, mStopover[i]);
		if(maxCost[mStopover[i]] == 0) return -1;
		res = std::min(res, maxCost[mStopover[i]]);	
	}

	getMaxCost(mStopover[M-1], eCity);
	res = std::min(res, maxCost[eCity]);
	
	if(res == 0) return -1;

	return res;
}
