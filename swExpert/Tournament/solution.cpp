extern void swapOrder(int mIndexA, int mIndexB);

#define MAXNUM 131072

int answer[MAXNUM];
int arr[MAXNUM];
int hashIdx[MAXNUM + 1];

void merge(int arr[], int left, int right) {
    int n = right - left + 1;
    int mid = (right + left) / 2;

    int temp[n];
    int i = 0;
    while (i < n) {
        if (i % 2 == 0) {
            temp[i] = arr[left + i / 2];
        }
        else {
            temp[i] = arr[mid + 1 + i / 2];
        }
        i++;
    }

    for (int i = left; i <= right; i++) {
        arr[i] = temp[i - left];
    }
}

void merge_sort(int arr[], int left, int right) {
    int mid;

    if (left < right) {
        mid = (left + right) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, right);
    }
}

void modifyTournament(int N, int mOrder[])
{
    bool checked[MAXNUM];
    for (int i = 0; i < N; i++) {
        arr[i] = mOrder[i];
        hashIdx[mOrder[i]] = i;
        answer[i] = i + 1;
        checked[i] = false;
    }

    merge_sort(answer, 0, N - 1);

    for (int i = 0; i < N; i++) {
        if (arr[i] != answer[i]) {
            int old = arr[i];
            arr[i] = arr[hashIdx[answer[i]]];
            arr[hashIdx[answer[i]]] = old;

            swapOrder(i, hashIdx[answer[i]]);

            int temp = hashIdx[old];
            hashIdx[old] = hashIdx[answer[i]];
            hashIdx[answer[i]] = temp;

        }
    }
}