import sys
from collections import deque


def bfs(maze):
    drows = [-1, 1, 0, 0]
    dcols = [0, 0, -1, 1]

    solution = [[[-1, -1] for _ in range(m)] for _ in range(n)]
    queue = deque()

    queue.append([0, 0, 1])
    solution[0][0][1] = 1

    while queue:
        crow, ccol, block = queue.popleft()

        if crow == n-1 and ccol == m-1:
            return solution[n-1][m-1][block]

        for d in range(4):
            drow = crow + drows[d]
            dcol = ccol + dcols[d]

            if 0 <= drow < n and 0 <= dcol < m:
                if maze[drow][dcol] == 1 and block:
                    solution[drow][dcol][block - 1] = solution[crow][ccol][block] + 1
                    queue.append([drow, dcol, block - 1])
                if maze[drow][dcol] == 0 and solution[drow][dcol][block] == -1:
                    solution[drow][dcol][block] = solution[crow][ccol][block] + 1
                    queue.append([drow, dcol, block])

    return -1


n, m = map(int, sys.stdin.readline().split())
maze = [[0 for _ in range(m)] for _ in range(n)]

for i in range(n):
   str = sys.stdin.readline().strip()
   for j, s in enumerate(str):
       maze[i][j] = int(s)

answer = bfs(maze)
print(answer)
