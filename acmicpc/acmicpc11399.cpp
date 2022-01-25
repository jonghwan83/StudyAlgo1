#include <iostream>
#include <algorithm>

using namespace std;

int main(){

    int N;
    scanf("%d", &N);

    int arr[N];
    for (int i=0; i < N; i++){
        scanf("%d", &arr[i]);
    }
    sort(arr, arr + N);

    int total = 0;
    for (int i=0; i < N; i++) {
        total = total + (N - i) * arr[i];
    }
    printf("%d\n", total);
}