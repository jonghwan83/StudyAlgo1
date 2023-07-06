
#include <iostream>

using namespace std;

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

void run(int coins[20], int n, int m, int dp[21][10001]) {

    for (int i = 1; i <= n; i++) {

        for (int j = 1; j <= m; j++) {

            dp[i][j] = dp[i - 1][j];
            if (j - coins[i] >= 0) {
                dp[i][j] += dp[i][j - coins[i]];
            }


        }
    }

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    freopen("sample_input.txt", "r", stdin);

    int T, N, M;
    int dp[21][10001];

    int coins[20];

    cin >> T;

    for (int i = 0; i < T; i++) {
        cin >> N;

        for (int j = 1; j <= N; j++) {
           cin >> coins[j];
           dp[j][0] = 1;
        }

        cin >> M;

        run(coins, N, M, dp);

        cout << dp[N][M] << "\n";
    }


    return 0;
}