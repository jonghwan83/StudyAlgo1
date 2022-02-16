'''
time complexity failed, priority queue needed
'''
import time
import random

# v = 20000
# e = 20000
# s = random.randint(1, v)
# graph = [[0]*(v+1)] * (v+1)
# for _ in range(e):
#     u = random.randint(1, v)
#     vt = random.randint(1, v)
#     w = random.randint(1, 10)
#     graph[u][vt] = w

t0 = time.time()

import sys
from collections import deque

v, e = map(int, sys.stdin.readline().split())
s = int(sys.stdin.readline())

graph = [[] for _ in range(v+1)]
weights = [[] for _ in range(v+1)]
for _ in range(e):
    u, vt, w = map(int, sys.stdin.readline().split())
    graph[u].append(vt)
    weights[u].append(w)

visited = [False] * (v + 1)
visited[0] = True
dist = [sys.maxsize] * (v+1)

queue = deque()
queue.append(s)
visited[s] = True
dist[s] = 0

while queue:
    cnode = queue.pop()
    visited[cnode] = True
    for nextnode in range(len(graph[cnode])):
        if dist[graph[cnode][nextnode]] > dist[cnode] + weights[cnode][nextnode]:
            dist[graph[cnode][nextnode]] = dist[cnode] + weights[cnode][nextnode]

    minvalue = sys.maxsize    
    for i in range(1, v+1):
        if not visited[i] and 0 < dist[i] < minvalue:
            minvalue = dist[i]
            queue.append(i)

for i in range(1, v+1):
    if visited[i] == False:
        print('INF')
    else:
        print(dist[i])

t1 = time.time()
print('elapsed: {}'.format(t1 - t0))