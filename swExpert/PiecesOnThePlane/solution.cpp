#define MAXLINE 100000
#define INF 1000000000

int H = 0;
int V = 1;
int D = 2;

class Data {
public:
    int id;
    int point;
};

class Line {
public:
    int length;
    Data arr[MAXLINE + 2];

    void init() {
        length = 0;
    }

    void push(int id, int p) {
        arr[length].id = id;
        arr[length].point = p;
        length++;
    }
};

void merge(Data arr[], int left, int right) {
    int n = right - left + 1;
    int mid = (left + right) / 2;

    Data temp[MAXLINE + 2];
    int k = left;
    int i = left; int j = mid + 1;

    while (k < left + n) {
        if (i <= mid && j <= right) {
            if (arr[i].point < arr[j].point) {
                temp[k] = arr[i];
                i++;
            }
            else {
                temp[k] = arr[j];
                j++;
            }
        }
        else if (i <= mid) {
            temp[k] = arr[i];
            i++;
        }
        else {
            temp[k] = arr[j];
            j++;
        }

        k++;
    }

    for (int i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

void mergeSort(Data arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, right);
    }
}

int binarySearch(Data arr[], int n, int target, int offset) {
    int left = 0;
    int right = n - 1;

    while (left < right) {
        int mid = (left + right) / 2;
        if (arr[mid].point + offset < target) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }

    return right;
}

Line lines[3];

void init(int N, int mId[], int mType[], int mX[], int mY[])
{
    for (int i = 0; i < 3; i++) {
        lines[i].init();
    }

    for (int i = 0; i < N; i++) {
        if (mType[i] == H) {
            lines[H].push(mId[i], mY[i]);
        }
        else if (mType[i] == V) {
            lines[V].push(mId[i], mX[i]);
        }
        else {
            lines[D].push(mId[i], mY[i] - mX[i]);
        }
    }

    for (int i = 0; i < 3; i++) {
        lines[i].push(0, -INF - 1);
        lines[i].push(0, INF + 1);
        mergeSort(lines[i].arr, 0, lines[i].length - 1);
    }
}

int findPiece(int mInteriorX, int mInteriorY)
{
    int ans = 0;

    int points[3][2];

    points[H][1] = binarySearch(lines[H].arr, lines[H].length, mInteriorY, 0);
    points[H][0] = points[H][1] - 1;
    points[V][1] = binarySearch(lines[V].arr, lines[V].length, mInteriorX, 0);
    points[V][0] = points[V][1] - 1;
    points[D][1] = binarySearch(lines[D].arr, lines[D].length, mInteriorY, mInteriorX);
    points[D][0] = points[D][1] - 1;

    for (int i = 0; i < 2; i++) {
        ans += lines[i].arr[points[i][0]].id;
        ans += lines[i].arr[points[i][1]].id;
    }

    if (lines[D].arr[points[D][0]].point + lines[V].arr[points[V][1]].point > lines[H].arr[points[H][0]].point) {
        ans += lines[D].arr[points[D][0]].id;
    }

    if (lines[D].arr[points[D][0]].point + lines[V].arr[points[V][1]].point >= lines[H].arr[points[H][1]].point) {
        ans -= lines[V].arr[points[V][1]].id;
    }

    if (lines[H].arr[points[H][0]].point - lines[D].arr[points[D][0]].point <= lines[V].arr[points[V][0]].point) {
        ans -= lines[H].arr[points[H][0]].id;
    }

    if (lines[D].arr[points[D][1]].point + lines[V].arr[points[V][0]].point < lines[H].arr[points[H][1]].point) {
        ans += lines[D].arr[points[D][1]].id;
    }

    if (lines[D].arr[points[D][1]].point + lines[V].arr[points[V][0]].point <= lines[H].arr[points[H][0]].point) {
        ans -= lines[V].arr[points[V][0]].id;
    }

    if (lines[H].arr[points[H][1]].point - lines[D].arr[points[D][1]].point >= lines[V].arr[points[V][1]].point) {
        ans -= lines[H].arr[points[H][1]].id;
    }

    return ans;
}