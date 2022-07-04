#include <iostream>
#include <queue>

using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int k, m;
    priority_queue<int> leftq, rightq;

    cin >> k;
    for (int i=0; i < k; i++) {
        cin >> m;
        if (leftq.size() == rightq.size()) {
            leftq.push(m);
        } else {
            rightq.push(-m);
        }

        if (leftq.size() > 0 && rightq.size()) {
            if (leftq.top() > -rightq.top()) {
                int a = leftq.top();
                int b = -rightq.top();
                leftq.pop(); rightq.pop();

                leftq.push(b);
                rightq.push(-a);
            }
        }
        cout << leftq.top() << "\n";
    }
    return 0;
}