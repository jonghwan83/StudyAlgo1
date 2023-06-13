#include <cstring>

#define MAXN 10
#define MAXPOLY 10000
#define TABLESIZE 2048

using namespace std;

int char2key(char mName[]) {
    unsigned long int hash = 5381;

    for (int i = 0; i < strlen(mName); i++) {
        hash = (((hash << 5) + hash) + (int) mName[i]) % TABLESIZE;
    }

    return (int) hash % TABLESIZE;
}

class Node {
public:
    char name[11];
    int idx;
    Node* next;
};

class Term {
public:
    int x;
    int y;
    int c;
    Term* next;
};

class Polynomial {
public:
    char name[MAXN + 1];
    Term* head;

    void init() {
        head = nullptr;
    }

    void push(int x, int y, int c) {

        Term* curr = head;

        while (curr) {
            if (curr->x == x && curr->y == y) {
                curr->c += c;
                return;
            }
            curr = curr->next;
        }

        Term* node = new Term();
        node->x = x; node->y = y; node->c = c;

        node->next = head;
        head = node;
    }

    int find(int x, int y) {

        Term* curr = head;

        while (curr) {
            if (curr->x == x && curr->y == y) {
                return curr->c;
            }
            curr = curr->next;
        }

        return 0;
    }

};

Polynomial polynomials[MAXPOLY];
int pIdx;
Node* hashPoly[TABLESIZE];

void pushIndex(char mName[], int idx) {
    int key = char2key(mName);

    Node* node = new Node();
    strcpy(node->name, mName);
    node->idx = idx;

    node->next = hashPoly[key];
    hashPoly[key] = node;
}

int findPolynomial(char mName[]) {
    int key = char2key(mName);

    Node* node = hashPoly[key];

    while (node) {
        if (strcmp(mName, node->name) == 0) { return node->idx; }
        node = node->next;
    }

    return 0;
}

void poly2class(int idx, char mPolynomial[]) {

    int x = 0; int y = 0; int c = 0;

    int unknown;  // 1: x, 2: y
    int op = 0; // 0: +, 1: -, 2: ^
    bool st = true;

    for (int i = 0; i < strlen(mPolynomial); i++) {
        if (mPolynomial[i] == '+' || mPolynomial[i] == '-' || mPolynomial[i] == '^') {

            if (mPolynomial[i] == '^') { op = 2; }
            else {
                polynomials[idx].push(x, y, c);

                if (mPolynomial[i] == '+') {
                    op = 0;
                    x = 0; y = 0; c = 0; st = true;
                }

                else {
                    op = 1;
                    x = 0; y = 0; c = 0; st = true;
                }

            }
        }

        else if (mPolynomial[i] == 'X' || mPolynomial[i] == 'Y') {
            if (st) {
                c = 1;
                st = false;
                if (op == 1) { c *= -1; }
            }

            if (mPolynomial[i] == 'X') {
                x = 1;
                unknown = 1;
            }
            else {
                y = 1;
                unknown = 2;
            }
        }

        else {
            if (st) {
                st = false;
                c = (int) mPolynomial[i] - 48;
                if (op == 1) { c *= -1; }
            }
            else {
                if (op == 2) {
                    if (unknown == 1) { x *= (int) mPolynomial[i] - 48;}
                    else { y *= (int) mPolynomial[i] - 48; }
                }
            }
        }

    }

    polynomials[idx].push(x, y, c);
}

void copyPolynomial(int idx1, int idx2) {
    Term* curr = polynomials[idx2].head;

    while (curr) {
        polynomials[idx1].push(curr->x, curr->y, curr->c);
        curr = curr->next;
    }

}

void init()
{
    pIdx = 0;

    for (int i = 0; i < TABLESIZE; i++) {
        hashPoly[i] = nullptr;
    }
}

void assign(char mName[], char mPolynomial[])
{
    polynomials[pIdx].init();

    strcpy(polynomials[pIdx].name, mName);

    poly2class(pIdx, mPolynomial);

    pushIndex(mName, pIdx);

    pIdx++;
}

void compute(char mNameR[], char mNameA[], char mNameB[], int mOp)
{
    polynomials[pIdx].init();

    strcpy(polynomials[pIdx].name, mNameR);
    pushIndex(mNameR, pIdx);

    int idxA = findPolynomial(mNameA);
    int idxB = findPolynomial(mNameB);

    if (mOp < 2) {
        copyPolynomial(pIdx, idxA);

        Term* node = polynomials[idxB].head;
        while (node) {
            if (mOp == 0) { polynomials[pIdx].push(node->x, node->y, node->c); }
            else { polynomials[pIdx].push(node->x, node->y, -node->c); }
            node = node->next;
        }

    }

    else {

        Term* node1 = polynomials[idxA].head;


        while (node1) {

            Term* node2 = polynomials[idxB].head;

            while (node2) {
                polynomials[pIdx].push(node1->x + node2->x, node1->y + node2->y, node1->c * node2->c);
                node2 = node2->next;
            }

            node1 = node1->next;
        }

    }

    pIdx++;
}

int getCoefficient(char mName[], int mDegreeX, int mDegreeY)
{
    int idx = findPolynomial(mName);

    int result = polynomials[idx].find(mDegreeX, mDegreeY);

    result %= 10000;
    if (result < 0) { result += 10000; }

    return result;
}

int calcPolynomial(char mName[], int mX, int mY)
{
    int idx = findPolynomial(mName);

    int ans = 0;

    Term* curr = polynomials[idx].head;

    while (curr) {
        long long int value = 1;

        for (int i = 0; i < curr->x; i++) {
            value *= mX; value %= 10000;
        }

        for (int i = 0; i < curr->y; i++) {
            value *= mY; value %= 10000;
        }

        value *= curr->c;
        value %= 10000;

        ans += (int) value;

        curr = curr->next;
    }

    ans %= 10000;
    if (ans < 0) { ans += 10000; }

    return ans;
}