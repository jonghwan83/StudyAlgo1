#define MAXBRAND 51
#define MAXHOTEL 5001
#define INF 50001

class HeapNode {
public:
    int dist;
    int id;
};

class Heap {
public:
    int length;
    HeapNode arr[MAXHOTEL];

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (arr[parent].dist > arr[child].dist) { return true;}
        return false;
    }

    void push(int d, int i) {
        HeapNode last;
        last.dist = d; last.id = i;

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    HeapNode pop() {
        HeapNode ans = arr[0];

        arr[0] = arr[--length];
        
        int idx = 0;
        int left, right, child;

        while (2 * idx + 1 < length) {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                HeapNode temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }
        return ans;
    }
};

class Node {
public:
    int data;
    Node* next;
};

class LinkedList {
public:
    int length;
    Node* head;
    Node* tail;

    void init() {
        length = 0;
        head = nullptr;
        tail = nullptr;
    }

    void push(int a) {
        Node* node = new Node();
        node->data = a;

        node->next = head;
        if (!tail) { tail = node; }
        head = node;
        length++;
    }
};

class Hotel {
public:
    int brand;
    LinkedList dist;
    LinkedList nextHotel;

    void init() {
        dist.init();
        nextHotel.init();
    }
};


int n;
LinkedList brands[MAXBRAND];
Hotel hotels[MAXHOTEL];

int minDist[MAXHOTEL];
int currHotel;

void getMinDist(int st, int brand, int nHotel) {
    Heap pQueue; pQueue.init();

    for (int i = 0; i < n; i++) {
        minDist[i] = INF;
    }

    pQueue.push(0, st);
    minDist[st] = 0;

    while (pQueue.length > 0) {
        HeapNode curr = pQueue.pop();

        if (minDist[curr.id] < curr.dist) { continue; }
        if (hotels[curr.id].brand == brand && curr.id != nHotel && curr.id != st) {
            currHotel = curr.id;
            break;
        }

        Node* dist = hotels[curr.id].dist.head;
        Node* nextHotel = hotels[curr.id].nextHotel.head;

        while (nextHotel) {

            if (minDist[curr.id] + dist->data < minDist[nextHotel->data]) {
                minDist[nextHotel->data] = minDist[curr.id] + dist->data;
                pQueue.push(minDist[nextHotel->data], nextHotel->data);
            }


            dist = dist->next;
            nextHotel = nextHotel->next;
        }
    }
}


void assembleLink(int idx1, int idx2) {
    Node* node = brands[idx2].head;

    while (node) {
        hotels[node->data].brand = idx1;
        node = node->next;
    }

    brands[idx1].tail->next = brands[idx2].head;
    brands[idx1].tail = brands[idx2].tail;
    brands[idx1].length += brands[idx2].length;
}

void init(int N, int mBrands[])
{
    n = N;

    for (int i = 0; i < MAXBRAND; i++) {
        brands[i].init();
    }

    for (int i = 0; i < N; i++) {
        hotels[i].init();
        hotels[i].brand = mBrands[i];
        brands[mBrands[i]].push(i);
    }
}



void connect(int mHotelA, int mHotelB, int mDistance)
{
    hotels[mHotelA].nextHotel.push(mHotelB);
    hotels[mHotelA].dist.push(mDistance);

    hotels[mHotelB].nextHotel.push(mHotelA);
    hotels[mHotelB].dist.push(mDistance);

}



int merge(int mHotelA, int mHotelB)
{
    int brandA = hotels[mHotelA].brand;
    int brandB = hotels[mHotelB].brand;

    if (brandA != brandB) {
        assembleLink(brandA, brandB);
    }

    return brands[brandA].length;
}



int move(int mStart, int mBrandA, int mBrandB)
{

    getMinDist(mStart, mBrandA, mStart);
    int hotelA = currHotel;
    int distA = minDist[hotelA];

    getMinDist(mStart, mBrandB, hotelA);
    int hotelB = currHotel;
    int distB = minDist[hotelB];

    return distA + distB;
}
