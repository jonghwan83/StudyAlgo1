#include <iostream>
#include <vector>
#define MAX 1000001

using namespace std;

vector<int> parent;
int find(int i)
{
    if (parent[i] == i)
        return i;
    int j = find(parent[i]);
    parent[i] = j;
    return j;
}

void getUnion(int a, int b) {
	int x = find(a);
	int y = find(b);

	if (x > y) {
		parent[x] = y;
	} else if (x < y) {
		parent[y] = x;
	}
}

int main() {
	// freopen("sample.txt", "r", stdin);
	
	int n, m;	
	scanf("%d %d", &n, &m);

	for (int i=0; i <= n; i++) {
		parent.push_back(i);
	}

	for (int i=0; i < m; i++) {
		int t, a, b;
		scanf("%d %d %d", &t, &a, &b);

		if (t == 0) {
			getUnion(a, b);
		} else {
			if (find(a) == find(b)) {
				printf("YES\n");
			} else {
				printf("NO\n");
			}
		}
	}

	return 0;
}