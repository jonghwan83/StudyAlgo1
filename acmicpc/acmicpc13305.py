import time
import random

# n = 100000
# cities = [random.randint(1, 10000) for _ in range(n-1)]
# prices = [random.randint(1, 1000000000) for _ in range(n)]

start = time.time()

import sys

n = int(sys.stdin.readline())
cities = list(map(int, sys.stdin.readline().split()))
prices = list(map(int, sys.stdin.readline().split()))

travel = cities[0] * prices[0]
price = prices[0]
for i in range(1, n-1):
    if prices[i] < price:
        price = prices[i]
    travel += price * cities[i]

print(travel)

'''
# sub tack 58 score
travel = cities[0] * prices[0]
for i in range(1, n-1):
    if prices[i] == min(prices[i:n-1]):
        travel += (sum(cities[i:] * prices[i]))
        break
    else:
        travel += cities[i] * min(prices[:i+1])
print(travel)


# sub task 17 score
oil = 0
i_min_price = prices.index(min(prices[:-1]))

for i in range(i_min_price):
    oil += (prices[i] * cities[i])
oil += (prices[i_min_price] * sum(cities[i_min_price:]))

print(oil)

'''
end = time.time()
print('elapsed time: {}'.format(end - start))