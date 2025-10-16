#include <vector>
#include <unordered_map>
#include <queue>

#define MAX_N 101
#define MAX_M 50
#define MAX_K 1001
#define INF 1000000000

struct Code
{
    long long attr;

    int idx;

    Code()

    {

        attr = 0; idx = -1;

    }

    Code(long long a, int i)

    {

        attr = a; idx = i;

    }
};


struct Pipe
{

    Code code;

    int a, b;

    bool isRemoved;

    

    Pipe() {}

    Pipe(Code c, int aStorage, int bStorage)

    {

        code = c; a = aStorage; b = bStorage;

        isRemoved = false;

    }

};





struct Node
{

    int cost, storage;

    Code energy;

    int idx;

    Node(int c, int s, Code e, int i)
    {

        cost = c; storage = s; energy = e;

        idx = i;
    }

};



int n;

int m;

int k;

std::vector<Pipe> pipes(MAX_K);



std::unordered_map<int, int> hashID;

std::vector<std::vector<std::pair<int, int>>> adjacency(MAX_N);

std::vector<std::vector<int>> minCost(MAX_N, std::vector<int>(MAX_K));




struct compare
{
    bool operator()(Node &a, Node &b)
    {
        if(a.cost > b.cost) return true;

        return false;
    }
};


Code attrToCode(char mAttr[])
{
    long long attr = 0;

    int idx = -1;

    for(int i = 0; i < m; i++)

    {

        if(mAttr[i] == 'W')

        {

            long long temp = 1;

            temp <<= i;

            attr |= temp;

        }

        if(mAttr[i] == 'D')

        {

            idx = i;

        }

    }

    return Code(attr, idx);
}


int countSetBits(long long n)
{
    int count = 0;

    while (n > 0) {

        count += (n & 1);

        n >>= 1;

    }

    return count;
}



int cmpCode(Code a, Code b)
{

    long long res = a.attr ^ b.attr;

    int cnt = countSetBits(res);

    if(a.idx == b.idx)

    {

        cnt++;

    }

    else

    {

        long long aOnA = a.attr >> a.idx;

        long long bOnA = b.attr >> a.idx;

        

        cnt += (aOnA & 1) == (bOnA & 1);

        

        long long aOnB = a.attr >> b.idx;

        long long bOnB = b.attr >> b.idx;

        

        cnt += (aOnB & 1) == (bOnB & 1);

    }

    

    return cnt;

}



void getMinCost(int st, int ed, Code energy)
{
    for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < k; j++)
		{
			minCost[i][j] = INF;
		}
	}

 
    std::priority_queue<Node, std::vector<Node>, compare> pq;

    pq.push(Node(0, st, energy, 0));

    minCost[st][0] = 0;

	while(!pq.empty())
	{
		Node curr = pq.top();
		pq.pop();

		if(curr.storage == ed) return;

		for(std::pair<int, int> next : adjacency[curr.storage])
		{
			int to = next.first;
			int idx = next.second;

			if(pipes[idx].isRemoved) continue;

			int cost1 = curr.cost + cmpCode(pipes[idx].code, curr.energy);
			
			if(cost1 < minCost[to][curr.idx])
			{
				minCost[to][curr.idx] = cost1;

				pq.push(Node(cost1, to, curr.energy, curr.idx));
			}

			if(curr.idx > 0) continue;

			Code newEnergy = pipes[idx].code;
			
			int cost2 = curr.cost + cmpCode(pipes[idx].code, newEnergy);

			if(cost2 < minCost[to][idx])
			{
				minCost[to][idx] = cost2;

				pq.push(Node(cost2, to, newEnergy, idx));
			}
		}
	}
    
}


void add(int mID, int aStorage, int bStorage, char mAttr[])
{
    Code code = attrToCode(mAttr);
    
    adjacency[aStorage].push_back(std::make_pair(bStorage, k));

    adjacency[bStorage].push_back(std::make_pair(aStorage, k));

    hashID.insert(std::make_pair(mID, k));

    pipes[k++] = Pipe(code, aStorage, bStorage);
}



void init(int N, int M, int K, int mID[], int aStorage[], int bStorage[], char mAttr[][MAX_M])
{
    hashID.clear();

    m = M;

    k = 1;

    n = N + 1;

    for(int i = 0; i < n; i++)
    {

        adjacency[i].clear();
    }

    for(int i = 0; i < K; i++)
    {

        add(mID[i], aStorage[i], bStorage[i], mAttr[i]);
    }

}


void remove(int mID)
{
    int idx = hashID.find(mID)->second;

   
    pipes[idx].isRemoved = true;
}


int transport(int sStorage, int eStorage, char mAttr[])
{
    Code energy = attrToCode(mAttr);   

    getMinCost(sStorage, eStorage, energy);

    int res = INF;

	for(int i = 1; i < k; i++)
	{
		if(minCost[eStorage][i] < res)
		{
			res = minCost[eStorage][i];
		}
	}

    if(res == INF) return -1;

    return res;

}
