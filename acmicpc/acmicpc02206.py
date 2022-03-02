# not enough time complexity

import time
import random

t0 = time.time()

n, m = [70,70]
maze = [[0 for _ in range(m)] for _ in range(n)]
k = []
for i in range(n):
    for j in range(m):
        maze[i][j] = random.randint(0, 1)
        if maze[i][j] == 1:
            k.append([i, j])

import sys
from collections import deque
from copy import deepcopy

# n, m = map(int, sys.stdin.readline().split())
# maze = [[0 for _ in range(m)] for _ in range(n)]

# k = []
# for i in range(n):
#     str = sys.stdin.readline().strip()
#     for j, s in enumerate(str):
#         maze[i][j] = int(s)
#         if maze[i][j] == 1:
#             k.append([i, j])

def bfs(maze, i, j):
    drows = [-1, 1, 0, 0]
    dcols = [0, 0, -1, 1]
    outlet = 0
    for d in range(4):
        di = i + drows[d]
        dj = j + dcols[d]
        if 0 <= di < n and 0 <= dj < m:
            if maze[di][dj] == 0:
                outlet += 1

    if outlet < 2:
        return -1

    maze2 = deepcopy(maze)
    maze2[i][j] = 0

    solution = [[-1 for _ in range(m)]for _ in range(n)]
    queue = deque()
    queue.append([0, 0])
    solution[0][0] = 1
    while queue:
        crow, ccol = queue.popleft()
        for d in range(4):
            drow = crow + drows[d]
            dcol = ccol + dcols[d]
            if 0 <= drow < n and 0 <= dcol < m:
                if maze2[drow][dcol] == 0 and solution[drow][dcol] == -1:
                    queue.append([drow, dcol])
                    solution[drow][dcol] = solution[crow][ccol] + 1
    
    return solution[n-1][m-1]

ans = sys.maxsize
for i, j in k:
    temp = bfs(maze, i, j)
    if temp != -1:
        ans = min(temp, ans)

if ans != sys.maxsize:
    print(ans)
else:
    print(-1)

t1 = time.time()
print('elapsed :{}'.format(t1 - t0))
