import time

import sys

def fib(n, mz):
    if n in mz:
        return mz[n]
    if n == 0 or n == 1:
        return n
    else:
        mz[n-1] = fib(n-1, mz)
        mz[n-2] = fib(n-2, mz)
        return mz[n-1] + mz[n-2]

T = int(sys.stdin.readline())
for _ in range(T):
    n = int(sys.stdin.readline())
    mz = {}
    if n == 0:
        print(1, 0)
    elif n == 1:
        print(0, 1)
    else:
        print(fib(n-1, mz), fib(n, mz))