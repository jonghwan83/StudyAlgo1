import sys
from collections import deque

sys.setrecursionlimit(100000)

drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

def dfs(sPoint, color):
    global visited, n, graph
    if visited[sPoint[0]][sPoint[1]] or graph[sPoint[0]][sPoint[1]] != color:
        return
    else:
        visited[sPoint[0]][sPoint[1]] = 1

    for i in range(4):
        dr = sPoint[0] + drows[i]
        dc = sPoint[1] + dcols[i]

        if (0 <= dr < n) and (0 <= dc < n):
            dfs([dr, dc], color)

    return


if __name__ == "__main__":
    global visited, n, human

    # sys.stdin = open('sample_input.txt', 'r')

    human = 0
    cow = 0
    
    n = int(sys.stdin.readline())
    graph = []
    for _ in range(n):
        graph.append(list(sys.stdin.readline().strip()))

    visited = [[0] * n for _ in range(n)]

    for i in range(n):
        for j in range(n):
            if not visited[i][j]:
                dfs([i, j], graph[i][j])
                human += 1

    for i in range(n):
        for j in range(n):
            if graph[i][j] == 'R':
                graph[i][j] = 'G'

    visited = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            if not visited[i][j]:
                dfs([i, j], graph[i][j])
                cow += 1

    print(human, cow)