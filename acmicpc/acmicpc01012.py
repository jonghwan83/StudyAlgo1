import time
import random

t0 = time.time()

import sys
from collections import deque

def getAns(farm, row, col):
    visited = [[0 for _ in range(col)] for _ in range(row)]
    drows = [-1, 1, 0, 0]
    dcols = [0, 0, -1, 1]
    worms = deque()

    count = 0
    for y in range(row):
        for x in range(col):
            if farm[y][x] == 1:
                worms.append([y, x])

                while worms:
                    i, j = worms.popleft()
                    if farm[i][j] == 1:
                        farm[i][j] = 0
                        for t in range(4):
                            di = i + drows[t]
                            dj = j + dcols[t]
                            if 0 <= di < row and 0 <= dj < col:
                                if farm[di][dj] == 1:
                                    worms.append([di, dj])
        
                count += 1
    return count

T = int(sys.stdin.readline())
for _ in range(T):
    m, n, k = map(int, sys.stdin.readline().split())
    farm = [[0 for _ in range(m)] for _ in range(n)]

    for _ in range(k):
        col, row = map(int, sys.stdin.readline().split())
        farm[row][col] = 1

    print(getAns(farm, n, m))

t1 = time.time()
print('elapsed: {}'.format(t1 - t0))

'''
1
5 3 6
0 2
1 2
2 2
3 2
4 2
4 0
'''