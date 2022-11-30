#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

#define CMD_INIT 100
#define CMD_UPDATE_SCORE 200
#define CMD_UNION_TEAM 300
#define CMD_GET_SCORE 400

extern void init(int N);
extern void updateScore(int mWinnerID, int mLoserID, int mScore);
extern void unionTeam(int mPlayerA, int mPlayerB);
extern int getScore(int mID);

static bool run()
{
	int queryCnt, cmd;
	int ans, res;
	bool okay = false;

	scanf("%d", &queryCnt);
	for (int i = 1; i < queryCnt + 1; i++)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			int N;
			scanf("%d", &N);
			init(N);
			okay = true;
			break;

		case CMD_UPDATE_SCORE:
			int mWinnerID, mLoserID, mScore;
			scanf("%d%d%d", &mWinnerID, &mLoserID, &mScore);
			updateScore(mWinnerID, mLoserID, mScore);
			break;

		case CMD_UNION_TEAM:
			int mPlayerA, mPlayerB;
			scanf("%d%d", &mPlayerA, &mPlayerB);
			unionTeam(mPlayerA, mPlayerB);
			break;

		case CMD_GET_SCORE:
			int mID;
			scanf("%d", &mID);
			res = getScore(mID);
			scanf("%d", &ans);
			if (ans != res)
			{
				okay = false;
			}
			break;
		}
	}

	return okay;
}



int main()
{
	clock_t start, end;
	start = clock();

	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d%d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	end = clock() - start;
	printf("elpased: %f\n", (float)end / CLOCKS_PER_SEC);
	return 0;
}
