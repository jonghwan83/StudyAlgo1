import time

import sys

N = int(sys.stdin.readline())
arr = []
for _ in range(N):
    arr.append(list(map(int, sys.stdin.readline().split())))
start = time.time()

arr.sort()
t = 0
c = 0
for i in range(len(arr)):
    if t < arr[i][0]:
        t += arr[i][1]
        c += 1
print(t, c)

end = time.time()
print('elapsed time: {}'.format(end - start))