#include <iostream>
#include <algorithm>
using namespace std;

struct Stock
{
	int n;
	int id[30000];
	int product[30000];
	int price[30000];
	int quantity[30000];
	bool isSold[30000];
};

struct Sold
{
	int n;
	int sid[30000];
	int bid[30000];
	int product[30000];
	int price[30000];
	int quantity[30000];
	int isRefund[30000];
};


Stock stock;
Sold sold;

int search_id(int id) {
	int idx = -1;

	for (int i=0; i < stock.n; i++) {
		if (stock.id[i] == id) {
			idx = i;
			break;
		}
	}
	return idx;
}

int search_product(int product) {
	int idx = -1;

	for (int i=0; i < stock.n; i++) {
		if (stock.product[i] == product) {
			idx = i;
			break;
		}
	}
	return idx;
}

int get_total(int prod) {
	int total = 0;

	for (int i=0; i < stock.n; i++) {
		if (prod == stock.product[i]) { 
			total = total + stock.quantity[i];
		}
	}

	return total;
}

void swap_var(int a, int b) {
	swap(stock.id[a], stock.id[b]);
	swap(stock.product[a], stock.product[b]);
	swap(stock.price[a], stock.price[b]);
	swap(stock.quantity[a], stock.quantity[b]);

	return;
}

int get_parent(int a) {
	return (a-1) / 2;
}

int get_left(int a) {
	return 2 * a + 1;
}

int get_right(int a) {
	return 2 * a + 2;
}

void heapfy_up(int c) {
	int k = stock.n;

	while (k > 0) {
		int parent = get_parent(k);
		if (stock.price[parent] > stock.price[k]) {
			swap_var(parent, k);
		} else if (stock.price[parent] == stock.price[k]) {
			if (stock.id[parent] > stock.id[k]) {
				swap_var(parent, k);
			}
		}
		k = parent;
	}
	return;
}

void heapfy_pop(int c) {
	int k = c;
	int last = stock.n - 1;

	swap_var(k, last);

	int parent = get_parent(k);
	if (stock.price[parent] < stock.price[k]) {
		heapfy_up(k);
	} else {
		int left = get_left(k);
		int right = get_right(k);

		while (left >= stock.n and right >= stock.n) {
			left = get_left(k);
			right = get_right(k);

			if (stock.price[left] < stock.price[k]) {
				swap_var(left, k);
				k = left;
			} else if (stock.price[right] < stock.price[k]) {
				swap_var(right, k);
				k = right;
			} else if (stock.price[left] == stock.price[k]) {
				if (stock.id[left] < stock.id[k]) {
					swap_var(left, k);
					k = left;
				}
			} else if (stock.price[right] == stock.price[k]) {
				if (stock.id[right] < stock.id[k]) {
					swap_var(right, k);
					k = right;
				}
			} else {
				break;
			}
		}
	}
	stock.n--;
	return;
}

void log_sold(int sid, int bid, int product, int price, int quantity) {
	sold.sid[sold.n] = sid;
	sold.bid[sold.n] = bid;
	sold.product[sold.n] = product;
	sold.price[sold.n] = price;
	sold.quantity[sold.n] = quantity;

	int k = sold.n;

	while (k > 0) {
		int parent = get_parent(k);

		if (sold.sid[parent] > sold.sid[k]) {
			swap_var(parent, k);
		} 
		k = parent;
	}

	sold.n++;
	return;
}

void init() {
	stock.n = 0;
	sold.n = 0;
	for (int i=1; i < 30000; i++) {
		stock.id[i] = 1000000000;
		stock.product[i] = 1000000000;
		stock.price[i] = 300000;
		stock.quantity[i] = 500;
		stock.isSold[i] = false;

		sold.sid[i] = 1000000000;
		sold.bid[i] = 1000000000;
		sold.price[i] = 300000;
		sold.quantity[i] = 500;
		sold.isRefund[i] = false;
	}
	return;
}

int buy(int bId, int mProduct, int mPrice, int mQuantity) {
	
	stock.id[stock.n] = bId;
	stock.product[stock.n] = mProduct;
	stock.price[stock.n] = mPrice;
	stock.quantity[stock.n] = mQuantity;
	heapfy_up(stock.n);

	stock.n++;

	cout << "buy: " << get_total(mProduct) << endl;
	return get_total(mProduct);
}

int cancel(int bId) {
	int idx = search_id(bId);

	if (stock.isSold[idx] == true || idx == -1) {
		cout << "cancel: " << -1 << endl;
		return -1;
	}

	int product = stock.product[idx];

	heapfy_pop(idx);

	cout << "cancel: " << get_total(product) << endl;
	return get_total(product);
}

int sell(int sId, int mProduct, int mPrice, int mQuantity) {
	int idx = search_product(mProduct);

	// sell
	if (get_total(stock.product[idx]) - mQuantity < 0) {
		cout << "sell: " << -1 << endl;
		return -1;
	}

	int quant = mQuantity;
	int revenue = 0;
	while (quant > 0) {
		if (stock.quantity[idx] > quant) {
			log_sold(sId, stock.id[idx], stock.product[idx], stock.price[idx], quant);
			stock.quantity[idx] -= quant;
			revenue += quant * (mPrice - stock.price[idx]);
			stock.isSold[idx] = true;	
			break;
		} else {
			log_sold(sId, stock.id[idx], stock.product[idx], stock.price[idx], stock.quantity[idx]);
			quant -= stock.quantity[idx];
			revenue += stock.quantity[idx] * (mPrice - stock.price[idx]);
			stock.quantity[idx] = 0;
			stock.isSold[idx] = true;
			
			heapfy_pop(idx);
			idx = search_product(mProduct);
		}
	}
	cout << "sell: "  << revenue <<  endl;

	if (stock.quantity[idx] == 0) {
		heapfy_pop(idx);
	}
	return revenue;
}

int refund(int sId) {
	int total = -1;

	cout << "bid: ";
	for (int i=0; i < sold.n; i++) {
		cout << sold.bid[i] << " ";
	}
	cout << endl;

	for (int i=0; i < sold.n; i++) {
		if (sold.sid[i] == sId && sold.isRefund[i] == false) {
			total += sold.quantity[i];
			sold.isRefund[i] = true;

			buy(sold.bid[i], sold.product[i], sold.price[i], sold.quantity[i]);
		}
	}
	cout << "refund: " << total << endl;
	return total + 1;
}