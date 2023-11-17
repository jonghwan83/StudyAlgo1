
#define MAXN 100'000

#define SUBN 400

int min(int a, int b)
{
	if (a < b) { return a; }
	return b;
}

struct Highest
{
	int idx;
	int box;
};

int n;

int box[MAXN];
Highest highest[MAXN / SUBN];



int getLeftIdx(int loc)
{
	int height = 0;
	int ans = -1;
	int row = loc / SUBN;

	for (int i = loc % SUBN - 1; i >= 0; i--)
	{
		if (box[SUBN * row + i] > height)
		{
			height = box[SUBN * row + i];
			ans = SUBN * row + i;
		}
	}

	for (int i = row - 1; i >= 0; i--)
	{
		if (highest[i].box > height)
		{
			height = highest[i].box;
			ans = highest[i].idx;
		}
	}

	return ans;
}

int getRightIdx(int loc)
{
	int height = 0;
	int ans = -1;
	int row = loc / SUBN;

	int ed;
	if (row == n / SUBN) { ed = n % SUBN; }
	else { ed = SUBN; }

	for (int i = loc % SUBN + 1; i < ed; i++)
	{
		if (box[row * SUBN + i] > height)
		{
			height = box[row * SUBN + i];
			ans = row * SUBN + i;
		}
	}

	for (int i = row + 1; i <= n / SUBN; i++)
	{
		if (highest[i].box > height)
		{
			height = highest[i].box;
			ans = highest[i].idx;
		}
	}

	return ans;
}

int getHighestIdx()
{
	int height = 0;
	int ans = -1;

	for (int i = 0; i <= n / SUBN; i++)
	{
		if (highest[i].box > height)
		{
			height = highest[i].box;
			ans = highest[i].idx;
		}
	}

	return ans;
}

int getArea()
{
	int ans = 0;

	int idx1, idx2;
	idx1 = idx2 = getHighestIdx();

	ans += (box[idx1]);

	while (true)
	{
		int left = getLeftIdx(idx1);
		if (left == -1) { break; }
		ans += (idx1 - left) * min(box[idx1], box[left]);
		idx1 = left;
	}

	while (true)
	{
		int right = getRightIdx(idx2);
		if (right == -1) { break; }
		ans += (right - idx2) * min(box[idx2], box[right]);
		idx2 = right;
	}

	return ans;
}

void init(int N)
{
	n = N;

	for (int i = 0; i < N; i++)
	{
		box[i] = 0;
	}

	for (int i = 0; i <= N / SUBN; i++)
	{
		highest[i].idx = -1;
		highest[i].box = 0;
	}

	return;
}



int stock(int mLoc, int mBox)
{
	mLoc--;

	box[mLoc] += mBox;

	int row = mLoc / SUBN;

	if (box[mLoc] > highest[row].box)
	{
		highest[row].box = box[mLoc];
		highest[row].idx = mLoc;
	}


	return getArea();
}



int ship(int mLoc, int mBox)
{
	mLoc--;

	box[mLoc] -= mBox;

	int row = mLoc / SUBN;

	highest[row].box = 0;
	highest[row].idx = -1;

	int ed;
	if (row == n / SUBN) { ed = n % SUBN; }
	else { ed = SUBN; }

	for (int i = 0; i < ed; i++)
	{
		if (box[row * SUBN + i] > highest[row].box)
		{
			highest[row].box = box[row * SUBN + i];
			highest[row].idx = row * SUBN + i;
		}
	}


	return getArea();
}



int getHeight(int mLoc)
{
	mLoc--;

	return box[mLoc];
}

