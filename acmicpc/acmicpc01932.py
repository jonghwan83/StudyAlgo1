import sys

n = int(sys.stdin.readline())
T = []
for i in range(n):
    T.append(list(map(int, sys.stdin.readline().split())))

for i in range(len(T) - 1, 0, -1):
    for j in range(len(T[i])-1):
        temp = max(T[i][j], T[i][j+1])
        T[i-1][j] += temp

print(T[0][0])