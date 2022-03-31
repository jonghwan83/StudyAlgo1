#include <iostream>

using namespace std;

class Node {
public:
	int data;
	Node* next;
	Node* prev;

	Node(int a) {
		data = a;
		next = NULL;
	}
};

class Queue {
public:
	Node* head;
	Node* tail;
	int length;

	Queue() {
		head = NULL;
		tail = NULL;
		length = 0;
	}
	void push(int a) {
		Node* new_node = new Node(a);

		if (length == 0) {
			head = new_node;
			tail = new_node;
			head->next = tail;
			tail->prev = head;
		}
		else {
			if (new_node->data > head->data) {
				Node* first;
				first = head;
				new_node->next = first;
				head = new_node;
				first->prev = head;
			}
			else {
				Node* second;
				second = head->next;
				head->next = new_node;
				new_node->next = second;
				new_node->prev = head;
				second->prev = new_node;
			}
			while (new_node->data < new_node->next->data) {
				Node* nxt;
				Node* prv;
				
				nxt = new_node->next;
				prv = new_node->prev;

				nxt->prev = prv;
				prv->next = nxt;

				new_node->next = nxt->next;
				new_node->prev = nxt;
			}
		}
		length++;
	}

	int pop() {
		if (length == 0) {
			return 0;
		}
		else {
			int output;
			output = head->data;

			if (length > 0) {
				Node* second;
				second = head->next;
				second->prev = NULL;
				head = second;
			}
			length--;

			return output;
		}
	}
};

int main() {
	Queue *queue = new Queue;

	int n, k;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> k;
		if (k > 0) {
			queue->push(k);
		}
		else {
			cout << queue->pop() << "\n";
		}
	}
	return 0;
}