#include <iostream>
#include <algorithm>

using namespace std;

int bsearch(int *s, int n, int k){
    int l, r, m, ans;
    l = 0;
    r = n-1;
    while (l <= r){
        m = (l + r)/2;
        if (s[m] < k){
            l = m + 1;
        } else if (s[m] > k){
            r = m - 1;
        } else {
            break;
        }
    }
    if (s[m] != k){
        return 0;
    }
    ans = 1;
    int i = 1;
    while (m - i >= 0 && m + i < n){
        if (s[m+i] == k && m+i < n){
            ans++;
        }
        if (s[m-i] == k && m-i >=0){
            ans++;
        }
        if (s[m+i] > k && s[m-i] < k){
            break;
        }
        i++;
    }
    return ans;
}

int main(){
    int n, m;

    cin >> n;
    
    int s[n];
    for (int i=0; i < n; i++){
        cin >> s[i];
    }

    sort(s, s+n);

    cin >> m;

    int j[m];
    int ans[m];
    for (int i=0; i < m; i++){
        cin >> j[i];
        ans[i] = bsearch(s, n, j[i]);
    }

    for (int i=0; i < m; i++){
        cout << ans[i] << " ";
    }
    cout << "\n";

    return 0;
}