#include <string.h>

using namespace std;

#define MAXROW 500
#define ALPHABET 26

class Node {
public:
    char data;
    Node* next;
    Node* prev;

    Node() {
        next = nullptr;
        prev = nullptr;
        data = '$';
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

        head = h;
        tail = t;
        head->next = tail;
        tail->prev = head;
    }

    void push(char c) {
        Node* node = new Node();
        node->data = c;

        node->next = tail;
        node->prev = tail->prev;
        tail->prev->next = node;
        tail->prev = node;
        length++;
    }

    char pop() {
        if (tail->prev == head) {
            return '$';
        }

        Node* node = tail->prev;
        char ans = node->data;

        node->prev->next = tail;
        tail->prev = node->prev;
        length--;

        return ans;
    }

    void push_front(char c) {
        Node* node = new Node();
        node->data = c;

        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
        length++;
    }
};

Node* cursor;
LinkedList memo[MAXROW];
int rIdx;
int width, height;
int row;
int hashChar[MAXROW][ALPHABET];

void init_hash(int r) {
    for (int i = 0; i < ALPHABET; i++) {
        hashChar[r][i] = 0;
    }
}

void push_back(char c) {
    if (memo[rIdx - 1].length >= width) {
        memo[rIdx++].init();
        init_hash(rIdx - 1);
    }

    memo[rIdx - 1].push(c);
    hashChar[rIdx - 1][(int)c - (int)'a']++;
}

void init(int H, int W, char mStr[])
{
    rIdx = 0;
    width = W;
    height = H;

    memo[rIdx++].init();
    for (int i = 0; i < strlen(mStr); i++) {
        push_back(mStr[i]);
    }

    row = 0;
    cursor = memo[0].head;
}

void insert(char mChar)
{
    Node* node = new Node();
    node->data = mChar;

    node->prev = cursor;
    node->next = cursor->next;
    cursor->next->prev = node;
    cursor->next = node;
    memo[row].length++;
    hashChar[row][(int)mChar - (int)'a']++;

    for (int i = row; i < rIdx - 1; i++) {
        char s = memo[i].pop();
        hashChar[i][(int)s - (int)'a']--;
        memo[i + 1].push_front(s);
        hashChar[i + 1][(int)s - (int)'a']++;
    }

    if (memo[rIdx - 1].length > width) {
        memo[rIdx++].init();
        init_hash(rIdx - 1);
        char s = memo[rIdx - 2].pop();
        hashChar[rIdx - 2][(int)s - (int)'a']--;
        memo[rIdx - 1].push(s);
        hashChar[rIdx - 1][(int)s - (int)'a']++;

    }

    if (cursor->next == memo[row].tail) {
        cursor = memo[++row].head->next;
    }
    else {
        cursor = cursor->next;
    }
}

char moveCursor(int mRow, int mCol)
{
    mRow--; mCol--;
    if (mRow > rIdx - 1) {
        cursor = memo[rIdx - 1].tail->prev;
        row = rIdx - 1;
        return cursor->next->data;
    }

    cursor = memo[mRow].head;
    for (int i = 0; i < mCol; i++) {
        cursor = cursor->next;
        if (cursor->next == memo[mRow].tail) { break; }
    }
    row = mRow;
    return cursor->next->data;
}

int countCharacter(char mChar)
{
    int ans = 0;

    for (int i = row + 1; i < rIdx; i++) {
        ans += hashChar[i][(int)mChar - (int)'a'];
    }

    Node* temp = cursor->next;
    while (temp->next) {
        if (temp->data == mChar) {
            ans++;
        }
        temp = temp->next;
    }

    return ans;
}