#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define MAXV 100001

vector< pair<int, int> > v;

int dp[MAXV];

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	//freopen("sample_input.txt", "r", stdin);

	int c, n;
	
	dp[0] = 0;
	cin >> c >> n;


	for (int i = 0; i < n; i++) {
		int price, customer;
		cin >> price >> customer;
		v.push_back(make_pair(price, customer));
	}

	for (int i = 0; i < n; i++) {

		for (int j = 1; j < MAXV; j++) {
			if (j - v[i].first >= 0) {
				dp[j] = max(dp[j], dp[j - v[i].first] + v[i].second);
			}
		}
	}

	for (int i = 0; i < MAXV; i++) {
		if (dp[i] >= c) {
			cout << i;
			return 0;
		}
	}

}