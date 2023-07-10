
#include <iostream>

using namespace std;

int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}

int abs(int a) {
    if (a < 0) { return -a; }
    return a;
}

int N;
bool dp[31][40001];
int weights[31];

void run(int i, int w) {
    if (i > N || dp[i][w]) { return; }
    dp[i][w] = true;
    run(i + 1, w + weights[i]);
    run(i + 1, abs(w - weights[i]));
    run(i + 1, w);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

//    freopen("sample_input.txt", "r", stdin);


    cin >> N;
    for (int i = 0; i < N; i++) {
        cin >> weights[i];
    }

    int M;
    int marbles[40001];

    cin >> M;
    for (int i = 1; i <= M; i++) {
        cin >> marbles[i];
    }

    run(0, 0);

    for (int i = 1;i <= M; i++) {
        if (dp[N][marbles[i]]) {
            cout << "Y ";
        }
        else { cout << "N "; }
    }

    return 0;
}