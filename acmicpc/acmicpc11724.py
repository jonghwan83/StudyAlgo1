import time

t0 = time.time()

import sys

arr = [[] for _ in range(1001)]

n, m = map(int, sys.stdin.readline().split())

for _ in range(m):
    u, v = map(int, sys.stdin.readline().split())
    arr[u].append(v)
    arr[v].append(u)

visited = [0] * (n + 1)
stack = []

count = 0

def dfs(count):
    for i in range(1, n+1):
        if visited[i]:
            continue

        stack.append(i)
        visited[i] = 1

        while stack:
            cnode = stack.pop()
            visited[cnode] = 1

            for node in arr[cnode]:
                if not visited[node]:
                    stack.append(node)
        count += 1

    return count

answer = dfs(count)
print(answer)

t1 = time.time()
print('elapsed :', t1- t0)
