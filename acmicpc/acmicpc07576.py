import sys
from collections import deque

def bfs(n, m, box):
    dis = [-1, 1, 0, 0]
    djs = [0, 0, -1, 1]

    days = -1
    while queue:
        days += 1
        
        for _ in range(len(queue)):
            i, j = queue.popleft()
            for k in range(4):
                ni = i + dis[k]
                nj = j + djs[k]

                if (0 <= ni < n) and (0 <= nj < m) and (box[ni][nj] == 0):
                    box[ni][nj] = 1
                    queue.append([ni, nj])
    for lst in box:
        if 0 in lst:
            return -1

    return days


m, n = map(int, sys.stdin.readline().split())
box = []
queue = deque()

for i in range(n):
    row = list(map(int, sys.stdin.readline().split()))
    box.append(row)
    for j in range(m):
        if row[j] == 1:
            queue.append([i, j])

print(bfs(n, m, box))