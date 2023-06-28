
#include <iostream>

using namespace std;

int max(int a, int b) {
    if (a > b) { return a;}
    return b;
}

int N, K;
int W[101];
int V[101];
int dp[101][100001];


int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

//    freopen("sample_input.txt", "r", stdin);

    cin >> N >> K;

    for (int i = 1; i <= N; i++) {
        cin >> W[i] >> V[i];
    }


    for (int i = 1; i <= N; i++) {

        for (int j = 1; j <= K; j++) {
            if (j - W[i] >= 0) {
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - W[i]] + V[i]);
            }
            else {
                dp[i][j] = dp[i - 1][j];
            }
        }

    }

    cout << dp[N][K];

}