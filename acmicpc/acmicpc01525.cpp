#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>

using namespace std;

class Node {
public:
    int loc;
    string p;
    int cnt;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

//    freopen("sample_input.txt", "r", stdin);

    string puzzle; int st;
    int dir[4] = { -3, 3, -1, 1 };

    unordered_map<string, bool> visited;

    for (int i = 0; i < 9; i++) {
        char c;
        cin >> c;
        puzzle += c;
        if (c == '0') { st = i; }
    }

    deque<Node> queue;

    Node node;
    node.loc = st; node.p = puzzle; node.cnt = 0;

    queue.push_back(node);
    visited.insert(make_pair(puzzle, true));

    while (!queue.empty()) {
        Node curr = queue.front(); queue.pop_front();

        if (curr.p.compare("123456780") == 0) {
            cout << curr.cnt << "\n";
            return 0;
        }

        for (int i = 0; i < 4; i++) {
            int d = curr.loc + dir[i];

            if (d < 0 || d > 8) { continue; }
            if (curr.loc % 3 == 0 && d % 3 == 2) { continue; }
            if (curr.loc % 3 == 2 && d % 3 == 0) { continue; }

            string newPuzzle = curr.p;
            swap(newPuzzle[curr.loc], newPuzzle[d]);


            if (newPuzzle.compare("123456780") == 0) {
                cout << curr.cnt + 1 << "\n";
                return 0;
            }

            auto visited_itr = visited.find(newPuzzle);
            if (visited_itr == visited.end()) {
                visited.insert(make_pair(newPuzzle, true));
                Node newNode;
                newNode.loc = d; newNode.p = newPuzzle; newNode.cnt = curr.cnt + 1;
                queue.push_back(newNode);
            }
        }
    }

    cout << -1 << "\n";

    return 0;
}