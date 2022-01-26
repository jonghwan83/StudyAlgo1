import time
import random

n = 20000
cities = [random.randint(1, 10000) for _ in range(n-1)]
prices = [random.randint(1, 10000) for _ in range(n)]

start = time.time()

import sys

# n = int(sys.stdin.readline())
# cities = list(map(int, sys.stdin.readline().split()))
# prices = list(map(int, sys.stdin.readline().split()))

travel = 0
oil = 0
for i in range(len(cities)):
    if min(prices[i:n-1]) == prices[i]:
        oil += (prices[i] * (sum(cities) - travel))
        break
    else:
        oil += (cities[i] * prices[i])
    travel += cities[i]

print(oil)

end = time.time()
print('elapsed time: {}'.format(end - start))