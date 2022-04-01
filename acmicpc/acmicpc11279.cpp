#include <iostream>

using namespace std;

class Node {
public:	
	int data;
	Node* next;
	
	Node(int a) {
		data = a;
		next = NULL;
	}
};

class Queue {
public:
	int length;
	Node* head;

	Queue() {
		length = 0;
		head = NULL;
	}
	void push(int a) {
		Node* newNode = new Node(a);
		
		if (length == 0) {
			head = newNode;
		}
		else if (length == 1) {
			Node* cNode;
			cNode = head;

			if (cNode->data > newNode->data) {
				head->next = newNode;
			}
			else {
				newNode->next = cNode;
				head = newNode;
			}
		}
		else {
			Node* cNode;
			cNode = head;
			
			if (newNode->data > cNode->data) {
				newNode->next = cNode;
				head = newNode;
			}
			else {
				while (cNode->next != NULL) {
					if (newNode->data > cNode->data) {
						break;
					}
					
					cNode = cNode->next;
				}
				if (cNode->next != NULL) {
					cNode->next = newNode;
				}
				else {
					Node* nNode;
					nNode = cNode->next;
					cNode->next = newNode;
					newNode->next = nNode;
				}
			}
		}
		length++;
	}

	int pop() {
		if (length == 0) {
			return 0;
		}
		else {
			int answer = head->data;
			if (length > 1) {
				Node* second;
				second = head->next;

				head = second;
			}
			else {
				head = NULL;
			}
			length--;
			return answer;
		}
	}
};

int main() {
	Queue* queue = new Queue();
	
	int n;
	long int k;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> k;
		k = rand() % 100;
		if (k == 0) {
			cout << queue->pop() << "\n";
		}
		else {
			queue->push(k);
		}
	}

	return 0;
}