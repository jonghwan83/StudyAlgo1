import time
import random

from numpy import true_divide

N = 100000
A = [random.randint(-2**31, 2**31) for _ in range(N)]
M = 100000
K = [random.randint(-2**31, 2**31) for _ in range(M)]

t0 = time.time()

import sys

N = int(sys.stdin.readline())
A = sorted(map(int, sys.stdin.readline().split()))
M = int(sys.stdin.readline())
K = list(map(int, sys.stdin.readline().split()))

for k in K:
    P = N // 2
    L = 0
    R = N 
    while True:
        if (A[R-1] < k) or (A[L] > k):
            print(0)
            break
        elif A[P] == k:
            print(1)
            break
        else:
            if k >= A[P]:
                L = P
            else:
                R = P
            P = (R - L) // 2 + L
        

t1 = time.time()
print('elapsed: {}'.format(t1 - t0))