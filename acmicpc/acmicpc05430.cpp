// segmentation fault? 테스트 케이스에서는 문제 없지만 제출에서 발생

#include <iostream>

using namespace std;

class Node{
    public:
        int data;
        Node *next;
        Node *prev;
        Node(int a){
            data = a;
        }
};

class Queue{
    public:
        Node *head;
        Node *tail;
        int length;
        bool revflag;

        Queue(){
            length = 0;
            head = NULL;
            tail = NULL;
            revflag = false;
        }
        void push(int a){
            Node *new_node = new Node(a);
            if (head == NULL){
                head = new_node;
                tail = new_node;
            } else {
                if (length > 1){
                    Node *last = tail;
                    last->next = new_node;
                    new_node->prev = last;
                    tail = new_node;
                } else if (length == 1){
                    head->next = new_node;
                    new_node->prev = head;
                    tail = new_node;
                }
            }
            length++;
        }
        bool pop(){
            if (revflag == false){
                if (length > 1){
                    head = head->next;
                    head->prev = NULL;
                } else if (length == 1){
                    head = NULL;
                    tail = NULL;
                } else {
                    return false;
                }
            } else {
                if (length > 1){
                    tail = tail->prev;
                    tail->next = NULL;
                } else if (length == 1){
                    head = NULL;
                    tail = NULL;
                } else {
                    return false;
                }
            }
            length--;
            if (length == 1){
                tail = head;
            }
            return true;
        }

        string traverse(){
            if (length == 0){
                return "[]";
            }
            string answer = "";
            Node *cnode;
            if (revflag == false){
                cnode = head;
                answer = answer + to_string(cnode->data) + ",";
                while (cnode->next){
                    cnode = cnode->next;
                    answer = answer + to_string(cnode->data) + ",";
                }
            } else {
                cnode = tail;
                answer = answer + to_string(cnode->data) + ",";
                while (cnode->prev){
                    cnode = cnode->prev;
                    answer = answer + to_string(cnode->data) + ",";
                }
            }
            return "[" + answer.substr(0, answer.length() - 1) + "]";
        }

        void changeflag(){
            if (revflag == false){
                revflag = true;
            } else {
                revflag = false;
            }
        }
};

int main(){
    int t, n;
    cin >> t;
    string cmd, numbers;
    for (int k=0; k < t; k++){
        cin >> cmd;
        cin >> n;
        cin >> numbers;

        Queue *queue = new Queue();
        int i = 0;
        string result = "";
        while (true){
            if (numbers[i] == ']'){
                try{
                    queue->push(stoi(result));
                } catch(exception){
                    ;
                }
                result = "";
                break;
            }
            if (numbers[i] != '[' && numbers[i] != ','){
                result = result + numbers[i];
            }
            if (numbers[i] == ','){
                try{
                    queue->push(stoi(result));
                } catch(exception){
                    ;
                }
                result = "";
            }
            i++;
        }
        bool ans = true;
        for (int j=0; j < cmd.length(); j++){
            if (cmd[j] == 'R'){
                queue->changeflag();
            } else if (cmd[j] == 'D'){
                ans = queue->pop();
            }
        }
        if (ans){
            cout << queue->traverse() << "\n";
        } else {
            cout << "error" << "\n";
        }
    }
    return 0;
}