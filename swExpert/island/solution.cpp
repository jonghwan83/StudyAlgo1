#include <vector>
#include <iostream>

#define MAXSIZE 20
#define MAXLENGTH 5
#define OFFSET 10
#define MAXISLAND 21

using namespace std;

int islandSize, cnt;
vector<int> drows = { -1, 1, 0, 0 };
vector<int> dcols = { 0, 0, -1, 1 };

vector< vector< vector< vector< vector< vector<int> > > > > > hashStructure(MAXSIZE, 
	vector< vector< vector< vector< vector<int> > > > > (MAXSIZE, 
		vector< vector< vector< vector<int> > > > (MAXSIZE, 
			vector< vector< vector<int> > > (MAXSIZE))));

vector< vector<int> > island(MAXISLAND, 
	vector<int> (MAXISLAND));

vector< vector<bool> > visited(MAXISLAND,
	vector<bool>(MAXISLAND));

vector< vector<int> > seaRised(MAXISLAND,
	vector<int>(MAXISLAND));

void putStructure(int r, int c, int k, int M, int mStructure[]) {
	int dr, dc;
	int t = 0;
	for (int i = 0; i < islandSize; i++) {
		for (int j = 0; j < islandSize; j++) {
			seaRised[i][j] = island[i][j];
			visited[i][j] = false;
		}
	}
	while (t < M) {
		dr = r + drows[k] * t;
		dc = c + dcols[k] * t;
		seaRised[dr][dc] += mStructure[t];
		t++;
	}
}

void riseSeaLevel(int r, int c, int mSeaLevel) {
	if (seaRised[r][c] >= mSeaLevel) {
		return;
	}

	if (seaRised[r][c] < mSeaLevel && !visited[r][c]) {
		cnt++;
		visited[r][c] = true;
	}

	int dr, dc;
	for (int i = 0; i < 4; i++) {
		dr = r + drows[i];
		dc = c + dcols[i];
		if (0 <= dr && dr < islandSize && 0 <= dc && dc < islandSize) {
			if (seaRised[dr][dc] < mSeaLevel && !visited[dr][dc]) {
				seaRised[dr][dc] == 0;
				visited[dr][dc] = true;
				cnt++;
				riseSeaLevel(dr, dc, mSeaLevel);
			}
		}
	}
}


void init(int N, int mMap[20][20])
{
	for (int i = 0; i < MAXSIZE; i++) {
		for (int j = 0; j < MAXSIZE; j++) {
			for (int k = 0; k < MAXSIZE; k++) {
				for (int l = 0; l < MAXSIZE; l++) {
					hashStructure[i][j][k][l].clear();
				}
			}
		}
	}

	islandSize = N;
	int dr, dc;
	vector<int> idx;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			island[i][j] = mMap[i][j];
			for (int l = 0; l < 4; l++) {
				idx.clear();
				idx.resize(4);
				for (int k = 1; k < MAXLENGTH; k++) {
					dr = i + drows[l] * k;
					dc = j + dcols[l] * k;
					if (0 <= dr && dr < N && 0 <= dc && dc < N) {
						idx[k - 1] = mMap[i][j] - mMap[dr][dc] + OFFSET;
						hashStructure[idx[0]][idx[1]][idx[2]][idx[3]].push_back({ i,  j,  l });
					}
				}
			}
		}
	}
}

int numberOfCandidate(int M, int mStructure[5])
{
	if (M == 1) { return islandSize * islandSize; }
	bool isSymmetry = false;
	if (M == 3) {
		if (mStructure[0] == mStructure[2]) { isSymmetry = true; }
	}
	else if (M == 2) {
		if (mStructure[0] == mStructure[1]) { isSymmetry = true; }
	}
	else if (M == 4) {
		if (mStructure[0] == mStructure[3] && mStructure[1] == mStructure[2]) { isSymmetry = true; }
	}
	else if (M == 5) {
		if (mStructure[0] == mStructure[4] && mStructure[1] == mStructure[3]) { isSymmetry = true; }
	}

	vector<int> idx(4);
	for (int i = 0; i < (M-1); i++) {
		idx[i] = mStructure[i + 1] - mStructure[0] + OFFSET;
	}

	if (isSymmetry) {
		return hashStructure[idx[0]][idx[1]][idx[2]][idx[3]].size() / 2;
	}
	return hashStructure[idx[0]][idx[1]][idx[2]][idx[3]].size();
}

int maxArea(int M, int mStructure[5], int mSeaLevel)
{
	int r, c, k;
	int ans = 0;

	vector<int> idx(4);
	for (int i = 0; i < (M - 1); i++) {
		idx[i] = mStructure[i + 1] - mStructure[0] + OFFSET;
	}

	if (M == 1) {
		for (int r = 0; r < islandSize; r++) {
			for (int c = 0; c < islandSize; c++) {
				putStructure(r, c, 0, M, mStructure);
				cnt = 0;
				for (int i = 0; i < islandSize; i++) {
					riseSeaLevel(0, i, mSeaLevel);
					riseSeaLevel(i, 0, mSeaLevel);
					riseSeaLevel(islandSize - 1, i, mSeaLevel);
					riseSeaLevel(i, islandSize - 1, mSeaLevel);
				}
				if ((islandSize * islandSize - cnt) > ans) {
					ans = islandSize * islandSize - cnt;
				}
				
			}
		}
		return ans;
	}


	if (hashStructure[idx[0]][idx[1]][idx[2]][idx[3]].empty()) { return -1; }

	
	for (auto region : hashStructure[idx[0]][idx[1]][idx[2]][idx[3]]) {
		r = region[0];
		c = region[1];
		k = region[2];
		putStructure(r, c, k, M, mStructure);
		
		cnt = 0;
		for (int i = 0; i < islandSize; i++) {
			riseSeaLevel(0, i, mSeaLevel);
			riseSeaLevel(i, 0, mSeaLevel);
			riseSeaLevel(islandSize - 1, i, mSeaLevel);
			riseSeaLevel(i, islandSize - 1, mSeaLevel);
		}

		if ((islandSize * islandSize - cnt) > ans) {
			ans = islandSize * islandSize - cnt;
		}
	}

	return ans;
}