import time
import random

t0 = time.time()

# n = 100000
# arr = []
# for _ in range(n):
#     temp = random.randint(0, 100)
#     if (temp == 0) and (len(arr) != 0):
#         arr = arr[0:len(arr)-1]
#     else:
#         arr.append(temp)

import sys

n = int(sys.stdin.readline())
arr = []
for _ in range(n):
    temp = int(sys.stdin.readline())
    if (temp == 0) and (len(arr) != 0):
        arr = arr[0:len(arr)-1]
    else:
        arr.append(temp)

print(sum(arr))

t1 = time.time()
print('elapsed: {}'.format(t1 - t0))