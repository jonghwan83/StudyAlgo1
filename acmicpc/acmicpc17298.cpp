#include <stdio.h>

class Stack {
    private:
        int length;
        int arr[1000000];
    public:
        Stack(){
            length = 0;
        }
    void push(int a){
        arr[length] = a;
        length += 1;
    }
    int pop(){
        if (length != 0){
            length -= 1;
            return arr[length];
        } else {
            return -1;
        }
    }
    int top(){
        if (length != 0){
            return arr[length-1];
        } else {
            return -1;
        }
    }
    int size(){
        return length;
    }
};
int main(){
    int N;
    scanf("%d", &N);

    int A[N];
    int result[N];

    for (int i=0; i < N; i++){
        scanf("%d", &A[i]);
        result[i] = -1;
    }

    Stack stack;

    for (int i=N-1; i >= 0; i--){
        while (stack.top() <= A[i]){
            stack.pop();
            if (stack.size() < 1){
                break;
            }
        }
        
        if (stack.size() > 0){
            if (A[i] > stack.top()){
                result[i] = A[i];
            } else {
                result[i] = stack.top();
            }
        }
        stack.push(A[i]);
    }

    for (int i=0; i < N-1; i++){
        printf("%d ", result[i]);
    }
    printf("%d\n", result[N-1]);
    return 0;
}