import sys

T = int(sys.stdin.readline())
lst = [0 for _ in range(10001)]
for _ in range(T):
    lst[int(sys.stdin.readline())] += 1

for i in range(len(lst)):
    if (lst[i] != 0):
        n = lst[i]
        while (n > 0):
            print(i)
            n -= 1
