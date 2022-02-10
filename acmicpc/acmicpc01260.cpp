#include <stdio.h>
#include <stdlib.h>

class Graph
{
private:
    void swap(int *a, int *b){
        int t = *a;
        *a = *b;
        *b = t;
    }
    int partition(int arr[], int low, int high){
        int pivot = arr[high];
        int i = low - 1;

        for (int j=low; j < high; j++){
            if (arr[j] <= pivot){
                i += 1;
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i+1], &arr[high]);
        return (i + 1);
    }
    void quicksort(int arr[], int low, int high){
        if (low < high){
            int pi = partition(arr, low, high);
            quicksort(arr, low, pi - 1);
            quicksort(arr, pi + 1, high);
        }
    }
    int pop(){
        if (qsize < 0){
            return -1;
        } else {
            int a = queue[0];
            for (int i=0; i<qsize-1; i++){
                queue[i] = queue[i+1];
            }
            qsize -= 1;
            return a;
        }
    }
    void push(int a){
        queue[qsize] = a;
        qsize += 1;
    }

public:
    int vertices[1001];
    int *adjacents[1001];
    int len_adj[1001];
    bool visited[1001];

    int index, index2, qsize;
    int *dfs;
    int *bfs;
    int *queue;
    Graph(){
        for (int i=0; i < 1001; i++){
            len_adj[i] = 0;
            adjacents[i] = (int *)malloc(sizeof(int) * 10000);
            visited[i] = false;
        }

        index = 0;
        index2 = 0;
        qsize = 0;
        dfs = (int *)malloc(sizeof(int) * 10000);
        bfs = (int *)malloc(sizeof(int) * 10000);
        queue = (int *)malloc(sizeof(int) * 10000);
    }

    void add_adj(int a, int b){
        adjacents[a][len_adj[a]] = b;
        len_adj[a] += 1;
        adjacents[b][len_adj[b]] = a;
        len_adj[b] += 1;
    }

    void sort(int N){
        for (int i=1; i < (N+1); i++){
            quicksort(adjacents[i], 0, len_adj[i] - 1);
        }
    }

    void dfs_traverse(int vertex){
        dfs[index] = vertex;
        visited[vertex] = true;
        index += 1;
        for (int i=0; i < len_adj[vertex]; i++){
            if (visited[adjacents[vertex][i]] == false){
                dfs_traverse(adjacents[vertex][i]);
            }
        }
    }

    void bfs_traverse(int vertex){
        bool visited[1001];
        for (int i=0; i<1001; i++){
            visited[i] = false;
        }
        visited[vertex] = true;
        push(vertex);

        while (qsize > 0){
            int current = pop();
            bfs[index2] = current;
            index2 += 1;
            visited[current] = true;
            for (int j=0; j < len_adj[current]; j++){
                if (visited[adjacents[current][j]] == false){
                    visited[adjacents[current][j]] = true;
                    push(adjacents[current][j]);
                }
            }
        }
        
    }
};

int main(){
    int N, M, V;
    scanf("%d", &N);
    scanf("%d", &M);
    scanf("%d", &V);
    
    Graph graph;

    for (int i=0; i < M; i++){
        int a, b;
        scanf("%d", &a);
        scanf("%d", &b);
        graph.add_adj(a, b);
    }

    graph.sort(N);
    
    graph.dfs_traverse(V);
    for (int i=0; i < graph.index - 1; i++){
        printf("%d ", graph.dfs[i]);
    }
    printf("%d\n", graph.dfs[graph.index - 1]);

    graph.bfs_traverse(V);
    for (int j=0; j < graph.index2 - 1; j++){
        printf("%d ", graph.bfs[j]);
    }
    printf("%d\n", graph.bfs[graph.index2 - 1]);

    return 0;
}