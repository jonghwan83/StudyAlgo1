import time
start = time.time()

import sys

N = int(sys.stdin.readline())
arr = []
for _ in range(N):
    arr.append(list(map(int, sys.stdin.readline().split())))

arr.sort()
arr2 = []

for i in range(N):
    arr2.append(arr[i][1])

dp = [1 for _ in range(N)]
for i in range(1, len(arr2)):
    for j in range(i):
        if arr2[i] > arr2[j]:
            dp[i] = max(dp[i], dp[j] + 1)

print(N - max(dp))

end = time.time()
print('elapsed time: {}'.format(end - start))
