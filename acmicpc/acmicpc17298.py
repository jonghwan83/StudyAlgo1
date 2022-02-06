import time
import random

# N = 1000000
# A = [random.randint(1, 1000000) for _ in range(N)]

t0 = time.time()

import sys

N = int(sys.stdin.readline())
A = list(map(int, sys.stdin.readline().split()))

result = [-1 for _ in range(N)]
stack = []
for i in range(N-1, -1, -1):
    if len(stack) != 0:
        while stack[-1] <= A[i]:
            stack.pop()
            if len(stack) == 0:
                break
    if len(stack) != 0:
        result[i] = max(stack[-1], A[i])
    stack.append(A[i])
print(" ".join(str(i) for i in result))

t1 = time.time()
print("elapsed time: {}".format(t1 - t0))