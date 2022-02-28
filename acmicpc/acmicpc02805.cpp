#include <iostream>

using namespace std;
int main(){
    long long int n, m;
    scanf("%lld", &n);
    scanf("%lld", &m);

    long long int left = 0;
    long long int right = 0;

    long long int trees[n];
    for (int i=0; i < n; i++){
        scanf("%lld", &trees[i]);
        if (right < trees[i]){
            right = trees[i];
        }
    }

    long long int output, target;
    output = (left + right) / 2;
    while (left <= right){
        target = 0;
        for (int i=0; i < n; i++){
            if (trees[i] - output > 0){
                target = target + (trees[i] - output);
            }
        }

        if (target >= m){
            left = output + 1;
        } else if (target < m){
            right = output - 1;
        }
        output = (left + right) / 2;
    }
    cout << output << "\n";

    return 0;
}