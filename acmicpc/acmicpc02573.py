from collections import deque
import sys

# sys.stdin = open('acmicpc/sample.txt', 'r')

n, m = map(int, sys.stdin.readline().split())
graph = [[] for _ in range(n)]
loc_ice = deque()
for i in range(n):
    graph[i] = list(map(int, sys.stdin.readline().split()))
    for j in range(m):
        if graph[i][j] != 0:
            loc_ice.append([i, j])

drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]


def get_region(G):
    visited = [[0 for _ in range(m)] for _ in range(n)]
    n_region = 0

    for i in range(n):
        for j in range(m):
            if G[i][j] != 0 and not visited[i][j]:
                stack = []
                stack.append([i, j])
                visited[i][j] = 1
                while stack:
                    crow, ccol = stack.pop()
                    for k in range(4):
                        drow = crow + drows[k]
                        dcol = ccol + dcols[k]
                        if 0 <= drow < n and 0 <= dcol < m:
                            if G[drow][dcol] != 0 and not visited[drow][dcol]:
                                visited[drow][dcol] = 1
                                stack.append([drow, dcol])
                n_region += 1

    return n_region


def pass_time():
    t_loc = len(loc_ice)
    n_waters = [0] * t_loc
    for i in range(t_loc):
        crow = loc_ice[i][0]
        ccol = loc_ice[i][1]
        water = 0
        for k in range(4):
            drow = crow + drows[k]
            dcol = ccol + dcols[k]
            if 0 <= drow < n and 0 <= dcol < m:
                if graph[drow][dcol] == 0:
                    water += 1
        n_waters[i] = water

    i = 0
    while i < t_loc:
        crow, ccol = loc_ice.popleft()
        if graph[crow][ccol] - n_waters[i] <= 0:
            graph[crow][ccol] = 0
        else:
            graph[crow][ccol] -= n_waters[i]
            loc_ice.append([crow, ccol])

        i += 1


n_region = get_region(graph)
n_year = 0
while n_region == 1:
    if len(loc_ice) == 1:
        break
    pass_time()
    n_year += 1
    n_region = get_region(graph)

if n_region <= 1:
    print(0)
else:    
    print(n_year)
    