#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

extern void init(int N, int K, int sCity[], int eCity[], int mToll[]);
extern void add(int sCity, int eCity, int mToll);
extern void calculate(int sCity, int eCity, int M, int mIncrease[], int mRet[]);

/////////////////////////////////////////////////////////////////////////

#define MAX_E 300
#define MAX_I 20000
#define CMD_INIT 100
#define CMD_ADD 200
#define CMD_CALC 300

static bool run() {
	int q;
	scanf("%d", &q);

	int n, m, k;
	char strTmp[30];
	int sCityArr[MAX_E], eCityArr[MAX_E], mTollArr[MAX_E], mIncrease[MAX_I], mRet[MAX_I + 1];
	int sCity, eCity, mToll;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d %s", &cmd, strTmp);
		switch (cmd) {
			case CMD_INIT:
				okay = true;
				scanf("%s %d %s %d", strTmp, &n, strTmp, &k);
				for (int j = 0; j < k; ++j) {
					scanf("%s %d %s %d %s %d", strTmp, &sCityArr[j], strTmp, &eCityArr[j], strTmp, &mTollArr[j]);
				}
				init(n, k, sCityArr, eCityArr, mTollArr);
				break;
			case CMD_ADD:
				scanf("%s %d %s %d %s %d", strTmp, &sCity, strTmp, &eCity, strTmp, &mToll);
				add(sCity, eCity, mToll);
				break;
			case CMD_CALC:
				scanf("%s %d %s %d %s %d", strTmp, &sCity, strTmp, &eCity, strTmp, &m);
				for (int j = 0; j < m; ++j) {
					scanf("%s %d", strTmp, &mIncrease[j]);
				}
				calculate(sCity, eCity, m, mIncrease, mRet);
				scanf("%s", strTmp);
				for (int j = 0; j <= m; ++j) {
					scanf("%d", &ans);
					if (ans != mRet[j])
						okay = false;
				}
				break;
			default:
				okay = false;
				break;
		}
	}
	return okay;
}

int main() {
	clock_t start = clock();

	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	clock_t end = clock();
	printf("elapsed time: %f sec\n", (float)(end - start) / CLOCKS_PER_SEC);

	return 0;
}