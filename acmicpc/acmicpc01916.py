import sys
from collections import deque

# sys.stdin = open('acmicpc/sample.txt', 'r')

n = int(sys.stdin.readline())
m = int(sys.stdin.readline())

graph = [[-1 for _ in range(n+1)] for _ in range(n+1)]

for i in range(m):
    start, end, fare = map(int, sys.stdin.readline().split())

    if graph[start][end] > -1:
        graph[start][end] = min(graph[start][end], fare)
    else:
        graph[start][end] = fare

departure, arrival = map(int, sys.stdin.readline().split())

min_fare = [sys.maxsize] * (n+1)
queue = deque()
visited = [[0 for _ in range(n+1)] for _ in range(n+1)]

queue.append(departure)
min_fare[departure] = 0
while queue:
    current = queue.popleft()
    for city, fare in enumerate(graph[current]):
        if fare != -1:
            if not visited[current][city] and ((min_fare[current] + graph[current][city]) < min_fare[city]):
                min_fare[city] = min_fare[current] + graph[current][city]
                queue.append(city)

print(min_fare[arrival])
