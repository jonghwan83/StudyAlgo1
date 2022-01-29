#include <stdio.h>
#include <stack>

int main(){
    std::stack<int> arr;
    unsigned int n;
    scanf("%d", &n);
    unsigned int temp;
    for (int i=0; i < n; i++){
        scanf("%d", &temp);
        if (temp == 0){
            arr.pop();
        } else {
            arr.push(temp);
        }
    }
    long long int sum = 0;
    int size = arr.size();
    for (int i=0; i < size; i++){
        sum += arr.top();
        arr.pop();
    }
    printf("%lld\n", sum);
}