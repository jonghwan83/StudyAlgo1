class Node {
public:
    int data;
    Node* next;
    Node* prev;

    Node() {
        data = 0;
        next = nullptr;
        prev = nullptr;
    }
};

class LinkedList {
public:
    int length;
    Node* head;
    Node* tail;

    void init() {
        length = 0;

        Node* h = new Node();
        Node* t = new Node();

        head = h; tail = t;
        head->next = tail;
        tail->prev = head;
    }

    void push_front(int a) {
        Node* node = new Node();
        node->data = a;

        node->prev = head;
        node->next = head->next;

        head->next->prev = node;
        head->next = node;
        length++;
    }

    void push_back(int a) {
        Node* node = new Node();
        node->data = a;

        node->next = tail;
        node->prev = tail->prev;

        tail->prev->next = node;
        tail->prev = node;
        length++;
    }
};

LinkedList arr;
int joker;

void init(int mJoker, int mNumbers[5])
{
    joker = mJoker;
    arr.init();

    for (int i = 0; i < 5; i++) {
        arr.push_back(mNumbers[i]);
    }
}

void putCards(int mDir, int mNumbers[5])
{
    if (mDir == 0) {
        for (int i = 4; i >= 0; i--) {
            arr.push_front(mNumbers[i]);
        }
    }
    else {
        for (int i = 0; i < 5; i++) {
            arr.push_back(mNumbers[i]);
        }
    }
}

int findNumber(int mNum, int mNth, int ret[4])
{
    int a;
    int k = 0;
    Node* node = arr.head->next;
    for (int i = 0; i <= arr.length - 4; i++) {

        a = 0;
        Node* temp = node;
        for (int j = 0; j < 4; j++) {
            ret[j] = temp->data;

            if (temp->data == -1) {
                a += joker;
            }
            else {
                a += temp->data;
            }

            temp = temp->next;
        }

        if (a % 20 == mNum) {
            k++;
        }

        if (k == mNth) {
            return 1;
        }

        node = node->next;
    }
    return 0;
}

void changeJoker(int mValue)
{
    joker = mValue;
}