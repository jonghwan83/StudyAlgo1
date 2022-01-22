'''
바이토닉 수열
10
1 5 2 1 4 3 4 5 2 1
-> 7
'''
import time
start = time.time()

import sys

n = int(sys.stdin.readline())
arr = list(map(int, sys.stdin.readline().split()))

up = [1 for _ in range(n)]
down = [1 for _ in range(n)]

for i in range(1, n):
    for k in range(i):
        if arr[i] > arr[k]:
            up[i] = max(up[i], up[k] + 1)

for j in range(n-1, -1, -1):
    for k in range(j, n):
        if arr[j] > arr[k]:
            down[j] = max(down[j], down[k]+1)

dp = [1 for _ in range(n)]
for i in range(len(up)-1):
    dp[i] = up[i] + down[i] - 1

print(max(dp))

end = time.time()
print('elapsed time: {}'.format(end - start))