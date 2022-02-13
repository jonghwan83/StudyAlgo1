#include <stdio.h>
#include <algorithm>

int main(){
    int N, M;
    scanf("%d", &N);
    long int A[N];
    for (int i=0; i < N; i++){
        scanf("%ld", &A[i]);
    }
    scanf("%d", &M);
    long int K[M];
    for (int i=0; i < M; i++){
        scanf("%ld", &K[i]);
    }

    std::sort(A, A + N);

    for (int i=0; i < M; i++){
        // code
        int P = N / 2;
        int L = 0;
        int R = N;
        while (true){
            if (A[R-1] < K[i] or A[L] > K[i]){
                printf("%d\n", 0);
                break;
            } else if (A[P] == K[i]){
                printf("%d\n", 1);
                break;
            } else{
                if (K[i] >= A[P]){
                    L = P;
                } else {
                    R = P;
                }
            }
            P = (R - L) / 2 + L;
        }
    }
    return 0;
}