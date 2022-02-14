#include <stdio.h>

class Queue{
    public:
        int arr[100001];
        int size;
        Queue(){
            size = 0;
        }
        int pop(){
            int temp = arr[0];
            size -= 1;
            for (int i=0; i < size; i++){
                arr[i] = arr[i+1];
            }
            return temp;
        }
        void push(int a){
            arr[size] = a;
            size += 1;
        }
};

int main(){
    int N, K;
    scanf("%d", &N);
    scanf("%d", &K);

    int visited[100001];
    Queue queue;

    queue.push(N);

    while (true)
    {
        int temp = queue.pop();
        if (temp == K){
            printf("%d\n", visited[temp]);
            break;
        }
        int next[3] = {temp + 1, temp - 1, temp * 2};
        for (int i=0; i < 3; i++){
            if ((0 <= next[i]) && (next[i] <= 100000) && (visited[next[i]] == 0)){
                visited[next[i]] = visited[temp] + 1;
                queue.push(next[i]);
            }
        }
    }
    return 0;
}