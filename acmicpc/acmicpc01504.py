from collections import deque
import sys

# sys.stdin = open('acmicpc/sample.txt', 'r')

n, e = map(int, sys.stdin.readline().split())
graph = [[-1 for _ in range(n+1)] for _ in range(n+1)]

for _ in range(e):
    start, end, weight = map(int, sys.stdin.readline().split())
    graph[start][end] = weight
    graph[end][start] = weight

v1, v2 = map(int, sys.stdin.readline().split())



def bfs(s, d, G):
    min_weight = [sys.maxsize] * (n+1)
    min_weight[s] = 0

    queue = deque()
    queue.append(s)
    while queue:
        strt = queue.popleft()
        for nxt, val in enumerate(G[strt]):
            if val > 0:
                if min_weight[nxt] > (min_weight[strt] + G[strt][nxt]):
                    min_weight[nxt] = min_weight[strt] + G[strt][nxt]
                    queue.append(nxt)

    return min_weight[d]

answer = min(bfs(1, v1, graph) + bfs(v1, v2, graph) + bfs(v2, n, graph), \
    bfs(1, v2, graph) + bfs(v2, v1, graph) + bfs(v1, n, graph))

if answer >= sys.maxsize:
    print(-1)
else:
    print(answer)

