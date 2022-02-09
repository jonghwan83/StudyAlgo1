import time
import random

t0 = time.time()

# N = 1000
# M = 10000
# V = random.randint(1, N)

# vertices = [i for i in range(1, N+1)]
# adjacents = [[] for _ in range(N+1)]

# for _ in range(M):
#     a, b = [random.randint(1, N), random.randint(1, N)]
#     adjacents[a].append(b)
#     adjacents[b].append(a)

import sys

N, M, V = list(map(int, sys.stdin.readline().split()))

vertices = [i for i in range(1, N+1)]
adjacents = [[] for _ in range(N+1)]

for _ in range(M):
    a, b = list(map(int, sys.stdin.readline().split()))
    adjacents[a].append(b)
    adjacents[b].append(a)

for l in adjacents:
    l.sort()

visited = [False] * (N+1)
def dfs_traverse(vertex, visited, arr):
    arr.append(vertex)
    visited[vertex] = True
    
    for i in adjacents[vertex]:
        if (visited[i]):
            pass
        else:
            dfs_traverse(i, visited, arr)
    return arr

arr = []
print(' '.join(str(x) for x in dfs_traverse(V, visited, arr)))

def bfs_trasverse(vertex):
    arr = []
    visited = [False] * (N+1)
    visited[vertex] = True
    q = []
    q.append(vertex)

    while len(q) != 0: # current = 2, [3, 4, 4]
        current = q.pop(0)
        arr.append(current)
        visited[current] = True
        for n in adjacents[current]:
            if visited[n] == False:
                visited[n] = True
                q.append(n)
    return arr

print(' '.join(str(x) for x in bfs_trasverse(V)))

t1 = time.time()
print('elapsed: {}'.format(t1 - t0))