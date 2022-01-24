import time
start = time.time()

import sys

N, K = map(int, sys.stdin.readline().split())
arr = []
for _ in range(N):
    arr.append(int(sys.stdin.readline()))


t = []
for i in range(N-1, -1, -1):
    t.append(K // arr[i])
    K -= t[-1] * arr[i]

print(sum(t))

end = time.time()
print('elapsed time: {}'.format(end - start))
