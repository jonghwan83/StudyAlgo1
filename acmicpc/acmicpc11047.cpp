#include <iostream>

int main()
{
    int N, K;
    scanf("%d %d", &N, &K);
    int arr[N];
    
    for (int i=0; i < N; i++){
        scanf("%d", &arr[i]);
    }
    int t[N];
    for (int i=N-1; i >= 0; i--){
        t[i] = int(K / arr[i]);
        K = K - t[i] * arr[i];
    }
    int sum = 0;
    for (int i=0; i < N; i++){
        sum = sum + t[i];
    }
    printf("%d\n", sum);
}