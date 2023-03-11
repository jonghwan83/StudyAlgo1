#define MAXROW 100000 / 2

class Node {
public:
    int data;
    Node* prev;
    Node* next;

    Node(char c) {
        data = c;
        prev = nullptr;
        next = nullptr;
    }
};

class LinkedList {
public:
    Node* head;
    Node* tail;
    int length;

    LinkedList() {
        length = 0;
        head = nullptr;
        tail = nullptr;
    }

    void push(int a) {
        Node* node = new Node(a);
        if (length == 0) {
            head = node;
            tail = node;
            length++;
            return;
        }

        tail->next = node;
        node->prev = tail;
        tail = node;
        length++;
        return;
    }

    int pop_front() {
        int res = head->data;

        head = head->next;
        head->prev = nullptr;
        length--;

        return res;
    }
};

int idx;
int docSize, row, col;
LinkedList allRow[MAXROW];
LinkedList document;
Node* rowNode;
Node* charNode;

void openDocument(int N, char mContents[], int M)
{
    document = LinkedList();

    idx = 0;
    allRow[idx] = LinkedList();
    document.push(idx);

    for (int i = 0; i < N; i++) {
        allRow[idx].push(mContents[i]);
        if (allRow[idx].length >= M || mContents[i] == '\n') {
            allRow[++idx] = LinkedList();
            document.push(idx);
        }
    }
    allRow[idx].push('!');

    docSize = M;
}

void moveCursor(int mPageNumber, int mRow, int mCol)
{
    int totalRow = (mPageNumber - 1) * docSize + (mRow - 1);
    rowNode = document.head;

    while (rowNode->next && totalRow > 0) {
        rowNode = rowNode->next;
        totalRow--;
    }
    row = mRow - totalRow;

    if (totalRow > 0) {
        charNode = allRow[rowNode->data].tail;
        col = allRow[rowNode->data].length;
        return;
    }
    else {
        int k = 1;
        charNode = allRow[rowNode->data].head;
        while (charNode->next && k < mCol) {
            charNode = charNode->next;
            k++;
        }
    }
    col = mCol;
}

void addNewLine()
{
    allRow[++idx] = LinkedList();

    for (int i = 1; i < col; i++) {
        allRow[idx].push(allRow[rowNode->data].pop_front());
    }

    allRow[idx].push('\n');
    Node* node = new Node(idx);

    if (rowNode->prev) {
        node->prev = rowNode->prev;
        rowNode->prev->next = node;
    }
    node->next = rowNode;
    if (!rowNode->prev) { document.head = node; }
    rowNode->prev = node;

    row++;
    col = 1;
    if (row >= docSize) { row = 1; }
}

void backSpace(int mLength)
{
    while (mLength > 0 && rowNode->prev) {
        if (charNode == allRow[rowNode->data].head) {
            allRow[rowNode->prev->data].push(charNode->data);
            rowNode->prev->next = rowNode->next;
            if (rowNode->next) { rowNode->next->prev = rowNode->prev; }

            rowNode = rowNode->prev;
            if (!rowNode->next) { document.tail = rowNode; }
            col = allRow[rowNode->data].length;

            charNode = allRow[rowNode->data].tail;
            row--;
            continue;
        }
        else {
            Node* preNode = charNode->prev;
            if (preNode->prev) {
                charNode->prev->prev->next = charNode;
                charNode->prev = charNode->prev->prev;
            }
            else {
                allRow[rowNode->data].head = charNode;
                charNode->prev = nullptr;
            }
        }

        allRow[rowNode->data].length--;
        mLength--;
        col--;
        if (row == 0) { row = docSize; }
    }

    int k = docSize - col;
    Node* temp = rowNode;
    bool endFlag = false;

    while (temp->next && charNode->data != '\n') {
        for (int i = 0; i < k; i++) {
            if (allRow[temp->next->data].head->data == '!') {
                endFlag = true;
                break;
            }
            if (allRow[temp->next->data].head->data == '\n') { endFlag = true; }
            allRow[temp->data].push(allRow[temp->next->data].pop_front());
            if (endFlag) { break; }
        }
        if (endFlag) { return; }
        temp = temp->next;
    }
}

void getRow(int mRow, char retString[])
{
    int k = mRow - row;
    Node* node = rowNode;

    while (k != 0 && node) {
        if (k > 0) {
            k--;
            node = node->next;
        }
        else {
            k++;
            node = node->prev;
        }
    }

    if (!node) {
        retString[0] = '\0';
        return;
    }

    int i = 0;
    Node* ans = allRow[node->data].head;
    while (ans && ans->data != '!') {
        retString[i] = ans->data;
        ans = ans->next;
        i++;
    }
    retString[i] = '\0';
}

