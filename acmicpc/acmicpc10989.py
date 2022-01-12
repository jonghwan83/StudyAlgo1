import sys

T = int(sys.stdin.readline())
lst = []
for _ in range(T):
    lst.append(int(sys.stdin.readline()))

index = [i for i in range(max(lst)+1)]
count = [0 for _ in range(max(lst)+1)]

for val in lst:
    count[val] += 1

for i in range(1, len(count)):
    count[i+1] += count[i]

print(count)
