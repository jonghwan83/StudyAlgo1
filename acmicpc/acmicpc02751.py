import sys

T = int(sys.stdin.readline())
lst = []
for _ in range(T):
    lst.append(int(sys.stdin.readline()))

lst.sort()
for i in lst:
    print(i)