#define MAXN 50
#define MAXD 11

extern int useDetector(int P[MAXD][MAXD]);

#define MAXPATH 10000

int drows[4] = {-1, 0, 1, 0};
int dcols[4] = {0, 1, 0, -1};

int forestSize;
int detectorSize;

int converse(int a) {
	if (a >= forestSize) { return a - forestSize; }
	if (a < 0) { return a + forestSize; }
	return a;
}

struct Result
{
	int r, c;
};

class Prisoner {
public:
	bool isRemoved;
	int row;
	int col;
	int direction;
	bool visited[MAXN][MAXN];
	int around[MAXD][MAXD];

	void init(int r, int c, int d, int n) {
		row = r;
		col = c;
		direction = d;
		isRemoved = false;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				visited[i][j] = false;
			}
		}
	}

	void move(int ar[MAXD][MAXD]) {
		int r = converse(row + drows[direction]);
		int c = converse(col + dcols[direction]);

		while (visited[r][c]) {
			direction++;
			direction %= 4;
			r = converse(row + drows[direction]);
			c = converse(col + dcols[direction]);
		}

		row = converse(row + drows[direction]);
		col = converse(col + dcols[direction]);
		visited[row][col] = true;

		for (int i = 0; i < detectorSize; i++) {
			for (int j = 0; j < detectorSize; j++) {
				around[i][j] = ar[i][j];
			}
		}
	}
};

int forest[MAXN][MAXN];
Prisoner prisoner[MAXPATH];
int pIdx, checked;



bool isCurrentFit(int idx) {
	int rs = converse(prisoner[idx].row - detectorSize / 2);
	int cs = converse(prisoner[idx].col - detectorSize / 2);

	for (int i = 0; i < detectorSize; i++) {
		for (int j = 0; j < detectorSize; j++) {
			int r = converse(rs + i);
			int c = converse(cs + j);
			if (prisoner[idx].around[i][j] != forest[r][c]) {
				return false;
			}
		}
	}

	return true;
}

void init(int N, int D, int mForest[MAXN][MAXN], int K)
{
	forestSize = N;
	detectorSize = D;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			forest[i][j] = mForest[i][j];
		}
	}
}



Result findCriminal()
{
	pIdx = 0;
	checked = 0;
	for (int i = 0; i < forestSize; i++) {
		for (int j = 0; j < forestSize; j++) {
			for (int k = 0; k < 4; k++) {
				prisoner[pIdx].init(i, j, k, forestSize);
				prisoner[pIdx].visited[i][j] = true;
				pIdx++;
				checked++;
			}
		}
	}


	Result res;

	res.r = res.c = -1;
	int around[MAXD][MAXD];

	while (checked > 1) {
		useDetector(around);
		for (int i = 0; i < pIdx; i++) {
			if (prisoner[i].isRemoved) { continue; }

			prisoner[i].move(around);
			if (!isCurrentFit(i)) {
				prisoner[i].isRemoved = true;
				checked--;
			}
		}
		
	}

	for (int i = 0; i < pIdx; i++) {
		if (!prisoner[i].isRemoved) {
			res.r = prisoner[i].row;
			res.c = prisoner[i].col;
		}
	}

	return res;

}