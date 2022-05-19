import sys
from collections import deque

# sys.stdin = open('acmicpc/sample.txt', 'r')

m, n, h = map(int, sys.stdin.readline().split())
graph = [[] for _ in range(h)]
visited = [[] for _ in range(h)]

queue = deque()

for i in range(h):
    for j in range(n):
        temp = list(map(int, sys.stdin.readline().split()))
        graph[i].append(temp)
        visited[i].append(temp)
        for k in range(len(temp)):
            if temp[k] == 1:
                queue.append([i, j, k])

dxs = [-1, 1, 0, 0, 0, 0]
dys = [0, 0, -1, 1, 0, 0]
dzs = [0, 0, 0, 0, -1, 1]

#graph[z][y][x]

while queue:
    cz, cy, cx = queue.popleft()
    for i in range(6):
        dx = cx + dxs[i]
        dy = cy + dys[i]
        dz = cz + dzs[i]
        
        if (0 <= dx < m) and (0 <= dy < n) and (0 <= dz < h):
            if not visited[dz][dy][dx]:
                queue.append([dz, dy, dx])
                visited[dz][dy][dx] = 1
                
                graph[dz][dy][dx] = graph[cz][cy][cx] + 1

def get_result():
    answer = 0
    for i in range(h):
        for j in range(n):
            for k in range(m):
                if graph[i][j][k] == 0:
                    answer = 0
                    return answer
                if graph[i][j][k] > answer:
                    answer = graph[i][j][k]
    return answer

print(get_result() - 1)
