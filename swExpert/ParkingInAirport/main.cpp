#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

#define CMD_INIT        100
#define CMD_PARK        200
#define CMD_RETRIEVE    300
#define CMD_BUY         400
#define CMD_EVENT       500
#define CMD_INSPECT     600

#define MAX_N           10

extern void init(int N, int mCapacity[]);
extern int park(int mDate, int mID, int mCompany, int mPeriod);
extern int retrieve(int mDate, int mID);
extern void buy(int mID, int mPeriod);
extern void event(int mCompany, int mPeriod);
extern int inspect(int mDate, int mCompany);

static bool run()
{
	int Q;
	int N, mID, mDate, mCompany, mPeriod;
	int mCapacity[MAX_N];
	
	int ret, ans;

	scanf("%d", &Q);

	bool okay = false;

	for (int q = 0; q < Q; ++q)
	{
		int cmd;
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d", &N);
			for (int i = 0; i < N; i++) { scanf("%d", &mCapacity[i]); }
			init(N, mCapacity);
			okay = true;
			break;
		case CMD_PARK:
			scanf("%d %d %d %d", &mDate, &mID, &mCompany, &mPeriod);
			ret = park(mDate, mID, mCompany, mPeriod);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_RETRIEVE:
			scanf("%d %d", &mDate, &mID);
			ret = retrieve(mDate, mID);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_BUY:
			scanf("%d %d", &mID, &mPeriod);
			buy(mID, mPeriod);
			break;
		case CMD_EVENT:
			scanf("%d %d", &mCompany, &mPeriod);
			event(mCompany, mPeriod);
			break;
		case CMD_INSPECT:
			scanf("%d %d", &mDate, &mCompany);
			ret = inspect(mDate, mCompany);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
	}

	return okay;
}

int main()
{
	clock_t start = clock();

	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int TC, MARK;

	scanf("%d %d", &TC, &MARK);
	for (int tc = 1; tc <= TC; ++tc)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	printf("elapsed: %f \n", (float) (clock() - start) / CLOCKS_PER_SEC);

	return 0;
}
