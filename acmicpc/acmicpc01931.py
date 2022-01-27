import time
# import random

# N = 100000
# meetings = []
# for _ in range(N):
#     a = random.randint(0, 2**31 - 1)
#     b = random.randint(a, 2**31 - 1)
#     meetings.append([a, b])

start = time.time()

import sys

N = int(sys.stdin.readline())
meetings = []
for _ in range(N):
    meetings.append(list(map(int,sys.stdin.readline().split())))

meetings.sort()
intervals = []
for lst in meetings:
    intervals.append(lst[1] - lst[0])

cum = [1] * N
nTime = [0] * N
nTime[0] = meetings[0][1]

for i in range(1, N):
    cum[i] = cum[i-1]
    nTime[i] = nTime[i-1]
    if meetings[i][0] >= nTime[i]:
        cum[i] += 1
        nTime[i] = meetings[i][1]
    elif intervals[i] < intervals[i-1]:
        nTime[i] = min(meetings[i][1], nTime[i])

print(cum[-1])

end = time.time()
print('elapsed time: {}'.format(end - start))
