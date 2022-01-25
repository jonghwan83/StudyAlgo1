import sys

N = int(sys.stdin.readline())
Pi = list(map(int, sys.stdin.readline().split()))

Pi.sort()
total = 0
for i in range(N):
    total += (N-i) * Pi[i]

print(total)