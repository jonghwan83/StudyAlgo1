#include <vector>
#include <iostream>

#define nMax 26

using namespace std;

class Node {
public:
    char left;
    char right;
};

vector<Node> tree(nMax);

void preorder(char node) {
    if (node == '.') { return; }

    cout << node;
    preorder(tree[node].left);
    preorder(tree[node].right);
}

void inorder(char node) {
    if (node == '.') { return; }

    inorder(tree[node].left);
    cout << node;
    inorder(tree[node].right);
}

void postorder(char node) {
    if (node == '.') { return; }

    postorder(tree[node].left);
    postorder(tree[node].right);
    cout << node;
}


int main() {
    // freopen("sample_input.txt", "r", stdin);

    int N;
    char c, l, r;
    
    cin >> N;
    for (int i=0; i < N; i++) {
        cin >> c >> l >> r;
        tree[c].left = l;
        tree[c].right = r;
    }

    preorder('A');
    cout << "\n";
    inorder('A');
    cout << "\n";
    postorder('A');
    cout << "\n";

    return 0;
}