
#include <iostream>

using namespace std;

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}


int main() {
    int N, M;
    int m[101];
    int c[101];
    int dp[101][10001];

    int maxCost = 0;

    int ans = 10001;

    ios::sync_with_stdio(false);
    cin.tie(NULL);

//    freopen("sample_input.txt", "r", stdin);


    cin >> N >> M;

    for (int i = 1; i <= N; i++) {
        cin >> m[i];
    }

    for (int i = 1; i <= N; i++) {
        cin >> c[i];
        maxCost += c[i];
    }

    for (int i = 1; i <= N; i++) {

        for (int j = 0; j <= maxCost; j++) {
            if (j - c[i] >= 0) {
                dp[i][j] = max(dp[i-1][j], dp[i-1][j - c[i]] + m[i]);
            }

            else  {
                dp[i][j] = max(dp[i-1][j], dp[i][j]);
            }

            if (dp[i][j] >= M) {
                ans = min(ans, j);

            }
        }
    }

    printf("%d", ans);


    return 0;
}