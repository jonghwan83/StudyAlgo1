import sys
from collections import deque

n, m = map(int, sys.stdin.readline().split())

arr = []
initloc = []
zeroloc = []

for i in range(n):
    arr.append(list(map(int, sys.stdin.readline().split())))
    for j in range(m):
        if arr[i][j] == 2:
            initloc.append([i, j])
        elif arr[i][j] == 0:
            zeroloc.append([i, j])

drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

ans = []
arr2 = [[0 for _ in range(m)] for _ in range(n)]
def bfs():
    for i in range(len(zeroloc)):
        for j in range(len(zeroloc)):
            for k in range(len(zeroloc)):
                if i != j and j != k and k != i:
                    queue = deque()

                    for y in range(m):
                        for x in range(n):
                            arr2[x][y] = arr[x][y]
                    arr2[zeroloc[i][0]][zeroloc[i][1]] = 1
                    arr2[zeroloc[j][0]][zeroloc[j][1]] = 1
                    arr2[zeroloc[k][0]][zeroloc[k][1]] = 1

                    for l in range(len(initloc)):
                        queue.append(initloc[l])

                    while queue:
                        crow, ccol = queue.popleft()

                        arr2[crow][ccol] = 2
                        for d in range(4):
                            drow = crow + drows[d]
                            dcol = ccol + dcols[d]
                            if 0 <= drow < n and 0 <= dcol < m:
                                if arr2[drow][dcol] == 0:
                                    queue.append([drow, dcol])
                    
                    count = 0
                    for x in range(n):
                        for y in range(m):
                            if arr2[x][y] == 0:
                                count += 1
                    ans.append(count)

bfs()
print(max(ans))
