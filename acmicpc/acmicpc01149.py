import time

import sys

def getMinCost(cost, mz):
    
    mz[0] = cost[0]
    for i in range(1, len(cost)):
        mz[i] = [min(mz[i-1][1], mz[i-1][2]) + cost[i][0], 
            min(mz[i-1][0], mz[i-1][2]) + cost[i][1],
            min(mz[i-1][0], mz[i-1][1]) + cost[i][2]]
    return mz
    
n = int(sys.stdin.readline())
cost = []
for _ in range(n):
    cost.append(list(map(int, sys.stdin.readline().split())))

mz = {}
result = getMinCost(cost, mz)
print(min(result[len(cost) - 1]))