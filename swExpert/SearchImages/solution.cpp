#define MAX_N 10000
#define MAX_M 10

class HeapNode {
public:
    int pixels;
    int id;
};

class Heap {
public:
    int length;
    HeapNode arr[MAX_N + 1];

    void init() { length = 0; }
    bool compare(int parent, int child) {
        if (arr[parent].pixels < arr[child].pixels) { return true; }
        if (arr[parent].pixels == arr[child].pixels && arr[parent].id > arr[child].id) { return true; }
        return false;
    }

    void push(int pixels, int id) {
        HeapNode last;
        last.pixels = pixels; last.id = id;

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }
};

class Image {
public:
    int pixels[MAX_M][MAX_M];
};

int pIdx;
int imageSize;
Image images[MAX_N + 1];

int validPixels(int idx, char mImage[MAX_M][MAX_M]) {
   int notValid = 0;

   for (int i = 0; i < imageSize; i++) {
       for (int j = 0; j < imageSize; j++) {
           if (images[idx].pixels[i][j] != mImage[i][j]) {
               notValid++;
           }

           if (notValid > 2) {
               return 0;
           }
       }
   }

   return imageSize - notValid;
}

void init(int N, int M, char mImageList[MAX_N][MAX_M][MAX_M])
{
    pIdx = 1;
    imageSize = M;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < M; k++) {
                images[pIdx].pixels[j][k] = mImageList[i][j][k];
            }
        }
        pIdx++;
    }
}

int findImage(char mImage[MAX_M][MAX_M])
{
    Heap pQueue;
    pQueue.init();

    for (int i = 1; i < pIdx; i++) {
        int n = validPixels(i, mImage);
        if (n > 0) {
            pQueue.push(n, i);
        }
    }

    return pQueue.arr[0].id;
}