# 내장 heap을 사용한 경우

import time

start = time.time()

import sys
import heapq

# sys.stdin = open('acmicpc/sample.txt', 'r')
k = int(sys.stdin.readline())

maxheap = []
minheap = []

for _ in range(k):
    m = int(sys.stdin.readline())
    if len(maxheap) == len(minheap):
        heapq.heappush(maxheap, (-m, m))
    else:
        heapq.heappush(minheap, (m, m))

    if minheap and maxheap[0][1] > minheap[0][1]:
        a = heapq.heappop(minheap)[0]
        b = heapq.heappop(maxheap)[1]
        heapq.heappush(maxheap, (-a, a))
        heapq.heappush(minheap, (b, b))

    print(maxheap[0][1])

end = time.time()
print('elapsed: ', end - start)