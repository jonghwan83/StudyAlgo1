#include <stdio.h>
#include <stdlib.h>

class Queue{
private:
    int right;
    int left;
    int arr[1000000][2];
public:
    Queue(){
        right = 0;
        left = 0;
    }
    void push(int a, int b){
        arr[right][0] = a;
        arr[right][1] = b;
        right += 1;
    }
    int* pop(){
        static int temp[2];
        temp[0] = arr[left][0];
        temp[1] = arr[left][1];
        left += 1;

        return temp;
    }
    int getsize(){
        return right - left;
    }
};

int main(){
    Queue queue;
    int m, n;
    scanf("%d", &m);
    scanf("%d", &n);
    
    
    int *box[1001];
    for (int i=0; i < n; i++){
        box[i] = (int *)malloc(sizeof(int) * 1001);
        for (int j=0; j < m; j++){
            scanf("%d", &box[i][j]);
            if (box[i][j] == 1){
                queue.push(i, j);
            } 
        }
    }

    int dis[4] = {-1, 1, 0, 0};
    int djs[4] = {0, 0, -1, 1};
    int days = -1;

    int* now;
    int ni, nj, len_queue;
    while (queue.getsize()){
        days += 1;

        len_queue = queue.getsize();
        for (int k=0; k < len_queue; k++){
            now = queue.pop();
            for (int i=0; i < 4; i++){
                ni = now[0] + dis[i];
                nj = now[1] + djs[i];

                if ((ni >= 0) && (ni < n) && (nj >= 0) && (nj < m) && (box[ni][nj] == 0)){
                    box[ni][nj] = 1;
                    queue.push(ni, nj);
                }
            }
        }
    }

    for (int i=0; i < n; i++){
        for (int j=0; j < m; j++){
            if (box[i][j] == 0){
                printf("%d\n", -1);
                return 0;
            }
        }
    }
    printf("%d\n", days);
    return 0;
}