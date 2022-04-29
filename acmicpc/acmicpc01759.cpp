#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int check[15];
int l, c;
vector <char> chars;

void dfs(int k){
    int summation = 0;
    for (int i=0; i < c; i++){
        summation += check[i];
    }
    if (summation == l){
        vector <char> ans;
        for (int i=0; i < c; i++){
            if (check[i] == 1){
                ans.push_back(chars[i]);
            }
        }

        int n_vowels = 0;
        int n_consonant = 0;
        for (int i=0; i < l; i++){
            if (ans[i] == 'a' || ans[i] == 'e' || ans[i] == 'i' || ans[i] == 'o' || ans[i] == 'u'){
                n_vowels++;
            } else {
                n_consonant++;
            }
        }

        if (n_vowels > 0 && n_consonant > 1){
            for (int i=0; i < l; i++){
                cout << ans[i];
            }
            cout << "\n";
        }
        return;
    }

    for (int i=k; i < c; i++){
        check[i] = 1;
        dfs(i+1);
        check[i] = 0;
    }
}

int main(){    
    cin >> l >> c;
    for (int i=0; i < c; i++){
        char temp;
        cin >> temp;
        chars.push_back(temp);
    }
    sort(chars.begin(), chars.end());

    dfs(0);

    return 0;
}
