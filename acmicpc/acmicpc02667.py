'''
intput
7
0110100
0110101
1110101
0000111
0100000
0111110
0111000
'''
import time
import random

# n = 25
# houses = []
# for _ in range(n):
#     houses.append([str(random.randint(0, 1)) for _ in range(n)])

t0 = time.time()

import sys
from collections import deque

n = int(sys.stdin.readline())
houses = []
for _ in range(n):
    houses.append(list(sys.stdin.readline().strip()))

visited = [[0] * (n) for _ in range(n)]
queue = deque()
cluster = deque()

for i in range(n):
    for j in range(n):
        queue.append([i, j])

drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

result = []

while queue:
    row, col = queue.popleft()
    if houses[row][col] == '1' and not visited[row][col]:
        cluster.append([row, col])
        ncluster = 0
        while cluster:
            crow, ccol = cluster.popleft()
            if not visited[crow][ccol]:
                visited[crow][ccol] = 1
                if houses[crow][ccol] == '1':
                    ncluster += 1
                if houses[crow][ccol] == '1':
                    for k in range(4):
                        nrow = crow + drows[k]
                        ncol = ccol + dcols[k]
                        if 0 <= nrow < n and 0 <= ncol < n and not visited[nrow][ncol]:
                            cluster.append([nrow, ncol])
        result.append(ncluster)

result.sort()
print(len(result))
for x in result:
    print(x)

t1 = time.time()
print('elpased: {}'.format(t1 - t0))