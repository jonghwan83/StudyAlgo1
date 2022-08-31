import copy
from collections import deque

drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

def riseSeaLevel(mGrid, mSeaLevel):
    graph = copy.deepcopy(mGrid)
    visited = [[0] * n for _ in range(n)]

    queue = deque()
    for r in [0, n-1]:
        for c in range(n):
            if graph[r][c] < mSeaLevel:
                queue.appendleft((r, c))

    for c in [0, n-1]:
        for r in range(1, n-1):
            if graph[r][c] < mSeaLevel:
                queue.appendleft((r, c))

    while queue:
        r, c = queue.popleft()
        visited[r][c] = 1
        graph[r][c] = 0

        for i in range(4):
            dr = r + drows[i]
            dc = c + dcols[i]
            if (0 < dr < n) and (0 < dc < n):
                if (graph[dr][dc] < mSeaLevel) and not visited[dr][dc]:
                    queue.append((dr, dc))
                    visited[dr][dc] = 1

    summation = 0
    for i in range(n):
        summation += sum(visited[i])

    return n**2 - summation


def init(N, mMap):
    global grid, n
    grid = mMap
    n = N
    return

def numberOfCandidate(M, mStructure):   
    global grid, n
    
    if M == 1:
        return n ** 2

    count = 0
    isSame = True

    for r in range(n):
        for c in range(n - M + 1):
            height = grid[r][c] + mStructure[0]
            isSame = True
            for i in range(1, M):
                if height != grid[r][c+i] + mStructure[i]:
                    isSame = False
                    break
            if isSame:
                count += 1

    for c in range(n):
        for r in range(n - M + 1):
            height = grid[r][c] + mStructure[0]
            isSame = True
            for i in range(1, M):
                if height != grid[r+i][c] + mStructure[i]:
                    isSame = False
                    break
            if isSame:
                count += 1

    return count

def maxArea(M, mStructure, mSeaLevel):
    global grid

    return 0
