#include <unordered_map>

#define MAXORDER 30000
#define MAXPRODUCT 600

using namespace std;

class Data {
public:
    int price;
    int id;
    Data () {}
    Data(int p, int i) {
        price = p;
        id = i;
    }
};

class Heap {
public:
    Data arr[MAXPRODUCT];
    int length;

    Heap() {
        length = 0;
    }

    void init() {
        length = 0;
    }

    bool compare(int parent, int child) {
        if (arr[parent].price > arr[child].price) { return true; }
        if (arr[parent].price == arr[child].price && arr[parent].id > arr[child].id) { return true; }
        return false;
    }

    void push(int p, int i) {
        Data last = Data(p, i);
        arr[length] = last;
        int idx = length;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            Data temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
        length++;
    }

    Data pop() {
        Data ans = arr[0];
        length--;
        arr[0] = arr[length];

        int idx = 0;
        int left, right, child;
        while (idx * 2 + 1 < length) {
            left = idx * 2 + 1;
            right = idx * 2 + 2;

            if (right < length) {
                if (compare(left, right)) {child = right;}
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                Data temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else {
                break;
            }
        }
        return ans;
    }
};

class BuyOrder {
public:
    int id;
    int price;
    int product;
    int initQuantity;
    int quantity;
    int pIdx;
    bool isCanceled;
};

class SellOrder {
public:
    int id;
    int product;
    int bIdxs[MAXPRODUCT];
    int quantity[MAXPRODUCT];
    int length;
    bool isRefunded;
};

class Product {
public:
    int product;
    int total;
    Heap minQueue;

    Product() {
        product = 0;
    }
};

int bIdx, sIdx, pIdx;
BuyOrder buyOrder[MAXORDER];
SellOrder sellOrder[MAXORDER];
Product products[MAXPRODUCT];
unordered_map<int, int> hashProduct;
unordered_map<int, int> hashBuyOrder;
unordered_map<int, int> hashSellOrder;

void init() {
    hashProduct.clear();
    hashBuyOrder.clear();
    hashSellOrder.clear();

    bIdx = 0;
    sIdx = 0;
    pIdx = 0;

    for (int i = 0; i < MAXPRODUCT; i++) {
        products[i].minQueue.init();
        products[i].total = 0;
    }

    return;
}

int buy(int bId, int mProduct, int mPrice, int mQuantity) {
    buyOrder[bIdx].id = bId;
    buyOrder[bIdx].product = mProduct;
    buyOrder[bIdx].price = mPrice;
    buyOrder[bIdx].quantity = mQuantity;
    buyOrder[bIdx].initQuantity = mQuantity;
    buyOrder[bIdx].isCanceled = false;

    hashBuyOrder[bId] = bIdx;

    unordered_map<int, int>::iterator it = hashProduct.find(mProduct);
    if (it == hashProduct.end()) {
        hashProduct[mProduct] = pIdx;
        products[pIdx].product = mProduct;
        products[pIdx].total = mQuantity;
        products[pIdx].minQueue.push(mPrice, bId);

        buyOrder[bIdx].pIdx = pIdx;
        pIdx++;
    }
    else {
        int idx = hashProduct[mProduct];
        products[idx].total += mQuantity;
        products[idx].minQueue.push(mPrice, bId);

        buyOrder[bIdx].pIdx = idx;
    }

    bIdx++;

    int idx = hashProduct[mProduct];

    return products[idx].total;
}

int cancel(int bId) {
    unordered_map<int, int>::iterator it = hashBuyOrder.find(bId);
    if (it == hashBuyOrder.end()) {
        return -1;
    }
    else {
        int idx = hashBuyOrder[bId];
        if (buyOrder[idx].quantity < buyOrder[idx].initQuantity || buyOrder[idx].isCanceled) {
            return -1;
        }

        buyOrder[idx].isCanceled = true;
        products[buyOrder[idx].pIdx].total -= buyOrder[idx].quantity;
        buyOrder[idx].quantity = 0;

        return products[buyOrder[idx].pIdx].total;
    }
}

int sell(int sId, int mProduct, int mPrice, int mQuantity) {
    int productIdx = hashProduct[mProduct];
    if (products[productIdx].total < mQuantity) { return -1; }

    int ans = 0;

    sellOrder[sIdx].id = sId;
    sellOrder[sIdx].product = mProduct;
    sellOrder[sIdx].length = 0;
    sellOrder[sIdx].isRefunded = false;

    while (mQuantity > 0) {
        int buyIdx = hashBuyOrder[products[productIdx].minQueue.arr[0].id];

        if (buyOrder[buyIdx].isCanceled) {
            products[productIdx].minQueue.pop();
            continue;
        }

        if (buyOrder[buyIdx].quantity > mQuantity) {
            buyOrder[buyIdx].quantity -= mQuantity;
            ans += (mPrice - buyOrder[buyIdx].price) * mQuantity;
            products[productIdx].total -= mQuantity;

            sellOrder[sIdx].quantity[sellOrder[sIdx].length] = mQuantity;
            sellOrder[sIdx].bIdxs[sellOrder[sIdx].length] = buyIdx;
            sellOrder[sIdx].length++;

            mQuantity = 0;
        }
        else {
            mQuantity -= buyOrder[buyIdx].quantity;
            ans += (mPrice - buyOrder[buyIdx].price) * buyOrder[buyIdx].quantity;
            products[productIdx].total -= buyOrder[buyIdx].quantity;

            sellOrder[sIdx].quantity[sellOrder[sIdx].length] = buyOrder[buyIdx].quantity;
            sellOrder[sIdx].bIdxs[sellOrder[sIdx].length] = buyIdx;
            sellOrder[sIdx].length++;

            buyOrder[buyIdx].quantity = 0;
            products[productIdx].minQueue.pop();
        }
    }

    hashSellOrder[sId] = sIdx;
    sIdx++;

    return ans;
}

int refund(int sId) {
    unordered_map<int, int>::iterator it = hashSellOrder.find(sId);

    int ans = 0;

    if (it == hashSellOrder.end()) {
        return -1;
    }
    else {
        int sellIdx = hashSellOrder[sId];
        if (sellOrder[sellIdx].isRefunded) { return -1; }

        sellOrder[sellIdx].isRefunded = true;
        for (int i = 0; i < sellOrder[sellIdx].length; i++) {
            buyOrder[sellOrder[sellIdx].bIdxs[i]].quantity += sellOrder[sellIdx].quantity[i];
            int idx = buyOrder[sellOrder[sellIdx].bIdxs[i]].pIdx;
            products[idx].minQueue.push(buyOrder[sellOrder[sellIdx].bIdxs[i]].price, buyOrder[sellOrder[sellIdx].bIdxs[i]].id);
            products[idx].total += sellOrder[sellIdx].quantity[i];
            ans += sellOrder[sellIdx].quantity[i];
        }
    }

    return ans;
}
