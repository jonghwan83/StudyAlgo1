import time

t0 = time.time()

import sys

T = int(sys.stdin.readline())
for _ in range(T):
    n, m = map(int, sys.stdin.readline().split())
    priorities = list(map(int, sys.stdin.readline().split()))

    printed = []
    while True:
        temp = priorities.pop(0)
        m -= 1
        if len(priorities) == 0:
            printed.append(temp)
            break
        if temp < max(priorities):
            priorities.append(temp)
            if m < 0:
                m = len(priorities) - 1
        else:
            printed.append(temp)
            if m < 0:
                break
    print(len(printed))

t1 = time.time()
print('elapsed time:{}'.format(t1 - t0))