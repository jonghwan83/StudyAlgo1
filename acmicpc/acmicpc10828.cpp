#include <iostream>
#include <cstring>

class Stack
{
private:
    int length;
    int arr[10000];
public:
    Stack() {
        length = 0;
    }
    void push(int a){
        arr[length] = a;
        length += 1;
    }
    int pop(){
        if (length == 0){ 
            return -1;
        }
        length -= 1;
        return arr[length];
    }
    int size(){
        return length;
    }
    int empty(){
        if (length == 0){
            return 1;
        }
        return 0;
    }
    int top(){
        if (length == 0){
            return -1;
        }
        return arr[length-1];
    }
};

int main(){
    Stack stack;

    int T, x;
    char command[8];
    scanf("%d", &T);
    for (int i=0; i < T; i++){
        scanf("%s", command);

        if (strcmp(command, "push") == 0){
            scanf("%d", &x);
            stack.push(x);
        } else if (strcmp(command, "pop") == 0){
            printf("%d\n", stack.pop());
        } else if (strcmp(command, "size") == 0){
            printf("%d\n", stack.size());
        } else if (strcmp(command, "empty") == 0){
            printf("%d\n", stack.empty());
        } else if (strcmp(command, "top") == 0){
            printf("%d\n", stack.top());
        }
    }
}