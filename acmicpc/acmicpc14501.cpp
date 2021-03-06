#include <iostream>

using namespace std;

int max(int a, int b, int c){
    if (a >= b && a >= c){
        return a;
    } else if (c >= a && c >= b){
        return c;
    } else {
        return b;
    }
}

int main(){
    int n;
    cin >> n;

    int Ti[16];
    int Pi[16];
    for (int i=0; i < n; i++){
        cin >> Ti[i] >> Pi[i];
    }

    int dp[16];

    for (int i=0; i < 17; i++){
        dp[i] = 0;
    }

     for (int i=0; i < n; i++){
         if (i + Ti[i] <= n) {
             dp[i] = Pi[i];
             for (int j = 0; j <= i; j++) {
                 if (j + Ti[j] <= i) {
                     dp[i] = max(dp[j] + Pi[i], Pi[i], dp[i]);
                 }
             }
         }
     }

     int ans = 0;
     for (int i=0; i < n; i++){
         if (dp[i] > ans){
             ans = dp[i];
         }
     }

     cout << ans << "\n";

    return 0;
}