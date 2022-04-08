#include <iostream>
#include <string.h>

using namespace std;

class Node{
public:
    int data;
    Node* next;
    Node* prev;

    Node(int a){
        data = a;
    }
};

class Queue{
public:
    int length;
    Node* head;
    Node* tail;

    Queue(){
        length = 0;
        head = NULL;
        tail = NULL;
    }

    void push_back(int a){
        Node* new_node = new Node(a);

        if (length == 0){
            head = new_node;
            tail = new_node;
            
            head->next = tail;
            tail->prev = head;
            
        } else if (length == 1){
            tail = new_node;
            
            head->next = tail;
            tail->prev = head;
        } else {
            tail->next = new_node;
            new_node->prev = tail;

            tail = tail->next;
        }
        length++;
    }

    void push_front(int a){
        Node* new_node = new Node(a);

        if (length == 0){
            head = new_node;
            tail = new_node;

            head->next = tail;
            tail->prev = head;
        }  else if (length == 1){
            head->prev = new_node;
            new_node->next = head;

            tail = head;
            head = new_node;
        } else {
            head->prev = new_node;
            new_node->next = head;

            head = new_node;
        }
        length++;
    }

    int pop_front(){
        if (length < 1){
            return -1;
        }

        int answer = head->data;
        if (length == 1){
            head = NULL;
            tail = NULL;
        } else {
            head = head->next;
            head->prev = NULL;
        }
        length--;
        return answer;
    }

    int pop_back(){
        if (length < 1){
            return -1;
        }

        int answer = tail->data;
        if (length == 1){
            head = NULL;
            tail = NULL;
        } else {
            tail = tail->prev;
            tail->next = NULL;
        }
        length--;
        return answer;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    Queue* deque = new Queue;

    string cmd;
    int n, k;
    cin >> n;
    for (int i=0; i < n; i++){
        cin >> cmd;
        if (cmd == "push_back"){
            cin >> k;
            deque->push_back(k);
        } else if (cmd == "push_front"){
            cin >> k;
            deque->push_front(k);
        } else if (cmd == "pop_front"){
            cout << deque->pop_front() << "\n";
        } else if (cmd == "pop_back"){
            cout << deque->pop_back() << "\n";
        } else if (cmd == "size"){
            cout << deque->length << "\n";
        } else if (cmd == "empty"){
            if (deque->length < 1){
                cout << 1 << "\n";
            } else {
                cout << 0 << "\n";
            }
        } else if (cmd == "front"){
            if (deque->length < 1){
                cout << -1 << "\n";
            } else {
                cout << deque->head->data << "\n";
            }
        } else {
            if (deque->length < 1){
                cout << -1 << "\n";
            } else {
                cout << deque->tail->data << "\n";
            }
        }
    }

    return 0;
}