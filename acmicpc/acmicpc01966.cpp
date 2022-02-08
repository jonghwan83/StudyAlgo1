#include <stdio.h>

class Queue {
    private:
        int length;
        int arr[100];
    public:
        Queue(){
            length = 0;
        }
        int pop(){
            if (length > 0){
                int temp = arr[0];
                length -= 1;
                for (int i=0; i < length; i++){
                    arr[i] = arr[i+1];
                }
                return temp;
            } else {
                return 0;
            }
        }
        void push(int a){
            arr[length] = a;
            length += 1;
        }
        int size(){
            return length;
        }
        int max(){
            int temp = arr[0];
            for (int i=0; i < length; i++){
                if (arr[i] > temp){
                    temp = arr[i];
                }
            }
            return temp;
        }
};

int main(){
    int T, n, m;
    scanf("%d", &T);
    for (int i=0; i < T; i++){
        scanf("%d", &n);
        scanf("%d", &m);
        
        Queue queue;
        int a;
        for (int i=0; i < n; i++){
            scanf("%d", &a);
            queue.push(a);
        }

        int printed[n];
        int index, temp;
        index = 0;
        while (true){
            temp = queue.pop();
            m -= 1;
            if (queue.size() == 0){
                printed[index] = temp;
                index += 1;
                break;
            }
            if (temp < queue.max()){
                queue.push(temp);
                if (m < 0){
                    m = queue.size() - 1;
                }
            } else {
                printed[index] = temp;
                index += 1;
                if (m < 0){
                    break;
                }
            }
        }
        printf("%d\n", index);
    }

    return 0;
}