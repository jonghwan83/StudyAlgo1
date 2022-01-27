#include <iostream>

int main() {
    int n;

    scanf("%d", &n);

    long long int cities[n], prices[n];

    for (int i=0; i < n-1; i++){
        scanf("%lld", &cities[i]);
    }

    for (int i=0; i < n; i++){
        scanf("%lld", &prices[i]);
    }
    
    long long int travel = cities[0] * prices[0];
    long long int price = prices[0];

    for (int i=1; i < n-1; i++){
        if (prices[i] < price){
            price = prices[i];
        }
        travel = travel + (price * cities[i]);
    }
    printf("%lld\n", travel);
}