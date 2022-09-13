import sys

sys.setrecursionlimit(int(1e7))

MOD = 1000000

N = int(sys.stdin.readline())
t1, t2, t3 = map(int, sys.stdin.readline().split())

pos = [0] * (N + 1)
for d1 in map(int, sys.stdin.readline().split()):
    pos[d1] = 1

for d2 in map(int, sys.stdin.readline().split()):
    pos[d2] = 2

for d3 in map(int, sys.stdin.readline().split()):
    pos[d3] = 3

pow_2 = [1] * (N + 1)
for i in range(1, N + 1):
    pow_2[i] = (pow_2[i - 1] * 2) % MOD

ans = 0

def hanoi(disc, end):
    global ans

    if disc:
        start = pos[disc]
        mid = ({1, 2, 3} - {start} - {end}).pop()

        if start == end:
            hanoi(disc-1, end)
        else:
            ans = (ans + pow_2[disc-1])
            hanoi(disc - 1, mid)

hanoi(N, pos[N])
print(pos[N])
print(ans)
