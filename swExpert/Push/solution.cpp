
#define MAX_N 30

struct Coor
{
	int row, col;
};

int max(int a, int b)
{
	return (a > b) ? a : b;
}

struct Node
{
	int distance, count;
	int row, col, dir;
};

class Heap {
public:
	int length;
	Node arr[MAX_N * MAX_N + 100];

	void init() { length = 0; }

	bool compare(int parent, int child)
	{
		int parentScore = arr[parent].distance / 3 + arr[parent].count;
		int childScore = arr[child].distance / 3 + arr[child].count;

		if (parentScore > childScore)
			return true;

		return false;
	}

	void push(int dist, int cnt, int r, int c, int d)
	{
		Node node = { dist, cnt, r, c, d };

		int idx = length;
		arr[length++] = node;

		while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx))
		{
			Node temp = arr[idx];
			arr[idx] = arr[(idx - 1) / 2];
			arr[(idx - 1) / 2] = temp;
			idx = (idx - 1) / 2;
		}
	}

	Node pop()
	{
		Node ans = arr[0];

		arr[0] = arr[--length];

		int idx = 0;
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
				Node temp = arr[idx];
				arr[idx] = arr[child];
				arr[child] = temp;
				idx = child;
			}
			else
				break;
		}

		return ans;
	}
};

int abs(int a)
{
	if (a > 0) { return a; }
	return -a;
}

int manhattan(int r1, int c1, int r2, int c2) { return abs(r1 - r2) + abs(c1 - c2); }

int n;
int map[MAX_N][MAX_N];

int reverse[4] = { 2, 3, 0, 1 };

int drows[4] = { -1, 0, 1, 0 };
int dcols[4] = { 0, 1, 0, -1 };

Heap pQueue;
Heap pCheck;

int nVisited;
int visited[MAX_N][MAX_N];

int nChecked;
int checked[MAX_N][MAX_N];

int rockR, rockC;

bool detour(int cr, int cc, int dir, int destR, int destC)
{
	nChecked++;
	pCheck.init();

	int r = cr + drows[dir];
	int c = cc + dcols[dir];

	pCheck.push(manhattan(r, c, destR, destC), 0, r, c, dir);
	checked[r][c] = nChecked;

	if (r == destR && c == destC) { return true; }

	while (pCheck.length > 0)
	{
		Node curr = pCheck.pop();

		for (int i = 0; i < 4; i++)
		{
			int dr = curr.row + drows[i];
			int dc = curr.col + dcols[i];

			if (dr < 0 || dc < 0 || dr >= n || dc >= n) { continue; }

			if (map[dr][dc] == 1) { continue; }

			if (checked[dr][dc] >= nChecked) { continue; }

			if (dr == cr && dc == cc) { continue; }

			if (dr == destR && dc == destC) { return true; }

			checked[dr][dc] = nChecked;

			pCheck.push(manhattan(dr, dc, destR, destC), curr.count + 1, dr, dc, reverse[i]);
		}
	}

	return false;
}

int bfs(int goalR, int goalC, int mDir)
{
	nVisited ++;
	pQueue.init();

	pQueue.push(manhattan(rockR, rockC, goalR, goalC), 0, rockR, rockC, mDir);
	visited[rockR][rockC] = nVisited;

	while (pQueue.length > 0)
	{
		Node curr = pQueue.pop();

		if (curr.count > n * n) { continue; }

		for (int i = 0; i < 4; i++)
		{
			int dr = curr.row + drows[i];
			int dc = curr.col + dcols[i];

			if (dr < 0 || dc < 0 || dr >= n || dc >= n) { continue; }
			if (map[dr][dc] == 1) { continue; }
			if (visited[dr][dc] >= nVisited) { continue; }

			int destR = curr.row + drows[reverse[i]];
			int destC = curr.col + dcols[reverse[i]];

			if (!detour(curr.row, curr.col, curr.dir, destR, destC))
			{
				dr = curr.row + drows[reverse[curr.dir]];
				dc = curr.col + dcols[reverse[curr.dir]];

				if (dr < 0 || dc < 0 || dr >= n || dc >= n) { continue; }
				if (map[dr][dc] == 1) { continue; }

				if (dr == goalR && dc == goalC)
					return curr.count + 1;
				
				visited[curr.row][curr.col]--;

				continue;
			}

			if (dr == goalR && dc == goalC)
				return curr.count + 1;

			visited[dr][dc] = nVisited;
			pQueue.push(manhattan(dr, dc, goalR, goalC), curr.count + 1, dr, dc, reverse[i]);
		}
	}

	return 0;
}

void init(int N, int mMap[MAX_N][MAX_N])
{
	n = N;

	for (int r = 0; r < N; r++)
		for (int c = 0; c < N; c++)
		{
			map[r][c] = mMap[r][c];
			visited[r][c] = 0;
			checked[r][c] = 0;
		}
	
	nVisited = 0;
	nChecked = 0;

	return;
}


int push(int mRockR, int mRockC, int mDir, int mGoalR, int mGoalC)
{
	rockR = mRockR; rockC = mRockC;
	
	return bfs(mGoalR, mGoalC, mDir);
}
