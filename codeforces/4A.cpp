#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* solution(int w){
    char* arr = (char*) malloc(10);
    if (w <= 2){
        strcpy(arr, "NO");
        return arr;
    }
    for (int i=0; i < w/2; i++){
        if ((w - (2*i)) % 2 == 0){
            strcpy(arr, "YES");
            return arr;
        }
    }
    strcpy(arr, "NO");
    return arr;
}

int main(){
    int w;
    scanf("%d", &w);
    printf("%s\n", solution(w));
    return 0;
}